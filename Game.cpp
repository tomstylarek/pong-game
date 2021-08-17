// implementacion de la clase Game

#include "Game.h"

Game::Game() : window(nullptr), isRunning(true) {}

// returns true if initialization of SDL and window creation is successful
bool Game::initialize() {
	// SDL initialization. 0 if success
	// recieves a set of subsistems to use
	int sdlRunning = SDL_Init(SDL_INIT_VIDEO);

	// in case of failure
	if (sdlRunning != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// after library initialization, create the window which will contain the game
	this->window = SDL_CreateWindow(
		"Pong", // window title
		100, // x and y coordinated of the top-left corner of the window
		100,
		1024, // size
		768,
		0 // aditional flags, such as fullscreen mode, rezisable, etc.
	);

	// if window is a nullptr, the window creation failed
	if (!this->window) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	return true;
}

void Game::shutDown() {
	SDL_DestroyWindow(this->window);
	SDL_Quit();
}

void Game::runLoop() {

	while (this->isRunning) {
		processInput();
		updateGame();
		generateOutput();
	}
}