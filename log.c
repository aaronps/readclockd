/***************************************************************************
 *            log.c
 *
 *  Tue Jun 22 13:15:35 2004
 *  Copyright  2004  Aaron Perez
 *  aaronperezsanchez@hotmail.com
 ****************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "log.h"
#include "readclock.h"

int fdlog=-1;

int
open_log(char *n)
{
	if ( !n )
		return(-1);
	
	fdlog=open(n, O_WRONLY | O_APPEND | O_CREAT | O_DSYNC);
	if (fdlog == -1) 
		return(-1);
	LOG("Log started");	
	return(0);
} 

void
close_log(void)
{
	if (fdlog != -1) {
		close(fdlog);
		fdlog=-1;
	}
}

void
LOG(char *s)
{
	int mitime;
	char str[MAXLOGLINE];
	struct tm mitm;

	if (fdlog == -1)
			return;

	mitime=time(NULL);
	localtime_r(&mitime,(struct tm *)&mitm);
	memset(str,0,MAXLOGLINE);

	sprintf(str,"%04d%02d%02d %02d:%02d:%02d ",
		mitm.tm_year+1900,mitm.tm_mon+1,mitm.tm_mday,
		mitm.tm_hour,mitm.tm_min,mitm.tm_sec);

	if (strlen(s) > (MAXLOGLINE-19)) {
		strcat(str,"LOGERROR: Line exceded line length limit");
	} else {
		strcat(str,s);
	}
	strcat(str,"\n");
	write(fdlog,str,strlen(str));
}

void
safe_log(char *s)
{
	int len;
	
	if ( (fdlog == -1) || (s == NULL) )
		return;
	
	len=0;
	while (s[len])
			len++;
	
	write(fdlog,s,len);
}
