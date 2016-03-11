#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
	pid_t parent, agentp, recvp, sendp, sid;

	if((parent = fork()) < 0) {
		fprintf(stderr, "[CRIT] Nie moge stworzyc procesu\n");
		exit(EXIT_FAILURE);
	}
	if(parent > 0)
		exit(EXIT_SUCCESS);

	// demonizujemy proces
	umask(0);
	sid = setsid();
	if(sid < 0)
		exit(EXIT_FAILURE);
	if((chdir("/")) < 0)
		exit(EXIT_FAILURE);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	// uruchamiamy proces nadrzedny
	agentp = fork();
	if(!agentp) {
		recvp = fork();
		if(!recvp)
			while(1)
				sleep(1);
		sendp = fork();
		if(!sendp)
			while(1)
				sleep(1);
		while(1)
			sleep(1);
	}
	return 0;
}
