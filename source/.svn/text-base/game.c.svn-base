#include <PA9.h>
#include "global.h"
#include "libsock.h"
#include "chat.h"
#include "gui.h"
#include <math.h>
#include "debug.h"
#include "game.h"
void game_menu(void);

/**
1) Realitza el sincronisme vertical
2) Comprova si es demana el teclat 
3) Actulitza el debug 
*/
void wait_game(void) {
	if(Pad.Newpress.R 
		&& chat_ready==1
	) {
		hide_sprites();
		getString("Escriu un missatge al xat",tstring,24);
		if(strlen(tstring)>0) {
			chat_send(tstring);
		}
		update_gfx(&game);
		draw_pitch();
	}
	WaitVBL();
}

bool goal_area(round_object *a, straight *line){
	if ( line->x2==line->x1 ){
		if ((a->y < line->y2) && (a->y > line->y1)) {
			return true;			
		}
	}else if ( line->y2==line->y1 ){
		if ((a->x < line->x2) && (a->x > line->x1)){
			return true;	
		}	
	}else{
		if ((a->y < line->y2) && (a->y > line->y1) && (a->x < line->x2) && (a->x > line->x1)){
 			return true;
		}
	}
	return false;
}
void draw_pitch(void) {
	PA_LoadPal(PAL_BG0, campo_Pal);
	PA_Load8bitBitmap(BOTTOM_SCREEN, campo_Bitmap);
}

void init_pitch(tgame *g) {

	g->pitch.padding=10;
//Goal
	g->pitch.goals[0].line.x1=15+1;
	g->pitch.goals[0].line.y1=100;
	g->pitch.goals[0].line.x2=15+1;
	g->pitch.goals[0].line.y2=130;
	g->pitch.goals[0].team=1;
	g->pitch.goals[1].line.x1=240-1;
	g->pitch.goals[1].line.y1=100;
	g->pitch.goals[1].line.x2=240-1;
	g->pitch.goals[1].line.y2=130;
	g->pitch.goals[1].team=0;
	g->pitch.upLeft_x=15;
	g->pitch.upLeft_y=50;
	g->pitch.downLeft_x=15;
	g->pitch.downLeft_y=180;
	g->pitch.upRight_x=240;
	g->pitch.upRight_y=50;
	g->pitch.downRight_x=240;
	g->pitch.downRight_y=180;

	draw_pitch();
	sprintf(tstring,"Porteria 1: x:%d,y:%d x:%d,y%d\n",g->pitch.goals[0].line.x1,g->pitch.goals[0].line.y1,g->pitch.goals[0].line.x2,g->pitch.goals[0].line.y2);
	debug(tstring);
	
	sprintf(tstring,"Porteria 2: x:%d,y:%d x:%d,y%d\n",g->pitch.goals[1].line.x1,g->pitch.goals[1].line.y1,g->pitch.goals[1].line.x2,g->pitch.goals[1].line.y2);
	debug(tstring);

}

/**
Posa els valors al joc
*/
void setGame(tgame *g,u8 neutral, u8 playerElem,float friction,u8 maxScore) {
	g->neutral_elements=neutral;
	g->friction=friction;
	g->players[0].elements=playerElem;
	g->players[1].elements=playerElem;
	g->max_score=maxScore;
	
	init_pitch(g);
}

void hide_sprites(void) {
	u32 i;
	for (i = 0; i < game.players[0].elements+game.players[1].elements+game.neutral_elements; i++){
		PA_SetSpriteXY(BOTTOM_SCREEN,game.element[i].sprite_id, OUT_X, OUT_Y);
	}
	PA_SetSpriteXY(BOTTOM_SCREEN,MARCADOR_SPRITE,OUT_X,OUT_Y);
	PA_SetSpriteXY(BOTTOM_SCREEN,MARCADOR_SPRITE+1,OUT_X,OUT_Y);
}

