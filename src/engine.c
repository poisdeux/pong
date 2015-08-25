/**
 * Copyright 2008 Bruno Guedes
 * Copyright 2015 AT Computing
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

#include <engine.h>

#define BALL_SIZE (4)
#define VERSION "1.0"

int end_game, game_over, timer;
BITMAP* buffer;
SAMPLE *bump_bleep, *match_bleep;

char allegro_error[ALLEGRO_ERROR_SIZE];

typedef struct position position;

struct position {
  int x;
  int y;
};

static position ball;

typedef struct paddle paddle;

struct paddle {
	int height;
	int width;
	int speed;
	position pos;
};

static paddle player1; 
static paddle player2;

//Game Vars
static int paddle_visible_distance, score_P1, score_P2;
static float ball_hspeed,
	
	ball_vspeed;

//Game Options
static int ball_speed,
	two_player;
static float speed;

//Initialization Functions
void init(int argc, char** argv)
{
	end_game = 1; //On any error, get out!
	if (allegro_init() != 0)
	{
		fprintf(stderr,"ALLEGRO ABORTED!\n");
		return;
	}
	
	char x;
	int width = 640,
		height = 480;
	int opt_paddle_size = 64;

	paddle_visible_distance = -1;
	speed = 1.0;
	ball_speed = 3;
	two_player = 0;
	
	while((x = getopt(argc,argv,"w:h:s:p:d:2H")) != -1)
	{
		switch(x)
		{
			case 'w': //grid width
				sscanf(optarg,"%d",&width);
				break;
			case 'h': //grid height
				sscanf(optarg,"%d",&height);
				break;
			case 's': //game speed
				sscanf(optarg,"%f",&speed);
				break;
			case 'p': //paddle size
				sscanf(optarg,"%d", &opt_paddle_size);
				break;
			case 'd': //ball distance at which paddle will be visible
				sscanf(optarg,"%d", &paddle_visible_distance);
				break;
			case '2': //2P mode activated
				two_player = 1;
				break;
			case 'H':
			default:
				printf("Usage: ./pong-%s [-w <width>] [-h <height>] [-s <speed>] [-p <paddle_size>] [-2] [-H]\n", VERSION);
				printf("\t-w W: defines the window width to W (default: 640)\n");
				printf("\t-h H: defines the window width to H (default: 480)\n");
				printf("\t-s S: defines the game speed to factor S (default: 1.0)\n");
				printf("\t-p P: defines the size of the paddle to O (default: 64)\n");
				printf("\t-d D: defines the distance in pixels between ball and paddle at which the paddle will be visible (default: window width)\n");
				printf("\t-2: enables 2-player mode\n");
				printf("\t-H: prints this help.\n");
				return;
		}
	}

	player1.height = player2.height = opt_paddle_size;
	player1.width = player2.width = 4;
	player1.speed = player2.speed = 3;

	//initialize game components	
	config(width, height);
	
	start();
}

//Game Configuration
//Based upon comandline expressions
void config(int w, int h)
{
	//Keyboard
	//No key repeating: only key presses!
	if (install_keyboard() < 0)
	{
		fprintf(stderr, "Trying to configure keyboard, couldn't: %s", allegro_error);
		return;
	}
	fprintf(stderr, "Keyboard set up succesfully!\n");
	
	//Sound
	//Yay, Sound! \o/
	if (install_sound(DIGI_AUTODETECT, MIDI_NONE, NULL) < 0)
	{
		fprintf(stderr, "Trying to configure sound, couldn't: %s", allegro_error);
	}
	else
	{
		fprintf(stderr, "Sound card set up succesfully!\n");
	}
	bump_bleep = load_wav("snd/beep.wav");
	match_bleep = load_wav("snd/deedoop.wav");
	
	init_gfx(w, h);
}

//Initialize graphics
void init_gfx(int w, int h)
{
	set_color_depth(24);
	if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, w, h, 640, 480) != 0)
		printf("Error setting graphic mode\n");
	buffer = create_bitmap(SCREEN_W, SCREEN_H);
}

//Start the game
//For future plans: menu
void start()
{
	if(paddle_visible_distance < 0) {
		paddle_visible_distance = SCREEN_W;
	}

	player1.pos.x = 31;
	player2.pos.x = SCREEN_W - 31;
	player1.pos.y = player2.pos.y = ball.y = SCREEN_H/2;
	score_P1 = score_P2 = 0;
	ball.x = SCREEN_W/2;
	
	srand(time(NULL));
	float dir = ((float)(rand()%9100-4600)/100) * (PI/180); //0 <= dir < 2*PI
	ball_hspeed = ball_speed * cos(dir);
	ball_vspeed = ball_speed * sin(dir);
	
	//Timer
	//60 ticks per second
	install_timer();
	install_int(tick, 1000/100/speed);
	fprintf(stderr, "Timer routines set up succesfully!\n");
		
	end_game = game_over = 0;
}

//Game Loop Functions

void tick()
{
	ticks++;
}

//User input
void input()
{
	if (keypressed())	
	{
		readkey();
		
		//Quit on Q
		if (key[KEY_Q])
		{
			end_game = 1;
			return;
		}
	}
	
	//P1 Controls
	if (key[KEY_UP] && player1.pos.y >= player1.height/2)
	{
		player1.pos.y -= player1.speed;
	}
	if (key[KEY_DOWN] && player1.pos.y <= (SCREEN_H-player1.height/2))
	{
		player1.pos.y += player1.speed;
	}
	
	//P2 Controls
	if (two_player)
	{
		if (key[KEY_A] && player2.pos.y >= player2.height/2)
		{
			player2.pos.y -= player2.speed;
		}
		if (key[KEY_Z] && player1.pos.y <= (SCREEN_H-player2.height/2))
		{
			player1.pos.y += player2.speed;
		}
	}
}

//Game output
void output()
{
	//Clear screen from previous tick
	clear_bitmap(buffer);
	
	//Draw field
	int i;
	for (i=8; i<SCREEN_H; i+=64)
	{
		rectfill(buffer, SCREEN_W/2 - 1, i, SCREEN_W/2 + 1, i+32, C_WHITE);
	}
	
	//Draw Paddles
	//Hide or show left paddle
	if( (ball.x - player1.pos.x) < paddle_visible_distance )
	{		
		rectfill(buffer, player1.pos.x, player1.pos.y - player1.height/2, player1.pos.x + player1.width, player1.pos.y + player1.height/2, C_BLUE);
	}

	if( (player2.pos.x - ball.x) < paddle_visible_distance )
	{
		rectfill(buffer, player2.pos.x, player2.pos.y - player2.height/2, player2.pos.x - player2.width, player2.pos.y + player2.height/2, C_RED);
	}

	//Draw Ball
	circlefill(buffer, ball.x, ball.y, BALL_SIZE/2, C_WHITE);
	
	//Draw Score
	extern FONT *font;
	textprintf_ex(buffer,font, 64, 4,C_GREEN, -1, "%d", score_P1);
	textprintf_ex(buffer,font, SCREEN_W-64, 4,C_GREEN, -1, "%d", score_P2);

	if (game_over) 
	{
		textprintf_centre_ex(buffer,font, SCREEN_W/2, SCREEN_H/2,C_WHITE, -1, "GAME OVER");
	}
	
	//Debugger
	//textprintf_ex(buffer, font, 4, 4, C_WHITE, -1, "Ball Speed: %f, %f", ball_hspeed, ball_vspeed);
	
	masked_blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
}

//Game loop processing
void game_loop()
{
	input();
	
	//check for "restart" and "end" signals
	if (end_game) return;

	//Check collision with walls
	if (ball.y <= BALL_SIZE || ball.y >= SCREEN_H - BALL_SIZE)
	{
		play_sample(bump_bleep,128,128,1000,0);
		
		ball_vspeed = -ball_vspeed;
		ball.y = (ball.y > SCREEN_H/2? SCREEN_H-(BALL_SIZE*2): BALL_SIZE*2);
	}
	
	//collision with paddles
	if (ball.x < (player1.pos.x+player1.width*2) && ball.x > player1.pos.x && ball.y >= player1.pos.y - player1.height/2 && ball.y <= player1.pos.y + player1.height/2)
	{
		play_sample(bump_bleep,128,128,1000,0);
		
		ball_hspeed = -ball_hspeed;
		ball_vspeed = ball_speed * sin(atan(ball_vspeed/ball_hspeed + (ball.y-player1.pos.y)/(player1.height/2)));
		ball_hspeed = (ball_hspeed/abs(ball_hspeed)) * ball_speed * cos(asin(ball_vspeed/ball_speed));
		
		ball.x += ball_hspeed;
	}

//	if (ball.x >= SCREEN_W-(31+paddle_width*2-1) && ball.x <= SCREEN_W-31 && ball.y >= player2.y - right_paddle_size/2 && ball.y <= player2.y + right_paddle_size/2)	
	
	if (ball.x > (player2.pos.x-(player2.width*2)) && ball.x < player2.pos.x && ball.y >= player2.pos.y - player2.height/2 && ball.y <= player2.pos.y + player2.height/2)
	{
		play_sample(bump_bleep,128,128,1000,0);
		
		ball_hspeed = -ball_hspeed;
		ball_vspeed = ball_speed * sin(atan(ball_vspeed/ball_hspeed + (ball.y-player2.pos.y)/(player2.height/2)));
		ball_hspeed = (ball_hspeed/abs(ball_hspeed)) * ball_speed * cos(asin(ball_vspeed/ball_speed));
		
		ball.x += ball_hspeed;
	}
	
	//point marking!
	if (ball.x <= 0)
	{
		play_sample(match_bleep,128,128,1000,0);
		score_P2++;
		player1.height -= 2;
		ball.x = SCREEN_W/2;
		ball.y = SCREEN_H/2;
	}
	
	if (ball.x >= SCREEN_W - 4)
	{
		play_sample(match_bleep,128,128,1000,0);
		score_P1++;
		player2.height -= 2;
		ball.x = SCREEN_W/2;
		ball.y = SCREEN_H/2;
	}

	if ((player1.height < 2) || (player2.height < 2)) 
	{
		game_over = 1;
	}

	if (!two_player)
	{
		//AI... not so smart, kay?
		if (ball_hspeed > 0 && ball.x >= (SCREEN_W - SCREEN_W/4 * (ball_hspeed/ball_speed)))
		{
			if (ball.y > (player2.pos.y + player2.height/2) && player2.pos.y < (SCREEN_H - player2.height/2))
				player2.pos.y += player2.speed;
			else if (ball.y < (player2.pos.y - player2.height/2) && player2.pos.y > player2.height/2)
				player2.pos.y -= player2.speed;
		}
	}

	if( !game_over) {	
		//Move ball	
		ball.x += ball_hspeed;
		ball.y += ball_vspeed;
	}

	output();
}

//Game End Functions
void finish()
{
	remove_keyboard();
	remove_timer();
	//destroy_sample(bump_bleep);
	//destroy_sample(match_bleep);
	remove_sound();
}
