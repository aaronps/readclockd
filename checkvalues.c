#include "log.h"
#include "readclock.h"

int
comprobar_valores(unsigned char *datos)
{
        unsigned char diasxmes[13]={0,31,29,31,30,31,30,31,31,30,31,30,31};
        unsigned char michecksum;
        int n;

        if (datos[CABECERA] != 0x5c) {
                LOG("ERROR: Head is not 0x5c");
                return (-1);
        }

        if (datos[HORA] > 23) {
                LOG("ERROR: hour is > 23");
                return (-1);
        }

        if (datos[MINUTOS] > 59) {
                LOG("ERROR: minutes are > 59");
                return(-1);
        }

        if (datos[SEGUNDOS] > 59) {
                LOG("ERROR: seconds are > 59");
                return(-1);
        }

        if ((datos[CENTENARIO] < 20) || (datos[CENTENARIO] > 21)) {
                LOG("ERROR: century is < 20 or > 21");
                return(-1);
        }

        if (datos[ANIO] > 99) {
                LOG ("ERROR: year is > 99");
                return(-1);
        }

        if ((datos[ANIO] < 4) && (datos[CENTENARIO] == 20)) {
                LOG ("ERROR: year is less than 2004");
                return(-1);
        }

        if ((datos[MES] < 1) || (datos[MES] > 12)) {
                LOG("ERROR: month is not between 1 and 12");
                return(-1);
        }

        if ((datos[DIA] < 1) || (datos[DIA] > diasxmes[datos[MES]])) {
                LOG ("ERROR: day is not valid for this month");
                return(-1);
        }

        if ((datos[ANIO]%4)               //si no es bisiesto
                && (datos[MES] == 2)      //y es febrero
                && (datos[DIA] > 28)) {   //y el dia es mas de 28
                LOG ("ERROR: this year February doesn't have more than 28 days")
;
                return(-1);               //entonces esta mal
        }

        if (datos[DIASEMANAL] > 6) { // es 6 o es 7?
                LOG("ERROR: day is > 6");
                return(-1);
        }

        for (michecksum=0,n=1;n<9;n++)
                michecksum+=(datos[n]&0xff);
        if (datos[CHECKSUM] != michecksum) {
                LOG("ERROR: checksums are not equal");
                return(-1);
        }

        return(0);
}