/**
 Sincronitza el servidor i el client
 - mode 0: Sincronització completa
 - mode 1: Sincronització parcial
   En aquest mode envies les dades dels teus jugadors a l'oponent
*/
void game_sync(tgame *g, u8 type_sync) {
	
	switch(type_sync) {
	case 0:
		//Sincronitzem tot el joc
		if(game_mode==MODE_CLIENT) {
			debug("[game_sync] t=0 Sincronitzant (client)\n");
			while(recv(game_sock,g,sizeof(game),0)==-1) {
				wait_game();
			}
		} 
		if(game_mode==MODE_SERVER) {
			debug("[game_sync] t=0 Sincronitzant (server)\n");
			send(game_sock,g,sizeof(game),0);
		}
		break;
	
	case 1:
		//Sincronitzem els jugadors del contrari (init_round)
		if(game_mode==MODE_SERVER) {
			debug("[game_sync] t=1 Sincronitzant (server)\n");
			send(game_sock,&g->element[0],sizeof(round_object)*g->players[0].elements,0);
			while(recv(game_sock,&g->element[g->players[0].elements],sizeof(round_object)*g->players[1].elements,0)==-1) {
				wait_game();
			}
		} else if(game_mode==MODE_CLIENT) {
			debug("[game_sync] t=1 Sincronitzant (client)\n");
		//	send(game_sock,&g->element[PLAYER1_NUMBER],sizeof(round_object)*PLAYER2_NUMBER,0);
			send(game_sock,&g->element[g->players[0].elements],sizeof(round_object)*g->players[1].elements,0);
			//while(recv(game_sock,&g->element[0],sizeof(round_object)*PLAYER1_NUMBER,0)==-1) {
			while(recv(game_sock,&g->element[0],sizeof(round_object)*g->players[0].elements,0)==-1) {
				wait_game();
			}
		}
		break;
	}
}

void reset_players(tgame *g){
	s32 i;
	debug("[reset_players] inicialitzant jugadors\n");
	for(i=0;i<g->players[0].elements;i++)  {
		g->element[i].x = (g->pitch.downRight_x-g->pitch.downLeft_x)/4-1+g->pitch.downLeft_x; 
		g->element[i].y = (g->pitch.downLeft_y-g->pitch.upLeft_y)/(g->players[0].elements+1)*(i+1)-1+g->pitch.upLeft_y;
		g->element[i].vx = 0.0;
		g->element[i].vy = 0.0;
		g->element[i].angle = 0;	
		//PA_SetSpriteXY(BOTTOM_SCREEN,g->element[i].sprite_id, g->element[i].x-g->element[i].sprite_size/2, g->element[i].y-g->element[i].sprite_size/2);
	}
	for(i=g->players[0].elements;i<g->players[0].elements+g->players[1].elements;i++)  {
		g->element[i].x = (g->pitch.downRight_x-g->pitch.downLeft_x)/4*3-1+g->pitch.downLeft_x;
		g->element[i].y = (g->pitch.downLeft_y-g->pitch.upLeft_y)/(g->players[1].elements+1)*(i-g->players[1].elements+1)-1+g->pitch.upLeft_y;
		g->element[i].vx = 0.0;
		g->element[i].vy = 0.0;	
		g->element[i].angle = 0;	
	//	PA_SetSpriteXY(BOTTOM_SCREEN,g->element[i].sprite_id, g->element[i].x-g->element[i].sprite_size/2, g->element[i].y-g->element[i].sprite_size/2);
	}
	for (i = g->players[0].elements+g->players[1].elements; i < g->players[0].elements+g->players[1].elements+g->neutral_elements; i++){
		g->element[i].x = (g->pitch.downRight_x-g->pitch.downLeft_x)/2-1+g->pitch.downLeft_x;
		g->element[i].y= (g->pitch.downLeft_y-g->pitch.upLeft_y)/(g->neutral_elements+1)*(i-g->players[0].elements-g->players[1].elements+1)-1+g->pitch.upLeft_y;
		g->element[i].vx = 0.0;
		g->element[i].vy = 0.0;
		g->element[i].angle = 0;	
		//PA_SetSpriteXY(BOTTOM_SCREEN,g->element[i].sprite_id, g->element[i].x-g->element[i].sprite_size/2, g->element[i].y-g->element[i].sprite_size/2);
	}
	g->neutral_alive=g->neutral_elements;

}

