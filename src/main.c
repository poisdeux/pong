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
