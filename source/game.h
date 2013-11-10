/**
Agafa els valors per defecte del joc
*/
#define config_game_defaults(game) setGame(game,BALL_NUMBER,PLAYER1_NUMBER,FRICTION,MAX_SCORE) 
/**
Posa els valors al joc
*/
void setGame(tgame *g,u8 neutral, u8 playerElem,float friction,u8 maxScore);

/**
Copia un joc sobre un altre
*/
#define cpyGame(from,to) memcpy(to,from,sizeof(tgame))

/**
Inicialitza el videojoc
 - Crea els sockets
 - Es conecta o espera les conexions
   El tipus d'inicialització es:
	MODE_OFFLINE
	MODE_CLIENT
	MODE_SERVER
*/
u8 init_game(u8 type,char *ip);

/**
Prepara un partit
 - Carrega les paletes de colors
 - Dibuixa el fons de pantalla
 - Carrega la configuració del joc
 - Inicialitza les puntuacions
 - Posa els jugadors a la seva posició
 - Es sincronitza amb l'oponent
*/
void init_match(tgame *g);

/**
 - Es llegeixen les noves velocitats de les xapes
 - Es sicronitza amb l'oponent
 */
void init_round(tgame *g);

/**
 - Realitza la simulació
 - Torna els jugadors a la posició original si es necessari
 - Detecta el final de la partida i retorna (true) si s'ha d'acabar
*/
bool round_match(tgame *g);

/**
 Para tots els jugadors. V=0
 Es necessita ja que detectem el moviment en un cert interval i per sota 
d'aquest considerem parat.
*/
void end_round(tgame *g);

/**
 Tanca els sockets de la partida
*/
void end_game(void);

/**
 Final del partit
 - Esborrem tots els sprites
 - Notifiquem qui ha estat el guanyador
*/
void end_match(tgame *g);

/**
 - crida a init_match
 - crida a round match
 - itera fins que el partit acaba
 - crida a end_match
*/
void match(void);

/**
 Sincronitza el servidor i el client
 - mode 0: Sincronització completa
 - mode 1: Sincronització parcial
   En aquest mode envies les dades dels teus jugadors a l'oponent
*/
void game_sync(tgame *g,u8 sync_mode);

/**
1) Realitza el sincronisme vertical
2) Comprova si es demana el teclat 
3) Actulitza el debug 
*/
void wait_game(void);

/**
Amaga tots els sprites de la pantalla
*/
void hide_sprites(void);

/**
Actualitza els sprites i els posa al seu lloc
*/
void update_gfx(tgame *g);

/**
Dibuixa el terreny de joc
*/
void draw_pitch(void);