/* Funcion perfectamente documentada por daniela Pues si que pasa!!! */
s8 match_point(round_object *a, tgame *g){	
	//only if it is a ball.
	if (a->sprite_id>=g->players[0].elements+g->players[1].elements && a->sprite_id<g->players[0].elements+g->players[1].elements+g->neutral_elements){
		if ((a->radi/2>get_distance_object_line(a,&g->pitch.goals[0].line))&&(goal_area(a,&g->pitch.goals[0].line))){
			g->players[g->pitch.goals[0].team].score++;
			PA_SetSpriteAnim(BOTTOM_SCREEN,MARCADOR_SPRITE+g->pitch.goals[0].team, g->players[g->pitch.goals[0].team].score);
			notify("GOOOOL!!!");
			a->x=OUT_X;
			a->y=OUT_Y;
			a->vx=0;
			a->vy=0;
			g->neutral_alive--;
			debug("[match_point] gol 0\n");
			return g->pitch.goals[0].team;
		}
		if ((a->radi/2>get_distance_object_line(a,&g->pitch.goals[1].line))&&(goal_area(a,&g->pitch.goals[1].line))){
			g->players[g->pitch.goals[1].team].score++;
			PA_SetSpriteAnim(BOTTOM_SCREEN,MARCADOR_SPRITE+g->pitch.goals[1].team, g->players[g->pitch.goals[1].team].score);
			notify("GOOOOL!!!");
			a->x=OUT_X;
			a->y=OUT_Y;
			a->vx=0;
			a->vy=0;
			g->neutral_alive--;
			debug("[match_point] gol 1\n");
			return g->pitch.goals[1].team;
		}
	}
	return -1;
}

void cancel(void) {
	game_sock=-2;
}

void messageGame(void) {
	getString("Escriu un missatge al xat",tstring,24);
	if(strlen(tstring)>0) {
		chat_send(tstring);
	}
	game_menu();
}
void game_menu(void) {
	loadMenuBg();
	PAPI_ReInit(BOTTOM_SCREEN);
	PAPI_CreateButton(BOTTOM_SCREEN,190,160,60,16,&cancel,"Cancel.la",3,1);
	PAPI_CreateButton(BOTTOM_SCREEN,65,82,120,16,&messageGame,"Escriu missatge",3,2);
}

/**
Inicialitza el videojoc
 - Crea els sockets
 - Es conecta o espera les conexions
   El tipus d'inicialització es:
	MODE_OFFLINE
	MODE_CLIENT
	MODE_SERVER
*/
u8 init_game(u8 type,char *ip) {
	debug("[init_game]\n");
	game_mode=type;
	chat_refresh=CHAT_REFRESH_GAME;
	if(type==MODE_SERVER) {
		listen_sock=listensock_nowait(7772, 1);
		if(listen_sock==-1) {
			error("Error de inicializacion ...\n");
			return 0;
		} else {
			notify("Esperant els altres jugadors\n");
		}

		game_sock=acceptsock(listen_sock);
		game_menu();
		//Waiting for clients
		while(game_sock==-1) {
			WaitVBL();
			game_sock=acceptsock(listen_sock);
			PAPI_CheckButton();
		}
		if(game_sock==-2) return 0; //S'ha cancelat la partida
		PAPI_ReInit(BOTTOM_SCREEN);
		notify("Player conected ...\n");
		notify("Conectat!\n");
		closesocket(listen_sock);
	} else if(type==MODE_CLIENT) {
		if(PA_InitSocket(&game_sock,ip,7772,PA_NORMAL_TCP)) {
			notify("Connected ... Start game!\n");
		} else {
			error("Error can connect\n");
			return 0;
		}
		
	} else if(type==MODE_OFFLINE) {
		return 0;
	}
	return 1;
}

