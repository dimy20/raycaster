#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <vector>
#include <cmath>

#include "Engine.h"
int main(){
	Engine engine;
	engine.init();
	engine.run();
	return 0;
};
