#include <stdio.h>
#include <string.h>
#include "log.h"
#include "readclock.h"

extern int dolog;

extern int seconds;
extern int updateclock;
extern int limit;
extern char * portfile;
extern int recheck;

void
log_quedan(int quedan)
{
	char str[MAXLOGLINE];
	if (dolog) {
		memset(str,0,MAXLOGLINE);
		sprintf(str,"There are %d bytes to read from the port",quedan);
		LOG(str);
	}
}

void
log_data(unsigned char *data,int leidos)
{
	int n;
	unsigned char chksum;
	char str[MAXLOGLINE];

//	if (dolog) {
		for (chksum=0,n=1;n<9;n++)
			chksum+=(data[n]&0xff);

		memset(str,0,MAXLOGLINE);
 		sprintf(str,"read=%d H=%02x %02d:%02d:%02d %02d%02d%02d%02d wd=%d cs=%d mcs=%d",
			leidos,data[0],data[1],data[2],data[3],data[4],
			data[5],data[6],data[7],data[8],data[9],chksum);
	
		LOG(str);
//	}
}

void
log_seconds(int actual, int recibido)
{
	char str[MAXLOGLINE];

	if (dolog) {
		memset(str,0,MAXLOGLINE);
		sprintf(str,"now=%d received=%d difference=%d",
				actual,recibido,actual-recibido);
		LOG(str);
	}
}

void
log_conf(void)
{
	char str[MAXLOGLINE];

	memset(str,0,MAXLOGLINE);
	sprintf(str,"CONF: seconds=%d log=%s updateclock=%s limit=%d recheck=%d device=%s",
			seconds,
			(dolog==0)?"no":"yes",
			(updateclock==0)?"no":"yes",
			limit,
			recheck,
			portfile);
	LOG(str);
}