void init_players(tgame *g) {
	s32 i;
	debug("[init_players]\n");
	//Player one
	for(i=0;i<g->players[0].elements;i++)  {
		g->element[i].x = (g->pitch.downRight_x-g->pitch.downLeft_x)/4-1+g->pitch.downLeft_x; 
		g->element[i].y = (g->pitch.downLeft_y-g->pitch.upLeft_y)/(g->players[0].elements+1)*(i+1)-1+g->pitch.upLeft_y;
		// Speed of g->element in both ways
		g->element[i].vx = 0.0;
		g->element[i].vy = 0.0;
		g->element[i].angle = 0;	
		g->element[i].radi=16;
		g->element[i].sprite_id=i;
		g->element[i].sprite_size=16;
		g->element[i].massa=g->element[i].radi;
	}
	
	//Player Two players
	for(i=g->players[0].elements;i<g->players[0].elements+g->players[1].elements;i++)  {
		g->element[i].x = (g->pitch.downRight_x-g->pitch.downLeft_x)/4*3-1+g->pitch.downLeft_x;
		g->element[i].y = (g->pitch.downLeft_y-g->pitch.upLeft_y)/(g->players[1].elements+1)*(i-g->players[1].elements+1)-1+g->pitch.upLeft_y;
		// Speed of g->element in both ways
		g->element[i].vx = 0.0;
		g->element[i].vy = 0.0;	
		g->element[i].angle = 0;	
		g->element[i].radi=16;
		g->element[i].sprite_id=i;
		g->element[i].sprite_size=16;
		g->element[i].massa=g->element[i].radi;
	}
	
	for (i = g->players[0].elements+g->players[1].elements; i < g->players[0].elements+g->players[1].elements+g->neutral_elements; i++){
		g->element[i].x = (g->pitch.downRight_x-g->pitch.downLeft_x)/2-1+g->pitch.downLeft_x;
		g->element[i].y= (g->pitch.downLeft_y-g->pitch.upLeft_y)/(g->neutral_elements+1)*(i-g->players[0].elements-g->players[1].elements+1)-1+g->pitch.upLeft_y;


		// Speed of g->element in both ways		
		g->element[i].vx = 0.0;
		g->element[i].vy = 0.0;
		g->element[i].angle = 0;	
		g->element[i].radi=10;
		g->element[i].sprite_id=i;
		g->element[i].sprite_size=16;
		g->element[i].massa=g->element[i].radi;
	}
	g->neutral_alive=g->neutral_elements;
}

void create_gfx(tgame *g) {
	u32 i;
	//Player one
	debug("[create_gfx]\n");
	for(i=0;i<g->players[0].elements;i++)  {
		PA_CreateSprite(BOTTOM_SCREEN,g->element[i].sprite_id, (void*)jugador_verd_Sprite, OBJ_SIZE_16X16, 1, PAL_PLAYER1,g->element[i].x-g->element[i].sprite_size/2, g->element[i].y-g->element[i].sprite_size/2);
		debug("Creant xapa\n");
	}
	
	//Player Two players
	for(i=g->players[0].elements;i<g->players[0].elements+g->players[1].elements;i++)  {
		PA_CreateSprite(BOTTOM_SCREEN,g->element[i].sprite_id, (void*)jugador_vermell_Sprite, OBJ_SIZE_16X16, 1, PAL_PLAYER2, g->element[i].x-g->element[i].sprite_size/2, g->element[i].y-g->element[i].sprite_size/2);
		debug("Creant xapa\n");
	}
	
	for (i = g->players[0].elements+g->players[1].elements; i < g->players[0].elements+g->players[1].elements+g->neutral_elements; i++){
		PA_CreateSprite(BOTTOM_SCREEN,g->element[i].sprite_id, (void*)bola_Sprite, OBJ_SIZE_16X16, 1, PAL_BALL, g->element[i].x-g->element[i].sprite_size/2, g->element[i].y-g->element[i].sprite_size/2); 
		debug("Creant xapa\n");
	}

	PA_CreateSprite(BOTTOM_SCREEN,MARCADOR_SPRITE, (void*)marcador_Sprite, OBJ_SIZE_32X32, 1, PAL_MARCADOR,16,4);
	PA_CreateSprite(BOTTOM_SCREEN,MARCADOR_SPRITE+1, (void*)marcador_Sprite, OBJ_SIZE_32X32, 1, PAL_MARCADOR,208,4);
}

