#include <PA9.h>
#include <math.h>
#include "types.h"
#include "global.h"
#include "gui.h"
#include "debug.h"

/**
retorna el valor absolut
*/
s32 sabs(s32 numero) {
	if(numero<0) return -numero;
	return numero;
}

/**
Retorna el valor absolut
*/
float floatabs(float numero) {
	if(numero<0.0) return -numero;
	return numero;
}

/**
retorna la distancia d'un objecte rodo a una linea
*/
s32 get_distance_object_line(round_object *a, straight *line){
	s32 m=0;
	s32 b=0;
	s32 distance=0;	
	//distance  SQRT ( (X2-X1)Â² + (Y2-Y1)Â²)
	if ( line->x2==line->x1 ){
		distance=(s32)(line->x2 - a->x);
			return sabs(distance);
	}else if (line->y2==line->y1){
		distance=(s32)(line->y2 - a->y);
			return sabs(distance);
	}else{
		//y-y1=m(x-x1) y=mx+b
		m=(s32)((line->y2-line->y1)/(line->x2-line->x1));
		b=(s32)(line->y2 - m*line->x2);
		distance=(s32)(m*line->x2 - line->y2 + b)/sqrt(m*m + 1);
			return sabs(distance);
	}
}

/**
Comprova si hi ha hagut una colisió i retorna cert si es el cas
*/
bool collision(round_object *a, round_object *b) {
	s32 min_dist=0;
	s32 real_dist=0;
	min_dist=(a->radi/2+b->radi/2)*(a->radi/2+b->radi/2);
//	s32 dist_real=((a->x-b->x)*(a->x-b->x))+((a->y-b->y)*(a->y-b->y));
	real_dist=PA_Distance(a->x,a->y,b->x,b->y);
	if(min_dist>=real_dist) return true;
	return false;
}


/**
Realitza un canvi de base a un objecte rodo
*/
void rotate_base(s16 alfa,round_object *a) {
	float ax,ay;
	ax=(a->vx*PA_Cos(alfa)/256.0)+(a->vy*PA_Sin(alfa)/256.0);
	ay=-(a->vx*PA_Sin(alfa)/256.0)+(a->vy*PA_Cos(alfa)/256.0);
	if(debug_collision) {
		if (alfa>=0){
			sprintf(tstring,"Canvi base [%d] Alfa %4d || vx=%2.4f vy=%2.4f vi=%2.4f vj=%2.4f\n",a->sprite_id,alfa,a->vx,a->vy,ax,ay);
		}else{
			sprintf(tstring,"Canvi base [%d] Alfa %4d || vi=%2.4f vj=%2.4f vx=%2.4f vy=%2.4f\n",a->sprite_id,alfa,a->vx,a->vy,ax,ay);
		}
		debug(tstring);
	}
	a->vx=ax;
	a->vy=ay;
}

/**
Comprova si un objecte està en moviment
*/
bool inMovment(round_object *a) {
	return (floatabs(a->vx)>OBJECT_STOPED || floatabs(a->vy)>OBJECT_STOPED);
}

/**
Realitza els canvis en les velocitats pertinents al realitzar-se un xoc
*/
void collision_speed(round_object *a, round_object *b) {
	u16 alfa; //angle formed between the horizontal and the round_object 1&2 	
	float vai,vbi;
	
	alfa=PA_GetAngle(a->x, a->y, b->x,  b->y);

	a->vy=-a->vy;
	b->vy=-b->vy;

	rotate_base(alfa,a);
	rotate_base(alfa,b);
	
	
	vai=(2*b->massa*b->vx+(a->massa-b->massa)*a->vx)/(a->massa+b->massa);
	vbi=(2*a->massa*a->vx+(b->massa-a->massa)*b->vx)/(a->massa+b->massa);
	a->vx=vai;
	b->vx=vbi;
	if(debug_collision) {
		sprintf(tstring,"[Xoc] A.vx=%2.4f B.vx=%2.4f\n",vai,vbi);
		debug(tstring);
	}
	
	rotate_base(-alfa,a);
	rotate_base(-alfa,b);

	a->vy=-a->vy;
	b->vy=-b->vy;

	if(debug_collision) {
		sprintf(tstring,"Anti ReCollision [%d] X=%3.2f Y=%3.2f\n",a->sprite_id,a->x,a->y);
		debug(tstring);
		sprintf(tstring,"Anti ReCollision [%d] X=%3.2f Y=%3.2f\n",b->sprite_id,b->x,b->y);
		debug(tstring);
	}

	a->x+=a->vx;
	a->y+=a->vy;
	b->x+=b->vx;
	b->y+=b->vy;
	
	
	
	if(debug_collision) {
		sprintf(tstring,"Anti ReCollision [%d] X=%3.2f Y=%3.2f\n",a->sprite_id,a->x,a->y);
		debug(tstring);
		sprintf(tstring,"Anti ReCollision [%d] X=%3.2f Y=%3.2f\n",b->sprite_id,b->x,b->y);
		debug(tstring);
	}

	if(debug_collision) {
		pause_collision=true;
		while(pause_collision) {
			WaitVBL();
		}
		pause_collision=true;
	}
}

