#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <time.h>
#include "readclock.h"
#include "cfgread.h"
#include "log.h"

int fd=-1;

int seconds=DEF_SECONDS;
int dolog=DEF_DOLOG;
int updateclock=DEF_UPDATECLOCK;
int limit=DEF_LIMIT;
char * portfile=DEF_DEVICE;
int recheck=DEF_RECHECK;

void sig_salir(int sig);
void sig_segv(int sig);

int
open_port(char *p)
{
	int puerto;
	if (p == NULL)
		return(-1);
	puerto=open(p , O_RDONLY | O_NOCTTY | O_NDELAY | O_NONBLOCK);
	if (puerto != -1)
		fcntl(puerto,F_SETFL,0);
	return(puerto);
}

void
config_port(int p)
{
	struct termios options;
	tcgetattr(p,&options);
	cfsetispeed(&options,B4800);
	cfsetospeed(&options,B4800); //por seguridad
	options.c_cflag |= (CLOCAL | CREAD);

	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); 

	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;


	tcsetattr(p,TCSANOW,&options);
}

void
config_signals(void)
{
	signal(SIGINT,sig_salir);
	signal(SIGQUIT,sig_salir);
	signal(SIGTERM,sig_salir);
	signal(SIGHUP,sig_salir);
	signal(SIGSEGV,sig_segv);
}

int
to_seconds(unsigned char *data)
{
	struct tm tmvar;

	memset(&tmvar,0,sizeof(struct tm));
	tmvar.tm_hour=data[HORA];
	tmvar.tm_min=data[MINUTOS];
	tmvar.tm_sec=data[SEGUNDOS];
	tmvar.tm_year=((data[CENTENARIO]*100)+data[ANIO])-1900;
	tmvar.tm_mon=data[MES]-1;
	tmvar.tm_mday=data[DIA];

	return(mktime((struct tm*)&tmvar));
}

int
demonio(void)
{
	switch(fork()) {
		case -1:
			return(-1);
		case 0:
			break;
		default:
			exit(0);
	}
	setsid();
	chdir(WORKDIR);
	return(0);
}

int
check_cluster(char *myname)
{
	FILE *mipipe;
	char cl_activo[256];
	int retval;
	if ( !myname )
		return(0);

	mipipe=popen("cmviewcl -p pkg1 | awk '/pkg1/ {print $5}'","r");
	if ( mipipe == NULL) {
		LOG("check_cluster: can't open pipe");
		return(0);
	}

	if ( fscanf(mipipe,"%s",cl_activo) < 1 ) {
		LOG("check_cluster: read nothing");
		return(0);
	}

	retval=0;

	if ( strcmp(cl_activo,myname) == 0 ) 
		retval=1;

	pclose(mipipe);
	return(retval);
}

int
flush_port(int portfd)
{
	char *buf;
	int remaining=0;

	ioctl(portfd,FIONREAD,&remaining);
	if (remaining >= 4 ) {
		buf=(char *)malloc(remaining);
		if ( buf == NULL ) {
			LOG("ERROR: Can't reserver buffer for flush\n");
			return(-1);
		}
		read(portfd,buf,remaining);
		free(buf);
	}
	return(0);
}

int
main (int argc, char *argv[])
{
	int leidos;
	int quedan,n;
	struct tm tactual;
	struct tm mitiempo;
	time_t tsegundosactual;
	int diferencia;
	time_t misegundo;
	unsigned char michecksum;
	unsigned char buffer[10];
	char myhostname[256];
	int num_recheck;

	open_log(LOGFILENAME);

	read_conf(CONFFILE);
	log_conf();

	if (gethostname(myhostname,256)) {
		LOG("ERROR: Can't get hostname");
		return(1);
	}

	LOG(myhostname);
        if ( !check_cluster(myhostname) ) {
                LOG("I am NOT the master, bye!");
		close_log();
		return(0);
        } else {
		LOG("I am the MASTER");
	}

	demonio();
	
	fd=open_port(portfile);
	if (fd == -1) {
		LOG("ERROR: Can't open port. I'll stop here\n");
		close_log();
		close(fd);
		return(0);
	}
	
	config_port(fd);
	config_signals();

	flush_port(fd);


	num_recheck=0;

	while (1) {
	
		memset(buffer,0,10);
	
		quedan=0;
		do {
			ioctl(fd,FIONREAD,&quedan);
			sleep(1);
		} while (!quedan);
		tsegundosactual=time(NULL);
	
		log_quedan(quedan);

		leidos=read(fd,buffer,10);

		if (leidos<10) {
			LOG("ERROR: less than 10 bytes read\n");
			continue;
		}
	
		if (comprobar_valores(buffer)) {
			LOG("ERROR: The values are incorrect");
			continue;
		}
		
		log_data(buffer,leidos);

		localtime_r(&tsegundosactual,(struct tm *)&tactual);
		misegundo=to_seconds(buffer);
		diferencia=tsegundosactual-misegundo;
		log_seconds(tsegundosactual,misegundo);

		if (diferencia<0)
			diferencia=-diferencia;

		if ((diferencia >= seconds) 
		    && (diferencia <= limit)) {
		     if ( num_recheck < 1 ) {
			if (updateclock) {
				LOG("Time will be updated");

				misegundo++; // le sumo el que me duermo

				if (stime(&misegundo)==0) {
					LOG("Time updated succesfully");
				} else {
					LOG("ERROR: Time not updated");
				}
			} else {
				LOG("Time would be updated");
			}
		     } else {
			num_recheck--;
			LOG("Waiting for next time check to update time");
		     }
		} else {
			num_recheck=recheck-1;
		} 
	}	
	close_log();
	close(fd);
	return(0);
}

void
sig_salir(int sig)
{
	close(fd);
	close_log();
	_exit(0);
}

void
sig_segv(int sig)
{
	close(fd);
	safe_log("I SIGSEV\n");
	close_log();
	_exit(0);
}
