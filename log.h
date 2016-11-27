/***************************************************************************
 *            log.h
 *
 *  Tue Jun 22 13:12:25 2004
 *  Copyright  2004  Aaron Perez
 *  aaronperezsanchez@hotmail.com
 ****************************************************************************/

#ifndef _LOG_H
#define _LOG_H

int log_open(char *);
void log_close(void);
void LOG(char *);
void safe_log(char *);

#endif /* _LOG_H */
