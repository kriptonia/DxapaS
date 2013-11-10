/**
Treu el text passat per la sortida configurada al debug o en cas d'estar 
desactivat no fa res
*/
void debug(char *text);

/**
Inicialitza el debug:
 mode: DEBUG_SOCKET 1, DEBUG_DISPLAY
 El mode assigna el display o un socket a la sortida de missatges.
 En cas de seleccionar un socket, es posa en mode escolta al port 7773 
esperant una conexió
*/
void debug_init(u8 mode);

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
void debug_read(void);

