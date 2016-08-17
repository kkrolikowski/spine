/* monitoring.c -- funkcje do monitorowania uslug */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "network.h"
#include "core.h"

int apacheAlive(void) {
	// request do wyslania do apacza
	char * http_req = mkString("HEAD / HTTP/1.1\nHost: localhost\n\n", NULL);

	// przygotowujemy bufor
	const int SIZE = 256;
	char buff[SIZE];
	memset(buff, '\0', SIZE);

	// nawiazujemy polaczenie z apaczem
	int http_fd = connector("localhost", 80);
	if(http_fd < 0) {
		free(http_req);
		return 0;
	}

	// wysylamy request
	strncpy(buff, http_req, SIZE);
	if(write(http_fd, buff, SIZE) < 0) {
		close(http_fd);
		free(http_req);
		return 0;
	}

	// przygotowujemy bufor i pobieramy odpowiedz
	memset(buff, '\0', SIZE);
	if(read(http_fd, buff, SIZE) < 0) {
		close(http_fd);
		free(http_req);
		return 0;
	}

	// sprawdzamy kod odpowiedzi apacza
	if(strstr(buff, "HTTP/1.1 200 OK") == NULL) {
		close(http_fd);
		free(http_req);
		return 0;
	}

	// czynnosci koncowe
	free(http_req);
	close(http_fd);
	return 1;
}
