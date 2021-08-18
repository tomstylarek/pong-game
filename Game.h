#ifndef _GAME_H_
    #define _GAME_H_

#include "SDL.h"

class Game 
{
private:
	SDL_Window* window;
	bool isRunning;

	void processInput();
	void updateGame() {};
	void generateOutput() {};

public:
	// constructor de la clase
	Game();

	bool initialize();
	void runLoop();
	void shutDown();

};

#endif