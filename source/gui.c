#include "global.h"
#include "chat.h"
#include "game.h"
#include "debug.h"

/**
Carrega el menú principal
*/
void load_menu(void);

/**
Descarrega el menu
*/
void unload_menu(void);

/**
	Show an error on top screen
*/
void error(char *s) {
	if(debug_mode==DEBUG_SOCKET) {
		debug("Error : ");
		debug(s);
	}
	PA_SetTextTileCol(TOP_SCREEN,RED);
	PA_Print(TOP_SCREEN,s);
	PA_SetTextTileCol(TOP_SCREEN,NORMAL);
}

/**
	Show a warning on top screen
*/
void warning(char *s) {
	if(debug_mode==DEBUG_SOCKET) {
		debug("Warning: ");
		debug(s);
	}
	PA_SetTextTileCol(TOP_SCREEN,BLUE);
	PA_Print(TOP_SCREEN,s);
	PA_SetTextTileCol(TOP_SCREEN,NORMAL);
}

/**
	Notify to user on top screen
*/
void notify(char *s) {
	if(debug_mode==DEBUG_SOCKET) {
		debug("Notify: ");
		debug(s);
	}
	PA_SetTextTileCol(TOP_SCREEN,CIAN);
	PA_Print(TOP_SCREEN,s);
	PA_SetTextTileCol(TOP_SCREEN,NORMAL);
}

/**
	Show a toltip or help message on top screen
*/
void help(char *s) {
	PA_SetTextTileCol(TOP_SCREEN,GREEN);
	PA_Print(TOP_SCREEN,s);
	PA_SetTextTileCol(TOP_SCREEN,NORMAL);
}
void credits(void) {
	PA_ClearTextBg(TOP_SCREEN);
	PA_SetTextTileCol(TOP_SCREEN,BLUE);
	PA_Print(TOP_SCREEN,"DxapaS Game\n\n");
	PA_SetTextTileCol(TOP_SCREEN,NORMAL);
	PA_Print(TOP_SCREEN,"Autors:\n - Albert Solà Fabà\n - Daniela Vicente Valverde\n\n");
	PA_SetTextTileCol(TOP_SCREEN,RED);
	PA_Print(TOP_SCREEN,"Directora:\n - Dolors Puiganer Riva\n");
	PA_SetTextTileCol(TOP_SCREEN,GREEN);
	PA_Print(TOP_SCREEN,"\nProjecte de final de carrera\nUniversitat Rovira i Virgili\nDepartamant d'Enginyeria Informï¿½tica i Matemï¿½tica\nEng. Inormï¿½tica de Sistemes\nAny 2008\n\n");
}

/**
	Wait for vertical sync
	Check chat socket
	Check debug console socket
*/
void WaitVBL(void) {
	if(frameCount%(FRAMERATE*chat_refresh)==0) {
		chat_update();
	} 
	else if(frameCount%(FRAMERATE*2)==0) {
		debug_read();
	}
	frameCount++;
	
	PA_WaitForVBL();
}

/**
	Check if keyboard is pressed and read
*/
int read_keyboard(text *s) {
	char letter;
	letter=PA_CheckKeyboard();

	if(letter>31 && s->len<s->max) {
		s->string[s->len] = letter;
		s->len++;
	} else if ((letter == PA_BACKSPACE && s->len>0)) { // Backspace pressed
		s->len--;
		s->string[s->len]=0; // Erase the last letter
	} else if (letter == '\n'){ // Enter pressed
		return s->len;
	}
	return -1;
}

/**
Load palette and background
*/
void loadMenuBg(void) {
	PA_LoadPal(PAL_BG0, menu_Pal);
	PA_Load8bitBitmap(BOTTOM_SCREEN, menu_Bitmap);
	
}
void loadKbBg(void) {
	PA_LoadPal(PAL_BG0, keyboardbg_Pal);
	PA_Load8bitBitmap(BOTTOM_SCREEN, keyboardbg_Bitmap);
	
}
void loadTopBg(void) {
	PA_LoadPal(PAL_BG1, grass_Pal);
	PA_EasyBgLoad(TOP_SCREEN,BG_PIC,grass);
}

/**
	Read an string from keyboard
*/
void getString(char *info, char *string, int maxlen) {
	//Mostra teclat
	text s;
	//loadBottomBg(); //Comentar esto para que no se cuelgue
	s.max=maxlen;
	s.len=0;
	s.string=string;
	memset(s.string, 0, maxlen);
	loadKbBg();
	PA_SetBgPrio(BOTTOM_SCREEN,BG_KB,0);
	PA_ReloadKeyboardCol();
	PA_InitText(BOTTOM_SCREEN, BG_TEXT);
	PA_SetTextTileCol(BOTTOM_SCREEN,BLUE);
	PA_OutputSimpleText(BOTTOM_SCREEN,0,0,info);
	PA_KeyboardIn(20, 100);
	PA_InitTextBorders(BOTTOM_SCREEN,1,5,30,10);
	while(read_keyboard(&s)<0) {
		PA_EraseTextBox(BOTTOM_SCREEN);
		PA_SimpleBoxText(BOTTOM_SCREEN,s.string,s.max);
		//PA_Print(BOTTOM_SCREEN,s.string);
		WaitVBL();
		//PA_WaitForVBL();
	}
	PA_EraseTextBox(BOTTOM_SCREEN);
	PA_ClearTextBg(BOTTOM_SCREEN);
	PA_SetTextTileCol(BOTTOM_SCREEN,NORMAL);
	PA_KeyboardOut();
}

