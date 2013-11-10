#define true 1
#define false 0
/**
	Escriu un string a un socket
*/
int wsock(int sock, char *string);
/**
	Si el socket es no blockejant retorna -1 si no hi ha dades
	No es pot llegir l'error que es genera per saber si es que no hi havien dades o es que hi ha problemes
*/
int rsock(int sock, char *buffer, int buffersize);
#define listensock(port,maxClients) create_listen_sock(port,maxClients,true)
#define listensock_nowait(port,maxClients) create_listen_sock(port,maxClients,false)
/**
	Crea un socket que escolta en un port per un maxim de clients
	- crida no bloquejant
	- es pot definir si es vol un socket bloquejant o no
*/
int create_listen_sock(int puerto, int maxClientes,bool wait);
/**
	Accepta una conexio
	- Es bloquejant si el socket es bloquejant
*/
int acceptsock(int sockEscucha);

