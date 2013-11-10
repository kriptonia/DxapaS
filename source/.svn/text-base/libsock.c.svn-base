#include <PA9.h>
#include <stdio.h>
#include "global.h"

/**
	Escriu un string a un socket
*/
int wsock(int sock, char *string) {
	int ssize;
	ssize=send(sock,string,strlen(string),0);
	return ssize;
}
/**
	Si el socket es no blockejant retorna -1 si no hi ha dades
	No es pot llegir l'error que es genera per saber si es que no hi havien dades o es que hi ha problemes
*/
int rsock(int sock, char *buffer, int buffersize) {
	int rsize;
	//if(sock==NULL) { return -1; }
	rsize=recv(sock,buffer,buffersize,0);
	return rsize;
}

/**
	Crea un socket que escolta en un port per un maxim de clients
	- no bloquejant
*/
int create_listen_sock(int port, int maxClients, bool wait) {
	struct sockaddr_in server;
	int s_id;
	int rc;
	int w;
 	//int numeroRaro;

	rc = socket(PF_INET, SOCK_STREAM, 0);
	if (rc == -1) return -1;
	s_id = rc;

	//El posem com a no bloquejant si així li indiquem
	if(!wait) {
		w=DONTWAIT;
		ioctl(rc,FIONBIO,&w);
	}
	/*
	// evita problemas al 'bind' pero no esta implementat al dswifi
	rc = setsockopt(s_id, SOL_SOCKET,SO_REUSEADDR,&numRaro,sizeof(int));
	if (rc == -1) return -1;
	*/

	
	server.sin_family = AF_INET;
	server.sin_port   = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;
	
	rc = bind(s_id, (struct sockaddr *)&server, sizeof(server));
	if (rc == -1) return -1;

	rc = listen(s_id, maxClients);
	if (rc == -1) return -1;
	else return s_id;
}
/**
	Accepta una conexio
	- Es bloquejant
*/
int acceptsock(int sockListen) {
    int ns;
    struct sockaddr_in client;  // para recibir la direccion del cliente
    int namelen;

    namelen = sizeof(client);
    ns = accept(sockListen, (struct sockaddr *)&client, &namelen);

    return ns;
}