void sendMessage(void) {
	char text[60];
	unload_menu();
	getString("Escriu un missatge al xat",text,24);
	if(strlen(text)>0) {
		chat_send(text);
	}
	load_menu();
}

void clearScreen(void) {
	PA_ClearTextBg(TOP_SCREEN);
}

void showIp(void) {
	sprintf(tstring,"La teva ip es: %s\n",ip);
	notify(tstring);
}

void startWifi(void) {
	long int lIP;
	PA_InitWifi(); //Initializes the wifi
	notify("Intentant conectar per wifi\n\n");
	notify("Per a conectar has de tenir preconfigurada la xarxa a la consola.\n");
	if(PA_ConnectWifiWFC()) {
		PA_ClearTextBg(TOP_SCREEN);
		notify("Conectat a la wifi\n");
		lIP=Wifi_GetIP();
		sprintf(ip,"%i.%i.%i.%i",(int)((lIP)&255),(int)((lIP>>8)&255),(int)((lIP>>16)&255),(int)((lIP>>24)&255));
		showIp();
 		wifi=true;
		unload_menu();
		load_menu();
		if(chat_init()) {
			help("Estas conectat al xat\n");
		} else {
			error("Error al conectar al chat. Estas conectat a la wifi?\n");
		}
	} else {
		wifi=false;
		error("Error al conectar a la wifi\n");
	}
	
}

void createGame(void) {
	unload_menu();
	chat_newgame();
	if(init_game(MODE_SERVER,"0.0.0.0")) {
		match();
	}
	end_game();
	load_menu();
}

void connectGame(void) {
	char hostname[100];
	unload_menu();
	getString("Entra el hostname del contrincant",hostname, 100);
	if(init_game(MODE_CLIENT,hostname)) {
		match();
	}
	end_game();
	load_menu();
}

void connectLastGame(void) {
	unload_menu();
	if(last_game && init_game(MODE_CLIENT,last_game_ip)) {
		match();
	} else {
		error("Ultima partida no disponible\n");
	}
	end_game();
	load_menu();
}
void offlineGame(void) {
	unload_menu();
	init_game(MODE_OFFLINE,"0.0.0.0");
	match();
	end_game();
	load_menu();
}
void debugOnline(void) {
	debug_init(DEBUG_SOCKET);
}
void debugDisplay(void) {
	debug_init(DEBUG_DISPLAY);
}

void config_game(void) {
	int neutral=0;
	int playerElem=0;
	float friction=0.0;
	int frictionReal=0;
	int maxScore=0;
	unload_menu();
	
	getString("Quantes pilotes vols? [1..5]",tstring,1);
	sscanf(tstring,"%d",&neutral);
	if(neutral>5 || neutral<1) {
		neutral=BALL_NUMBER;
	}
	getString("Quantes xapes? [1..5]",tstring,1);
	sscanf(tstring,"%d",&playerElem);
	if(playerElem>5 || playerElem<1) {
		playerElem=PLAYER1_NUMBER;
	}
	getString("Guanya el que marca [1..9] gols",tstring,1);
	sscanf(tstring,"%d",&maxScore);
	if(maxScore>9 || maxScore<1) {
		maxScore=MAX_SCORE;
	}
	getString("Fricciï¿½? [0..100]\n Amb fricciï¿½ 0, les xapes no es paren mai",tstring,3);
	sscanf(tstring,"%d",&frictionReal);
	friction=(100-frictionReal)/100.0;
	if(friction>1 || friction<0) {
		friction=FRICTION;
	}
	
	setGame(&game_settings,neutral, playerElem,friction,maxScore);
	
	load_menu();
}

void load_menu(void) {
	loadMenuBg();
	PAPI_ReInit(BOTTOM_SCREEN);
	
	PAPI_CreateButton(BOTTOM_SCREEN,65,61,120,16,&offlineGame,"1 VS 1",3,1);
	PAPI_CreateButton(BOTTOM_SCREEN,0,82,60,16,&clearScreen,"Neteja",3,1);
	PAPI_CreateButton(BOTTOM_SCREEN,0,100,60,16,&credits,"Credits",3,1);
	
	PAPI_CreateButton(BOTTOM_SCREEN,0,140,60,16,&debugDisplay,"Debug Display",3,1);
	PAPI_CreateButton(BOTTOM_SCREEN,190,160,60,16,&config_game,"Config",3,1);
	PAPI_CreateButton(BOTTOM_SCREEN,65,82,120,16,&sendMessage,"Escriu missatge",3,2);
	if(wifi==true) {
		PAPI_CreateButton(BOTTOM_SCREEN,65,82,120,16,&sendMessage,"Escriu missatge",3,2);
		PAPI_CreateButton(BOTTOM_SCREEN,65,100,120,16,&createGame,"Crea partida",3,2);
		PAPI_CreateButton(BOTTOM_SCREEN,65,120,120,16,&connectGame,"Conecta partida",3,2);
		PAPI_CreateButton(BOTTOM_SCREEN,65,140,120,16,&connectLastGame,"Conecta a l'ultim",3,2);
		PAPI_CreateButton(BOTTOM_SCREEN,0,120,60,16,&debugOnline,"Debug Online",3,1);
		PAPI_CreateButton(BOTTOM_SCREEN,0,160,60,16,&showIp,"IP?",3,1);
	} else {
		PAPI_CreateButton(BOTTOM_SCREEN,0,61,60,16,&startWifi,"Activa Wifi",3,1);
	}
}

void unload_menu(void) {
	PAPI_ReInit(BOTTOM_SCREEN);
	loadMenuBg();
}



