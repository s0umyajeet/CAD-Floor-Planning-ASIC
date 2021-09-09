#pragma once
#include "SDL.h"
#include <string>

struct window_props {
	std::string window_name;
	unsigned int window_pos_x = SDL_WINDOWPOS_CENTERED;
	unsigned int window_pos_y = SDL_WINDOWPOS_CENTERED;
	int window_height;
	int window_width;

	window_props(std::string name, unsigned int xx, unsigned int yy, int height, int width) {
		this->window_name = name;
		this->window_pos_x = xx;
		this->window_pos_y = yy;
		this->window_height = height;
		this->window_width = width;
	}

	//default ctor
	window_props() {};
};
