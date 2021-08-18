#ifndef _GAME_H_
    #define _GAME_H_

#include "SDL.h"

struct Vector2 {
	float x;
	float y;
};

class Game 
{
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	bool isRunning;
	Vector2 ballPosition;
	Vector2 paddlePosition;

	void processInput();
	void updateGame() {};
	void generateOutput();

public:
	// constructor de la clase
	Game();

	bool initialize();
	void runLoop();
	void shutDown();

};

#endif