void update_gfx(tgame *g) {
	debug("[update_gfx]\n");
	u32 i;
	for (i = 0; i < g->players[0].elements+g->players[1].elements+g->neutral_elements; i++){
		PA_SetSpriteXY(BOTTOM_SCREEN,g->element[i].sprite_id, g->element[i].x-g->element[i].sprite_size/2, g->element[i].y-g->element[i].sprite_size/2);
	}
		
	PA_SetSpriteXY(BOTTOM_SCREEN,MARCADOR_SPRITE,16,4);
	PA_SetSpriteXY(BOTTOM_SCREEN,MARCADOR_SPRITE+1,208,4);

	PA_SetSpriteAnim(BOTTOM_SCREEN,MARCADOR_SPRITE+g->pitch.goals[0].team, g->players[g->pitch.goals[0].team].score);
	PA_SetSpriteAnim(BOTTOM_SCREEN,MARCADOR_SPRITE+g->pitch.goals[1].team, g->players[g->pitch.goals[1].team].score);
}

/**
Prepara un partit
 - Carrega les paletes de colors
 - Dibuixa el fons de pantalla
 - Carrega la configuració del joc
 - Inicialitza les puntuacions
 - Posa els jugadors a la seva posició
 - Es sincronitza amb l'oponent
*/
void init_match(tgame *g) {
	s32 i; // will be used in for loops to cycle through the g->elements...
	debug("[update_gfx]\n");

	// Load the palettes for the sprites on both screens
	PA_LoadSpritePal(BOTTOM_SCREEN,PAL_PLAYER1, (void*)jugador_verd_Pal);	
	PA_LoadSpritePal(BOTTOM_SCREEN,PAL_PLAYER2, (void*)jugador_vermell_Pal);
	PA_LoadSpritePal(BOTTOM_SCREEN,PAL_BALL, (void*)bola_Pal);	
	PA_LoadSpritePal(BOTTOM_SCREEN,PAL_MARCADOR, (void*)marcador_Pal);	

	//Load background
	draw_pitch();
	cpyGame(&game_settings,g);

	for (i=0;i<PLAYERS;i++) {
		g->players[i].score=0;
		g->players[i].score_sprite_id=MARCADOR_SPRITE+i;
	}
	init_players(g);
	game_sync(g,0); //sincronitzaciï¿½ completa





	create_gfx(g);
	update_gfx(g);
}

/**
 - Es llegeixen les noves velocitats de les xapes
 - Es sicronitza amb l'oponent
 */
void init_round(tgame *g) {
	//Llegeix forÃ§a i direcciÃ³ dels objectes (Posa velocitat)
	
	u16 i;
	s16 selected=-1;
	debug("[init_round]\n");
	if(game_mode!=MODE_OFFLINE) {
		help("Defineix la teva jugada i pitja A\n");
		help("Pitja R per escriure un missatge\n");
		if(game_mode==MODE_SERVER) {
			notify("Les teves xapes son verdes\n");
		}
		if(game_mode==MODE_CLIENT) {
			notify("Les teves xapes son vermelles\n");
		}
	} 
	if(game_mode==MODE_OFFLINE) {
		help("1) Defineix la teva jugada\n2) passa la consola al teu contrincant\n3)Quan estigueu pitjeu A\n");
	}
	while(!Pad.Newpress.A) {
		if(selected!=-1) {
			g->element[selected].vx=(Stylus.X-g->element[selected].x)/V_DIVIDE;
			g->element[selected].vy=(Stylus.Y-g->element[selected].y)/V_DIVIDE;
			if(g->element[selected].vx>V_MAX) {
				g->element[selected].vx=V_MAX;
			}
			if(g->element[selected].vx<-V_MAX) {
				g->element[selected].vx=-V_MAX;
			}
			if(g->element[selected].vy>V_MAX) {
				g->element[selected].vy=V_MAX;
			}
			if(g->element[selected].vy<-V_MAX) {
				g->element[selected].vy=-V_MAX;
			}
			draw_pitch();
			i=selected;

			PA_Draw8bitLineEx(BOTTOM_SCREEN, g->element[i].x, g->element[i].y,g->element[i].x+g->element[i].vx*V_DRAW,g->element[i].y+g->element[i].vy*V_DRAW, 1,1);
			//Quan deixem l'stylus deseleccionem el jugador i pintem totes les direccions
			if(Stylus.Released) {
				selected=-1; //Deseleccionem
				draw_pitch();
				//Si estem jugant offline com la consola es compartida amagem la tirada no mostran les direccions
				if(game_mode!=MODE_OFFLINE) {
					for (i = 0; i < g->players[0].elements+g->players[1].elements; i++){
						if(g->element[i].y>2) {
							PA_Draw8bitLineEx(BOTTOM_SCREEN, g->element[i].x, g->element[i].y,g->element[i].x+g->element[i].vx*V_DRAW,g->element[i].y+g->element[i].vy*V_DRAW, 1,1);
						}
					}
				}
				
			}
		} else {
			if(game_mode==MODE_SERVER) {
				//Movem els del jugador 1
				for (i = 0; i < g->players[0].elements; i++){
					if (PA_SpriteTouched(g->element[i].sprite_id) && Stylus.Newpress){
						selected=i;	
					}
				}
			} else if (game_mode==MODE_CLIENT) {
				//Movem els del jugador 2
				for (i = g->players[0].elements; i < g->players[0].elements+g->players[1].elements; i++){
					if (PA_SpriteTouched(g->element[i].sprite_id) && Stylus.Newpress){
						selected=i;	
					}
				}			
			} else {
				for (i = 0; i < g->players[0].elements+g->players[1].elements; i++){
					if (PA_SpriteTouched(g->element[i].sprite_id) && Stylus.Newpress){
						selected=i;
						
					}
		
				}
			}
		}
		wait_game();
		
	}
	if(game_mode!=MODE_OFFLINE) { notify("Esperant als altres jugadors...\n"); }
	
	game_sync(g,1);
	draw_pitch();
}

