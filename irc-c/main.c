#include <stdio.h>
#include <sys/types.h>
#include <sys/sockets.h>
#define BUFFER_SIZE 1024
typedef struct {
	char nickname[12];	
	char realname[40];
} t_userinfo;

typedef struct {
	char name[40];
	//server ip
	int port;	//should be 6667
} t_serverinfo;

typedef struct {
	t_userinfo user;
	t_serverinfo server;
	struct hostent *Host; 
	struct sockaddr_in Direccion;
	int socket;
	char buffer[BUFFER_SIZE];
} t_conecinfo;

//ssize_t recv(int s, void *buf, size_t len, int flags);
//FLAG MSG_DONTWAIT
void userinfo(t_userinfo *u, char *nickname, char *realname) {
	strcpy(&u->nickname,nickname);
	strcpy(&u->realname,realname);
	return u;
}
void irc_send(t_conecinfo *c, char *message) {
	//ssize_t send(int s, const void *buf, size_t len, int flags);
	send(c->socket,message,strlen(message),0);
}

void irc_check_socket(t_conecinfo *c) {
	int recived_size;
	recived_size=recv(c->socket, c->buffer, BUFFER_SIZE, MSG_DONTWAIT);
	if(recived_size>0) {
		printf(">> %s",c->buffer);
		if(c->buffer[0]==':') {
			//message recibed
		} else {
			//Command recibed
			char command[80];
			char params[80];
			sscanf(c->buffer,"%s %s", &command,&params);
			if(strcmp(&command,"PING")) {
				char buffer[BUFFER_SIZE];
				sprintf(&buffer,"PONG %s \n",&params);
				irc_send(c,buffer);
			}
			
		}
	}
}



void serverinfo(t_serverinfo *s, char *name, int port) {
	strcpy(&s->name,name);
	s->port=port;
}


int main (void) {
	t_conecinfo conec;
	userinfo(&conec.user,"DSpr12","DSpr12);
	serverinfo(&conec.server,"irc.irc-hispano.org",6667);
	//Conectar
	//

}

