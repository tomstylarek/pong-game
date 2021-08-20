#include "Game.h"
#include <stdlib.h>
#include <time.h>


const int windowWidth = 1024;
const int windowHeight = 600;
const int thickness = 15;
const int paddleHeight = 100;

Game::Game() : window(nullptr), renderer(nullptr), isRunning(true), ticksCount(0), paddle1Direction(0), paddle2Direction(0) {
	this->ballPosition.x = windowWidth / 2;
	this->ballPosition.y = windowHeight / 2;
	this->paddle1Position.x = 15.0f; // a bit to the right
	this->paddle1Position.y = windowHeight / 2.0f;
	this->paddle2Position.x = windowWidth - 15.0f;
	this->paddle2Position.y = windowHeight / 2.0f;


	// randomize the initial direction of the ball when the game starts
	srand(time(NULL));
	const int xDir = rand() % 2 == 0 ? -1 : 1;
	const int yDir = rand() % 2 == 0 ? -1 : 1;

	this->ballVelocity.x = xDir * 250.0f;
	this->ballVelocity.y = yDir * 235.0f;
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
		windowWidth, // size
		windowHeight,
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
		this->processInput();
		this->updateGame();
		this->generateOutput();
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

    // handle the input for moving the paddle
    // in this case, w for moving the paddle up, and s for moving it down
    this->paddle1Direction = 0;
    this->paddle2Direction = 0;

    // -1 represents the negative y direction, that is, moving up, and 1 moving down
    // adding and substracting makes that, if the user presses both keys, the paddle won't move
    if (state[SDL_SCANCODE_W]) {
    	this->paddle1Direction -= 1;
    }

    if (state[SDL_SCANCODE_S]) {
    	this->paddle1Direction += 1;
    }

    if (state[SDL_SCANCODE_UP]) {
    	this->paddle2Direction -= 1;
    }

    if (state[SDL_SCANCODE_DOWN]) {
    	this->paddle2Direction += 1;
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
		windowWidth, // size of the rectangle
		thickness
	};

	SDL_Rect bottomWall {
		0,
		windowHeight - thickness,
		windowWidth,
		thickness
	};

	// create the ball and the paddel

	// note that the position of the ball references its center, and the parameter takes the top left corner
	SDL_Rect ball {
		static_cast<int>(this->ballPosition.x - thickness / 2),
		static_cast<int>(this->ballPosition.y - thickness / 2),
		thickness,
		thickness
	};

	SDL_Rect paddle1 {
		static_cast<int>(this->paddle1Position.x - thickness / 2),
		static_cast<int>(this->paddle1Position.y - paddleHeight / 2),
		thickness,
		paddleHeight
	};

	SDL_Rect paddle2 {
		static_cast<int>(this->paddle2Position.x - thickness / 2),
		static_cast<int>(this->paddle2Position.y - paddleHeight / 2),
		thickness,
		paddleHeight
	};

	// render all
	SDL_RenderFillRect(this->renderer, &topWall);
	SDL_RenderFillRect(this->renderer, &bottomWall);
	SDL_RenderFillRect(this->renderer, &ball);
	SDL_RenderFillRect(this->renderer, &paddle1);
	SDL_RenderFillRect(this->renderer, &paddle2);

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

	// if the user is pressing w or s, the event will handle in processInput
	// and we recieve here -1 or 1 depending on the direction of the movement
	if (this->paddle1Direction != 0) {

		// velocity of 300 pixels per second
		this->paddle1Position.y += this->paddle1Direction * 300.0f * deltaTime;

		// control of the paddle position so it won't get off screen
		if (this->paddle1Position.y < (paddleHeight / 2.0f + thickness)) {
			this->paddle1Position.y = paddleHeight / 2.0f + thickness;
		} else if (this->paddle1Position.y > (windowHeight - paddleHeight / 2.0f - thickness)) {
			this->paddle1Position.y = windowHeight - paddleHeight / 2.0f - thickness;
		}
	}

	if (this->paddle2Direction != 0) {

		// velocity of 300 pixels per second
		this->paddle2Position.y += this->paddle2Direction * 300.0f * deltaTime;

		// control of the paddle position so it won't get off screen
		if (this->paddle2Position.y < (paddleHeight / 2.0f + thickness)) {
			this->paddle2Position.y = paddleHeight / 2.0f + thickness;
		} else if (this->paddle2Position.y > (windowHeight - paddleHeight / 2.0f - thickness)) {
			this->paddle2Position.y = windowHeight - paddleHeight / 2.0f - thickness;
		}
	}


	// update ball position
	this->ballPosition.x += this->ballVelocity.x * deltaTime;
	this->ballPosition.y += this->ballVelocity.y * deltaTime;

	// if the ball collides with the walls, change the direction of movement accordingly

	// if it collides and it's moving upwards
	if (this->ballPosition.y <= thickness / 2.0f + thickness && this->ballVelocity.y < 0) {
		// invert the direction
		this->ballVelocity.y = -this->ballVelocity.y;
	} else if (this->ballPosition.y > windowHeight - thickness / 2.0f - thickness && this->ballVelocity.y > 0) {
		this->ballVelocity.y = -this->ballVelocity.y;
	}

	// handle collision of the ball both paddles
	int dif1 = abs(this->ballPosition.y - this->paddle1Position.y);
	int dif2 = abs(this->ballPosition.y - this->paddle2Position.y);

	if (
		// x aligning with the paddle
		this->ballPosition.x <= 2 * thickness && this->ballPosition.x >= thickness + thickness / 2 && 
		this->ballVelocity.x < 0 &&    // ball is moving to the paddle
		dif1 <= paddleHeight / 2 + thickness / 2    // it doesn't touch the paddle
	) {
		// randomize velocity up to 2x
		this->ballVelocity.x = -this->ballVelocity.x * (1 + 0.2f * ((float) rand()) / (float) RAND_MAX);
		// change sign of direction in y
		this->ballVelocity.y = rand() % 2 == 0 ? -1 : 1 * 235.0f;
	} else if (
		this->ballPosition.x >= windowWidth - 2 * thickness && this->ballPosition.x <= windowWidth - (thickness + thickness / 2) && 
		this->ballVelocity.x > 0 &&    
		dif2 <= paddleHeight / 2 + thickness / 2    
	) {
		this->ballVelocity.x = -this->ballVelocity.x * (1 + 0.2f * ((float) rand()) / (float) RAND_MAX);
		this->ballVelocity.y = rand() % 2 == 0 ? -1 : 1 * 235.0f;
	}


	// handle ball off screen
	if (this->ballPosition.x <= 0 || this->ballPosition.x >= windowWidth) {
		this->isRunning = false;
	}
}