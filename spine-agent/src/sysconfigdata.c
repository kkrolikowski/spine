/* sysconfigdata.c -- zestaw funkcji do odczytywania i konfiguracji systemu */
#include <sys/sysinfo.h>
#include "sysconfigdata.h"

long getuptime(void) {
	struct sysinfo sys;

	if(!sysinfo(&sys))
		return sys.uptime;
	else
		return -1;
}
int getSystemInformation(systeminfo * sys) {
	int status = 0;
	if((sys->uptime = getuptime()) > 0)
		status = 1;

	return status;
}
void InitSystemInformation(systeminfo * sys) {
	sys->uptime = 0L;
}
