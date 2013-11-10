#ifndef TYPESH 
#define TYPESH 

#define MAX_ELEMENTS 15
typedef struct{
	s16 x1,y1;
	s16 x2,y2;
}straight; 

typedef struct{
	float x, y; // This is for the r_object's position
	float vx, vy; // r_object speed
	u8 angle; // To make the r_object turn on itself
	u8 massa;
	u8 radi;
	u8 sprite_id; //Save sprite ID
	u8 sprite_size;
} round_object;

typedef struct{
	straight line;
	u8 team;
} goal;

typedef struct{
	goal goals[2];
	s16 upLeft_x,upLeft_y;
	s16 downLeft_x,downLeft_y;
	s16 upRight_x,upRight_y;
	s16 downRight_x,downRight_y;
	s16 padding;
}tpitch;

typedef struct{
	u8 score;
	u8 score_sprite_id;
	u8 elements;
	char name[30];
}player;

typedef struct {
	unsigned int	len;
	unsigned int	max;
	char		*string;
} text;

typedef struct {
	round_object	element[MAX_ELEMENTS];
	u8		neutral_elements;
	u8		neutral_alive;
	player		players[2];
	tpitch		pitch;
	float friction;
	float stop_object;
	u8 max_score;
} tgame;
#endif

