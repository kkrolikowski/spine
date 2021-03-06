#ifndef SPINE_AGENT_SRC_MONITORING_H_
#define SPINE_AGENT_SRC_MONITORING_H_

/*			DANE			*/

// informacje na temat stanu uslug na hostach
typedef struct monitoring {
	int apache_status;
	int sshd_status;
        int mysql_status;
} monitoring;

// statystyki interfejsu sieciowego
typedef struct netifstats {
   unsigned int bytes_in;       // liczba bajtow odebranych przez interfejs
   unsigned int bytes_out;      // liczba bajtow wyslanych przez interfejs
} netifstats;

// prosta struktura klucz/wartosc.
// przechowa dane dotyczace monitoringu
typedef struct keyval {
	char * key;
	char * val;
} kv;

/*			FUNKCJE			*/

// funkcja sprawdza czy dziala serwer apache
// 1 - apache dziala poprawnie
// 0 - apache nie dziala
int apacheAlive(void);
int mysqlAlive(void);
// funkcja sprawdza czy dziala serwer ssh
// 1 - ssh dziala poprawnie
// 0 - ssh nie dziala
int sshdAlive(void);

// funkcja zbiera dane na temat monitoringu uslug.
// wypelnie strukture srvdata danymi pochadzacymi z check
void getServiceStatus(monitoring * srvdata, int (*check[])(void));

// funkcja zwraca string zawierajacy dane monitoringu, ktore sa zaszyte
// w jsonie przeslanym przez klienta
char * rawMonitoringData(const char * clientResp);

// funckja zlicza ile kluczy (checkow) zostalo przeslanych przez klienta
// Liczba zostanie uzyta do zainicjowania i przetworzenia tablicy klucz/wartosc
int checks(char * raw);

// funkcja odczytuje dane  monitoringu zwrocone przez rawMonitoringData()
// i zapisuje je w tablicy klucz/wartosc: kvdata.
void ReadRawMonitoringData(char * raw, kv kvdata[], int n);

// inicjumemy tablice z checkami
void InitCheckData(kv data[], int n);

// czyscimy tablice z checkami
void ClearCheckData(kv data[], int n);

// funkcja pobiera ilosc bajtow wyslanych oraz odebranych z interfejsu
int getNetifStats(netifstats * ifstats);

#endif /* SPINE_AGENT_SRC_MONITORING_H_ */
