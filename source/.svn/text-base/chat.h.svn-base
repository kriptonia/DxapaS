#include <PA9.h>

#define MAX_LEN_CHAT	32
#define LEN_NICKNAME	5

/**
Inicialitza el xat
Es conecta al servidor
i envia petició per rebre els nous missatges
*/
u8 chat_init(void);

/**
Envia un missatge al xat
*/
void chat_send(char *message);

/**
Processa els missatges del servidor i demana rebre els nous missatge
*/
void chat_update(void);

/**
Informa al servidor que hi ha has creat una partida
*/
void chat_newgame(void);

/**
Desconecta del xat
*/
void chat_quit(void);

extern bool chat_ready;

/**
Indiquem cada quants segons volem comprovar els nous missatges
*/
extern u8 chat_refresh;
