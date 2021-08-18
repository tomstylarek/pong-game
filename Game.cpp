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

// the events that the user generate in the window are inputs that need to be handled
// in order to give a response to the user.
// Every frame of the game can have multiple events and they are all in the queue that SDL implements.
// We need to handle every event iterating throug all of them in every frame of the game.
void Game::processInput() {
    SDL_Event event;
    
    // while there are events to process in the queue
    while (SDL_PollEvent(&event)) {
        
        switch (event.type) {
            // here we handle all the possible events that the user can do
            // using the event types SDL provides
            
            // for example, handling the event of quitting the game
            case SDL_QUIT:
                this->isRunning = false;
                break;
        }
    }

    // the user could want to quit the game with the esc key, so we can handle that.
    // A convinient way to do it is using another function that give us the state of the keyboard.
    // Instead of handling each key pressing in the swith above, we can do it here

    // an internal array of SDL with the keys that have been pressed
    const Uint8* state = SDL_GetKeyboardState(NULL);

    // The keys are constants that have this suffix. 1 if they have been pressed.
    if (state[SDL_SCANCODE_ESCAPE]) {
    	this->isRunning = false;
    }
}
