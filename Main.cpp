#include "Game.h"

int main(int argc, char** argv) {
	Game game;
	bool success = game.initialize();

	if (success) {
		game.runLoop();
	}

	game.shutDown();
	return 0;
}