s8 move_objects(tgame *g) {
	debug("[move_objects]\n");
	u8 all_stoped=false; //Set this to true to detect the end of de movement
	u32 frame_move=0;
	s16 i,j;
	s8 team_goal=0;
	
	while (!all_stoped && g->neutral_alive>0) {
		frame_move++;
		for (i = 0; i < g->players[0].elements+g->players[1].elements+g->neutral_elements; i++){
			
			// Now, the g->element's fixed point position will be updated according to the speed...
			g->element[i].x += (g->element[i].vx);
			g->element[i].y += (g->element[i].vy);
		
			// If the sprite touches the left or right border, flip the horizontal speed
			if ((g->element[i].x - g->element[i].radi/2 <= g->pitch.upLeft_x) && (g->element[i].vx < 0)) { 
				g->element[i].vx = -g->element[i].vx;
				g->element[i].x = g->pitch.upLeft_x+g->element[i].radi/2;
			}
			else if ((g->element[i].x + g->element[i].radi/2 >= g->pitch.upRight_x)&&(g->element[i].vx > 0)) {
				g->element[i].vx = - g->element[i].vx;
				g->element[i].x = g->pitch.upRight_x-g->element[i].radi/2;
			}
	
			// Same thing, for top and bottom limits...
			if ((g->element[i].y - g->element[i].radi/2 <= g->pitch.upLeft_y) && (g->element[i].vy < 0)) {
				g->element[i].vy = -g->element[i].vy;
				g->element[i].y=g->pitch.upLeft_y+g->element[i].radi/2;
			}
			else if ((g->element[i].y + g->element[i].radi/2 >= g->pitch.downLeft_y)&& (g->element[i].vy > 0)) {
				g->element[i].vy = - g->element[i].vy;
				g->element[i].y = g->pitch.downLeft_y - g->element[i].radi/2;

			}
			team_goal=match_point(&g->element[i], g);
			if (g->players[0].score>=g->max_score || g->players[1].score>=g->max_score){
				return true;
			}
			// Test collisions for all g->elements with a smaller number...
			for (j = 0; j < i; j++){ 
				if (collision(&g->element[i],&g->element[j])
					// && (inMovment(&g->element[i]) || inMovment(&g->element[j]))
				) {
						if(debug_collision) {
							sprintf(tstring,"New collision [%4d][%2d,%2d]\n",frame_move,i,j);
							debug(tstring);
						}
					#ifdef REALFISICS
						collision_speed(&g->element[i],&g->element[j]);
					#else
						g->element[i].vx = (g->element[i].x - g->element[j].x)/6;
						g->element[i].vy = (g->element[i].y - g->element[j].y)/6;
						g->element[j].vx = -g->element[i].vx;
						g->element[j].vy = -g->element[i].vy;
					#endif
				}
			}
			//Abans estava a dalt
			PA_SetSpriteXY(BOTTOM_SCREEN,g->element[i].sprite_id, g->element[i].x-g->element[i].sprite_size/2, g->element[i].y-g->element[i].sprite_size/2);
			
			g->element[i].vx*=g->friction;
			g->element[i].vy*=g->friction;
			
			//Check the end of de round (end of movment of the objects)			
		}
		//Comprovem si tots estan parats
		all_stoped=true;
		for (i = 0; i < g->players[0].elements+g->players[1].elements+g->neutral_elements; i++){
			if(inMovment(&g->element[i])) {
				all_stoped=false;
			}
		}
		wait_game();  // Synch to the framerate...
	}
	return false;
}

