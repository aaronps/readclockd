#ifndef _READCLOCK_H
#define _READCLOCK_H


enum {
	CABECERA=0,
	HORA,
	MINUTOS,
	SEGUNDOS,
	CENTENARIO,
	ANIO,
	MES,
	DIA,
	DIASEMANAL,
	CHECKSUM
};

#define LOGFILENAME "/var/adm/readclock.log"
#define CONFFILE "/etc/readclock.conf"
#define WORKDIR "/tmp"
#define MAXLOGLINE 1024
#define MIN_SECONDS 15
// 300 = 5 minutes
#define MAX_SECONDS 300
// 120 = 2 minutes
#define MIN_LIMIT 120
// 1800 = 30 minutes
#define MAX_LIMIT 1800
#define MIN_RECHECK 1
#define MAX_RECHECK 100

#define DEF_RECHECK 3
#define DEF_DEVICE "/dev/tty0p3"
#define DEF_LIMIT 300
#define DEF_SECONDS 30
#define DEF_DOLOG 1
#define DEF_UPDATECLOCK 0

void log_quedan(int);
void log_data(unsigned char *, int);
void log_seconds(int,int);
void log_conf(void);
int comprobar_valores(unsigned char *);



#endif
