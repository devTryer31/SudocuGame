#include "Game.h"

#include "profile.h"
#include <iostream>

#pragma warning (disable : 26812)

int main() {
	//first_seeds_generate(5);
	Game game;
	game.build_field(3u, 70u);
	game.loope();
	
	return 0;
}