/**
 - Realitza la simulació
 - Torna els jugadors a la posició original si es necessari
 - Detecta el final de la partida i retorna (true) si s'ha d'acabar
*/
bool round_match(tgame *g){
	debug("[round_match]\n");
	move_objects(g);
	if(g->neutral_alive==0) {
		reset_players(g);
	}
	if (g->players[0].score>=g->max_score || g->players[1].score>=g->max_score){
		return true;
	} 
	update_gfx(g);
	draw_pitch();

	return false;
}

/**
 Para tots els jugadors. V=0
 Es necessita ja que detectem el moviment en un cert interval i per sota 
d'aquest considerem parat.
*/
void end_round(tgame *g) {
	s16 i;
	debug("[end_round]\n");
	//Parem els objectes en moviment ja que la detecciï¿½ de moviment te un cert marge. Tardaria molt a arribar a 0 exacte
	for (i = 0; i < g->players[0].elements+g->players[1].elements+g->neutral_elements; i++){
		g->element[i].vx=0.0;
		g->element[i].vy=0.0;
	}
}

/**
 Final del partit
 - Esborrem tots els sprites
 - Notifiquem qui ha estat el guanyador
*/
void end_match(tgame *g) {
	
	//Mostra guanyador
	//Torna al chat
	u16 i;
	debug("[end_match]\n");
//	char *winner;
	//sprites to be deleted
	for(i=0;i<g->players[0].elements+g->players[1].elements+g->neutral_elements;i++)
			PA_DeleteSprite(BOTTOM_SCREEN,g->element[i].sprite_id);
	PA_DeleteSprite(BOTTOM_SCREEN,MARCADOR_SPRITE);
	PA_DeleteSprite(BOTTOM_SCREEN,MARCADOR_SPRITE+1);
	if(game_mode==MODE_SERVER) {
		if(g->players[0].score>g->players[1].score) {
			notify("Has guanyat!!\n");
		} else {
			notify("Has perdut!!\n");
		}
	} else if(game_mode==MODE_CLIENT) {
		if(g->players[0].score>g->players[1].score) {
			notify("Has perdut!!\n");
		} else {
			notify("Has guanyat!!\n");
		}
	} else {
		if(g->players[0].score>g->players[1].score) {
			notify("Ha guanyat el jugador 1\n");
		} else {
			notify("Ha guanyat el jugador 2\n");
		}
	}
	//while ( !Pad.Held.A) PA_WaitForVBL();
}

/**
 Tanca els sockets de la partida
*/
void end_game(void) {
	debug("[end_game]\n");
	chat_refresh=CHAT_REFRESH;
	closesocket(game_sock);
}

/**
 - crida a init_match
 - crida a round match
 - itera fins que el partit acaba
 - crida a end_match
*/
void match(void) {
	u8 round=0;
	debug("[match]\n");
	init_match(&game);
	bool endMatch=false;
	while (!endMatch){
		sprintf(tstring,"Tirada %d\n",round+1);
		notify(tstring);
		init_round(&game);
		endMatch=round_match(&game);
		end_round(&game);
		round++;
	}
	end_match(&game);
}
