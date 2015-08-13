//GOL's Game engine
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <allegro.h>

#define PI 3.14159265

//Colors
#define C_WHITE makecol(255,255,255)
#define C_BLUE makecol(50,50,255)
#define C_RED makecol(255,50,50)
#define C_GREEN makecol(0,255,0)
#define C_BLACK makecol(0,0,0)

extern int end_game,
	ticks;
extern BITMAP* buffer;

void init(int argc, char** argv);
void config(int, int);
void init_gfx(int, int);
void start();
void tick();
void input();
void game_loop();
void output();
void finish();
