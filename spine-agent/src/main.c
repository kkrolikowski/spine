#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/stat.h>

pid_t recvp = -1;
pid_t sendp = -1;
pid_t agentp = -1;

void kill_workers(int sig);

int main(int argc, char *argv[]) {
	pid_t parent, sid;

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
		// odpalamy proces odbierajacy dane
		recvp = fork();
		if(!recvp) {
			sprintf(argv[0], "spine-reciver");
			while(1)
				sleep(1);
		}
		else if(recvp > 0) {
			agentp = getppid();
			// odpalamy proces wysylajacy dane
			sendp = fork();
			if(!sendp) {
				sprintf(argv[0], "spine-sender");
				while(1)
					sleep(1);
			}
			while(1) {
				if(signal(SIGTERM, kill_workers)) {
					wait(NULL);
					break;
				}
				else
					sleep(1);
			}
		}
	}
	return 0;
}
void kill_workers(int sig) {
	kill(recvp, SIGTERM);
	kill(sendp, SIGTERM);
}
