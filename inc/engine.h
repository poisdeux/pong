/**
 * Copyright 2008 Bruno Guedes
 *
 * This file is part of Pong.
 *
 * Pong is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Pong is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Pong.  If not, see <http://www.gnu.org/licenses/>.
 */

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
