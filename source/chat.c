#include "chat.h"
#include "global.h"
#include "debug.h"
#include "gui.h"

int sock_chat;
bool chat_ready=false;

char message[MAX_LEN_CHAT+2];
u8 chat_refresh;


/**
Demana al servidor si hi han missatges
*/
void chat_request_update(void) {
        char buffer[MAX_LEN_CHAT+2];
        if(chat_ready==1) {
                snprintf(buffer,MAX_LEN_CHAT,"G\n");
                send(sock_chat,buffer,strlen(buffer),0);
        }
}
/**
Inicialitza el xat
Es conecta al servidor
i envia petició per rebre els nous missatges
*/
u8 chat_init(void) {
	char buffer[MAX_LEN_CHAT+10];
	char nickname[30];
	strcpy(nickname,(char *)&PA_UserInfo.Name);
	if(PA_InitSocket(&sock_chat,CHAT_SERVER,CHAT_PORT,PA_NORMAL_TCP)) {//PA_NONBLOCKING_TCP, PA_NORMAL_TCP
		chat_ready=true;
		
		sprintf(buffer,"L %s\n",nickname);
		send(sock_chat,buffer,strlen(buffer),0); //Logged
		chat_refresh=CHAT_REFRESH;
		chat_request_update();
		debug("[chat_init] ok\n");
		return 1;
	}
	debug("[chat_init] fail\n");
	return 0;
}

/**
Envia un missatge al xat
*/
void chat_send(char *message){
	char buffer[MAX_LEN_CHAT+10];
	if(chat_ready==1) {
		debug("[chat_send] Sending message\n");
		sprintf(buffer,"S %s\n",message);
		send(sock_chat,buffer,strlen(buffer),0);
	}
}

/**
Informa al servidor que hi ha has creat una partida
*/
void chat_newgame(void) {
	char buffer[MAX_LEN_CHAT+10];
	if(chat_ready==1) {
		debug("[chat_newgame]\n");
		sprintf(buffer,"M %s\n",ip);
		send(sock_chat,buffer,strlen(buffer),0);
	}
}

/**
Processa els missatges del servidor i demana rebre els nous missatge
*/
void chat_update(void) {
	char buffer[MAX_LEN_CHAT+2];
	char lip[20];
	char lnick[20];
	s16 n;
	s32 nmessages;
	//albert. si es s32 no da warning, s32 = int -> %d l46
	if(chat_ready==1) {
		debug("[chat_update] Checking for new messages\n");
		recv(sock_chat,&buffer,MAX_LEN_CHAT,0);
		//PA_OutputSimpleText(TOP_SCREEN,0,18,buffer);
		if(buffer[0]=='R') {
			sscanf(buffer,"R %d",&nmessages);
			while(nmessages>0) {
				n=recv(sock_chat,&buffer,MAX_LEN_CHAT,0);
				if(n>0) {
					buffer[n]=0;
					PA_Print(TOP_SCREEN,buffer);
				}
				if(sscanf(buffer,"-match %s %s\n",lip,lnick)) {
					if(strcmp(lip,ip)!=0) {
						last_game=true;
						strcpy(last_game_ip,lip);
						strcpy(last_game_nick,lnick);
					}
				}
				nmessages--;
			}
		}
		chat_request_update();
	}
}

/**
Desconecta del xat
*/
void chat_quit(void) {
	closesocket(sock_chat);
	chat_ready=false;
	debug("[chat_quit] Chat disconected\n");
}
