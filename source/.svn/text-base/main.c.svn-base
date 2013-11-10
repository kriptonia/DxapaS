
#include <PA9.h>
#include "types.h"
#include "chat.h"
#include "menu.h"
#include <math.h>
#include "libsock.h"
#include "gui.h"
#include "global.h"
#include "debug.h"
#include "game.h"

#include "gfx/all_gfx.c"

u8 game_mode;
tgame game;
tgame game_settings;
int listen_sock;
int game_sock;
u32 frameCount;

u8 debug_mode;
u8 debug_collision;
u8 debug_chat;
u8 debug_socket;
char tstring[200]; //String for a temporal use
char ip[16];
bool wifi=0;
bool pause_collision=false;
bool pause;
u16 *pal_bottom;
bool last_game=false;
char last_game_ip[20];
char last_game_nick[20];

int main(void)
{
	pal_bottom = (u16*)PAL_BG0;
	PA_Init();
	PA_InitVBL();
	debug_init(NO_DEBUG);
	
	PA_SplashEyes();
	PA_Init8bitBg(BOTTOM_SCREEN, 0);
	PA_EasyBgLoad(TOP_SCREEN,3,grass);
	PAPI_Init(BOTTOM_SCREEN); //Init the api of PA (Buttons, checkbox, and others)
	PA_InitText(TOP_SCREEN, BG_TEXT);
 	PA_InitText(BOTTOM_SCREEN, BG_TEXT);
	PA_InitKeyboard(BG_KB);
	//PA_KeyboardOut();
	frameCount=0;
	config_game_defaults(&game_settings);
	credits();
	load_menu();
	while(1) //there will check end of the match
	{
		PAPI_CheckButton();
		WaitVBL();
	}
	
	return 0;
}


