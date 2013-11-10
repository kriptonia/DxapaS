/**
retorna el valor absolut
*/
s32 sabs(s32 numero);

/**
Retorna el valor absolut
*/
float floatabs(float numero);

/**
retorna la distancia d'un objecte rodo a una linea
*/
s32 get_distance_object_line(round_object *a, straight *line);

/**
Comprova si hi ha hagut una colisió i retorna cert si es el cas
*/
bool collision(round_object *a, round_object *b);

/**
Realitza els canvis en les velocitats pertinents al realitzar-se un xoc
*/
void collision_speed(round_object *a, round_object *b);

/**
Comprova si un objecte està en moviment
*/
bool inMovment(round_object *a);

