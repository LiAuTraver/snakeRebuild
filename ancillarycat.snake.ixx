module;

#include <Windows.h>
#include <mmsystem.h>

export module  ancillarycat.snake;

import ancillarycat.print;
import ancillarycat.console;

export class Snake {

}snake;

namespace game
{
export void snakeGame()
{
	console
		.setCursorCoordinate(0, 0);
	system("CLS");
	SHORT& width = console.width;
	SHORT& height = console.height;
	console.box(5,5,10,10);
}
}