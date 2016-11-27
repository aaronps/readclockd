#include <stdio.h>
#include <string.h>
#include "cfgread.h"
#include "readclock.h"
#include "log.h"

extern int dolog;
extern int updateclock;
extern int seconds;
extern int limit;
extern char * portfile;
extern int recheck;


void
read_conf(char *filename)
{
        FILE *f;
        char linea[1024];
        char palabra[1024];
        char *punt;
        char *deviceok;
        int numero;
        int leidos;

	if ( filename == NULL) {
		LOG("Config filename not defined, using defaults\n");
		return;
	}

        f=fopen(CONFFILE,"r");
        if (f == NULL) {
                LOG("Can't open configuration file, using defaults\n");
                return;
        }

        while (!feof(f)) {
                memset(linea,0,1024);
                memset(palabra,0,1024);
                numero=0;
                leidos=0;
                punt=fgets(linea,1024,f);

                if (punt == NULL)
                        break;

                if (read_yesno(linea,"log",&dolog))
                        continue;

                if (read_yesno(linea,"updateclock",&updateclock))
                        continue;

                if (read_num(linea,"seconds",&seconds,MIN_SECONDS,MAX_SECONDS))
                        continue;

                if (read_num(linea,"limit",&limit,MIN_LIMIT,MAX_LIMIT))
                        continue;

                if (read_num(linea,"recheck",&recheck,MIN_RECHECK,MAX_RECHECK))
                        continue;

                if (sscanf(linea,"device %s",palabra) > 0) {
                        if (strncmp(palabra,"/dev/",5) == 0) {
                                deviceok=strdup(palabra);
                                if (deviceok != NULL)
                                        portfile=deviceok;
                        }
                        continue;
                }
        }
        fclose(f);
}

int
read_yesno(char *s, char *q, int *v)
{
	char pal[1024];
	char query[1024];
	int retval;

	retval=0;
	sprintf(query,"%s %%s",q);
	if (sscanf(s,query,pal) > 0) {
        	if (strcmp(pal,"yes") == 0) {
                	*v=1;
			retval=1;
                } else if (strcmp(pal,"no") == 0) {
                	*v=0;
			retval=1;
                } 
	}
	return(retval);
}

int
read_num(char *s, char *q, int *v, int min, int max)
{
	int num;
	char query[1024];
	int retval;

	retval=0;
	sprintf(query,"%s %%d",q);
	if (sscanf(s,query,&num) > 0) {
		if ( num < min ) {
			*v=min;
			retval=1;
		} else if ( num > max ) {
			*v=max;
			retval=1;
		} else {
			*v=num;
			retval=1;
		}
	}
	return(retval);
}
