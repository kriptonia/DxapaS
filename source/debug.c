#include <PA9.h>
#include "libsock.h"
#include "global.h"

/**
Treu el text passat per la sortida configurada al debug o en cas d'estar 
desactivat no fa res
*/
void debug(char *text) {
	if(!debug_mode) return;
	if(debug_mode==DEBUG_SOCKET) {
		send(debug_socket,text,strlen(text),0);
		/*if(errno!=EWOULDBLOCK) {
			error("S'hauria de bloquejar");
		}
		*/
	} else if (debug_mode==DEBUG_DISPLAY) {
		PA_SetTextTileCol(TOP_SCREEN,PINK);
		PA_Print(TOP_SCREEN,text);
		PA_SetTextTileCol(TOP_SCREEN,NORMAL);
	}
}

/**
Inicialitza el debug:
 mode: DEBUG_SOCKET 1, DEBUG_DISPLAY
 El mode assigna el display o un socket a la sortida de missatges.
 En cas de seleccionar un socket, es posa en mode escolta al port 7773 
esperant una conexió
*/
void debug_init(u8 mode) {
	int wait;
	debug_mode=mode;
	if(mode==DEBUG_SOCKET) {
		PA_SetTextTileCol(TOP_SCREEN,PINK);
		PA_Print(TOP_SCREEN,"Esperant client al port 7773\n");
		PA_SetTextTileCol(TOP_SCREEN,NORMAL);
		listen_sock=listensock(7773, 1);
		debug_socket=acceptsock(listen_sock);
		closesocket(listen_sock);
		wait=DONTWAIT;
		ioctl(debug_socket,FIONBIO,&wait);
	}
	debug("Debug inicialitzat\n");
}

/**
Procesa els comandes que se li envien.
Comandes:
 set pos element px py
 set vel element vx vy
 set mass element massa
 debug collision [on|off]
 unpause

Unpause:
Quan tenim debug collision on, quan hi ha un xoc mostra l'informació i 
el joc es para. Amb aquest comanda seguim fins al seguent xoc.
*/
void debug_read(void) {
	s16 s;
	char buffer[200];
	float px,py;
	int i=0;
	if(debug_mode!=DEBUG_SOCKET) return;
	s=recv(debug_socket,buffer,200,0);
	if(s>0) {
		//sprintf(tstring,"recibed[%d][%d]:",s,strncmp(buffer,"set pos ",strlen("set pos")));
		//debug(tstring);
		//debug(buffer);
		//set pilota posiciox posicioy vx vy 
		
		if(sscanf(buffer,"set pos %d %f %f",&i,&px,&py)) {
			//sscanf(buffer,"set pos %d %f %f",&i,&px,&py);
			game.element[i].x=px;
			game.element[i].y=py;
			debug("Modificant posicio\n");
		}
		//if(0==strncmp(tstring,"set vel ",strlen("set vel "))) {
		if(sscanf(buffer,"set vel %d %f %f\n",&i,&px,&py)) {
			//sscanf(tstring,"set vel %d %f %f",&i,&px,&py);
			game.element[i].vx=px;
			game.element[i].vy=py;
			debug("Modificant velocitat\n");
		}
		//if(0==strncmp(tstring,"set mass ",strlen("set mass "))) {
		if(sscanf(buffer,"set mass %d %f\n",&i,&px)){
			game.element[i].massa=px;
			debug("Modificant massa\n");
		}
		if(0==strncmp(buffer,"list objects ",strlen("list objects"))){
			debug("#	X	Y	VX	VY	Mass	Diam\n");
	
			for(i=0;i<game.players[0].elements+game.players[1].elements+game.neutral_elements;i++) {
				sprintf(tstring,"%8d %8.2f %8.2f %8.2f %8.2f %8d %8d\n", i, game.element[i].x, game.element[i].y, game.element[i].vx, game.element[i].vy, game.element[i].massa ,game.element[i].radi);
				debug(tstring);
			}	
		}
		if(0==strncmp(buffer,"unpause ",strlen("unpause"))){
			pause_collision=false;
		}
		if(0==strncmp(buffer,"debug collision on ",strlen("debug collision on"))){
			debug_collision=1;
		}
		if(0==strncmp(buffer,"debug collision off ",strlen("debug collision off"))){
			debug_collision=0;
		}
	}
}
