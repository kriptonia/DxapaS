
/**
El sistema pot esta en 4 modalitats
*/
#define MODE_SERVER 1
#define MODE_CLIENT 2
#define MODE_OFFLINE 3
#define MODE_NOGAME 4

/**
Colors per a la sortida de text
*/
#define NORMAL 0
#define RED 1	//Error color
#define GREEN 2	//Help
#define BLUE 3  
#define PINK 4	//Debug color
#define CIAN 5	//Notify
#define YELLOW 6 //Warning

/**
Enumeració de pantalles
*/
#define TOP_SCREEN 1
#define BOTTOM_SCREEN 0

/**
Gestió de fons de pantalla
*/
#define BG_TEXT 0
#define BG_KB 2
#define BG_PIC 1
#define BG_STYLUS 3

/**
Configuració de paletas de colors
*/
#define PAL_PLAYER1 0
#define PAL_PLAYER2 1
#define PAL_BALL 3
#define PAL_MARCADOR 4

#define REALFISICS //Define this for real fisics, if not define fisics are not real (arcade)

/**
Configuració del xat
*/
#define CHAT_SERVER	"nemezis.dsland.org" //"192.168.100.2"
#define CHAT_PORT	1025
#define	CHAT_REFRESH_GAME	4
#define CHAT_REFRESH	2

/**
Parametres per defecte del joc
*/
#define GOALS 2
#define PLAYERS 2
#define PLAYER1_NUMBER	3
#define PLAYER2_NUMBER	3
#define BALL_NUMBER	1

#define MAX_SCORE	3
#define FRICTION	0.99
#define MARCADOR_SPRITE	100

#define V_DIVIDE	70
#define V_DRAW		40
#define V_MAX		10
#define OBJECT_STOPED	0.1

#define	FRAMERATE	60


#define OUT_X		300
#define OUT_Y		300


/**
Modes del debug
*/
#define DEBUG_SOCKET	1
#define DEBUG_DISPLAY	2
#define NO_DEBUG	0

/**
S'utilitza per definir els sockets no bloquejants
*/
#define DONTWAIT	1

#include <PA9.h>
#include "types.h"
#include "physics.h"

/**
Variables globals del joc
*/
extern u8 game_mode;
extern int listen_sock;
extern int game_sock;
extern u32 frameCount;

extern u8 debug_mode;
extern u8 debug_collision;
extern u8 debug_chat;
extern u8 debug_socket;
extern char tstring[200];
extern char ip[16];
extern bool wifi;
extern bool chat_ready;
extern bool pause_collision;
extern bool pause;
extern tgame game;
extern tgame game_settings;
extern u16 *pal_bottom;

extern bool last_game;
extern char last_game_ip[20];
extern char last_game_nick[20];

#ifndef GFX
#define GFX
#include "gfx/all_gfx.h"
#endif

