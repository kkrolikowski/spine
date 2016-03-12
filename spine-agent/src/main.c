#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "core.h"

pid_t recvp = -1;
pid_t sendp = -1;
pid_t agentp = -1;

void kill_workers(int sig);		// nie, nie jest to funcja do zabijania pracownikow ;-)

int main(int argc, char *argv[]) {
	FILE * cf; 					// uchwyt pliku z logami
	pid_t parent, sid;			// PID procesu macierzystego oraz ID sesji
	config_data cfd;			// dane z pliku konfiguracyjnego

	// sprawdzamy czy program jest uruchomiony z prawami roota
	if(norootUser()) {
		fprintf(stderr, "Uruchom program jako root\n");
		exit(EXIT_FAILURE);
	}
	// Inicjujemy konfiguracje
	InitConfigData(&cfd);

	// sprawdzamy, czy zostala podana sciezka do pliku konfiguracyjnego
	// jesli nie, to szukamy go w katalogu /etc
	if(argc < 2) {
		if((cf = fopen("/etc/spine-agent.conf", "r")) == NULL) {
			fprintf(stderr, "Skladnia: %s [/sciezka/do/spine-agent.conf]\n", argv[0]);
			exit(EXIT_FAILURE);
		}
	}
	else {
		if((cf = fopen(argv[1], "r")) == NULL) {
			fprintf(stderr, "Blad odczytu pliku %s\n", argv[1]);
			exit(EXIT_FAILURE);
		}
	}
	// jesli udalo sie poprawnie otworzyc plik konfiguracyjny
	// sprobujemy wyciagnac z niego dane
	if(!ReadConfig(&cfd, cf)) {
		fprintf(stderr, "[CRIT] Dane w pliku konfiguracyjnym sa niepoprawne\n");
		exit(EXIT_FAILURE);
	}

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
	fclose(cf);
	return 0;
}
void kill_workers(int sig) {
	kill(recvp, SIGTERM);
	kill(sendp, SIGTERM);
}
