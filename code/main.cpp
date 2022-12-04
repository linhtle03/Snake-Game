#include "Snake.h"

int main()
{
	// Declare an instance of Engine
	Snake Snake(float startX, float startY);

	// Start the engine
	Snake.moveDown();
    Snake.moveUp();
    Snake.moveLeft();
    Snake.moveRight();

	// Quit in the usual way when the engine is stopped
	return 0;
}