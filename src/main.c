//Game of Life 0.9
//by Bruno Guedes
#include <engine.h>

int end_game, ticks;

int main(int argc, char** argv)
{
	init(argc, argv); //Initialize
	
	do {
		if (ticks)
		{
			game_loop();
			ticks--;
		}
	} while (!end_game);
	
	finish();
	return 0;
}

END_OF_MAIN()
