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
char mode[8];					// tryb pracy

void kill_workers(int sig);		// nie, nie jest to funcja do zabijania pracownikow ;-)

int main(int argc, char *argv[]) {
	FILE * cf; 					// uchwyt pliku z konfiguracyjnego
	FILE * lf;					// uchwyt pliku z logami
	pid_t parent, sid;			// PID procesu macierzystego oraz ID sesji
	config_data cfd;			// dane z pliku konfiguracyjnego
	char * logentry = NULL;		// wskaznik do informacji o zdarzeniu
	extern char mode[8];		// tryb pracy

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
		fclose(cf);
		exit(EXIT_FAILURE);
	}
	fclose(cf); 				// dane odczytane, mozna zamknac plik.

	// ustawiamy tryb pracy agenta
	memset(mode, '\0', 8);
	strncpy(mode, cfd.mode, 8);

	// uruchamiamy logowanie do pliku
	if((lf = fopen(cfd.logpath, "a+")) == NULL) {
		fprintf(stderr, "[CRIT] Blad zapisu pliku: %s\n", cfd.logpath);
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
		logentry = mkString("[INFO] (main) SpineAgent startuje", NULL);
		writeLog(lf, logentry);
		// odpalamy proces odbierajacy dane
		recvp = fork();
		if(!recvp) {
			sprintf(argv[0], "spine-reciver");
			logentry = mkString("[INFO] (reciver) * reciver ... Done", NULL);
			writeLog(lf, logentry);
			RetrieveData(cfd.port, lf);
		}
		else if(recvp > 0) {
			agentp = getppid();
			if(!strcmp(mode, "client")) {
				// odpalamy proces wysylajacy dane
				sendp = fork();
				if(!sendp) {
					sprintf(argv[0], "spine-sender");
					logentry = mkString("[INFO] (sender) * sender ... Done", NULL);
					writeLog(lf, logentry);
					SendData(cfd.mode, cfd.host, cfd.port, lf);
				}
			}
			/* tutaj nie robimy nic innego tylko czekamy na sygnal
			do wylaczenia procesow 								*/
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
	fclose(lf);				// zamykamy logi
	return 0;
}
void kill_workers(int sig) {
	kill(recvp, SIGTERM);
	if(!strcmp(mode, "client"))
		kill(sendp, SIGTERM);
}
