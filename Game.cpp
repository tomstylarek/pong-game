#include "Game.h"

Game::Game() : window(nullptr), renderer(nullptr), isRunning(true), ticksCount(0) {
	ballPosition.x = 1024 / 2;
	ballPosition.y = 300;
	paddlePosition.x = 15; // a bit to the right
	paddlePosition.y = 300;
}

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
		600,
		0 // aditional flags, such as fullscreen mode, rezisable, etc.
	);

	// if window is a nullptr, the window creation failed
	if (!this->window) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// creation of renderer for 2D graphics
	this->renderer = SDL_CreateRenderer(
		this->window, // window to create renderer for
		-1, // rendering driver to initialize. -1 for initializing the first one that supports the flags
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC // sdl renderer flags
	);

	// if the renderer fails to initialize
	if (!this->renderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	return true;
}

void Game::shutDown() {
	SDL_DestroyWindow(this->window);
	SDL_DestroyRenderer(this->renderer);
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


// We have two buffers in a game. One from where the display takes the information to display on the window
// and the other where the game updates the game scene. 
// This function should clear the back buffer to a color (the game's buffer), draw the game scene, and last,
// swap buffers between the front buffer and the back buffer. 
void Game::generateOutput() {
	// first, set the renderer draw color
	SDL_SetRenderDrawColor(
		this->renderer,
		70, // R
		70, // G
		70, // B
		255 // A
	);

	// clear the back buffer to the current draw color
	SDL_RenderClear(this->renderer);

	// -- draw the game scene --

	// change the drawing color. This is like a global color for all objects.
	// If it's not changed, all would have the same color
	SDL_SetRenderDrawColor(
		this->renderer,
		20,
		20,
		20,
		255 // white
	);

	// create the walls of the game, so the ball can hit them and bounce
	// for now it's just one player, so we need three walls
	SDL_Rect topWall {
		0, // x y coordinates on the window
		0,
		1024, // size of the rectangle
		15
	};

	SDL_Rect bottomWall {
		0,
		600 - 15,
		1024,
		15
	};

	SDL_Rect rightWall {
		1024 - 15,
		0,
		15,
		600
	};

	// create the ball and the paddel

	// note that the position of the ball references its center, and the parameter takes the top left corner
	SDL_Rect ball {
		static_cast<int>(this->ballPosition.x - 15 / 2),
		static_cast<int>(this->ballPosition.y - 15 / 2),
		15,
		15
	};

	SDL_Rect paddle {
		static_cast<int>(this->paddlePosition.x - 15 / 2),
		static_cast<int>(this->paddlePosition.y - 80 / 2),
		15,
		80
	};

	// render all
	SDL_RenderFillRect(this->renderer, &topWall);
	SDL_RenderFillRect(this->renderer, &bottomWall);
	SDL_RenderFillRect(this->renderer, &rightWall);
	SDL_RenderFillRect(this->renderer, &ball);
	SDL_RenderFillRect(this->renderer, &paddle);

	// swap the buffers
	SDL_RenderPresent(this->renderer);
}


void Game::updateGame() {
	// handle the time of the game, so it's possible to update the position of the objects
	// according to the deltaTime per frame, and not to a quantity of pixels.
	// In this way, the movement of the objects in the game doesn't depend on the FPS of the game, nor 
	// in the Hz of the processor

	// frame limiting
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), this->ticksCount + 16));

	// GetTicks returns the miliseconds elapsed since the Init of the game
	float deltaTime = (SDL_GetTicks() - this->ticksCount) / 1000.0f;

	// set a maximum for delta time
	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}

	// update so we can calculate the delta time in the next frame
	this->ticksCount = SDL_GetTicks();

	// Now, update the objects in the game as a function of the delta time
}