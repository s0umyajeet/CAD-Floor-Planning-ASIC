#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <fstream>

//------------------------
#include "SDL.h"
#include "GraphicEngine.h"
#include "ActiveShapeBuffer.h"
#include "InputHandler.h"
#include "window_props.h"
#include "rapidcsv.h"

//--------IMGUI------------
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_sdl.h"
#include <SDL_opengl.h>


void engine() {
	window_props window_props(std::string("iHack-CAD"),
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		1280, 1280
	);

	if (!GraphicEngine::get().init(window_props)) {
		std::cout << "Error" << "\n";
		EXIT_FAILURE;
	}

	int fps = 60;
	int desiredDelta = 1000 / fps;
	
	while (GraphicEngine::get().isRunning()) {

		int start = SDL_GetTicks();
		GraphicEngine::get().drawGUI();
		GraphicEngine::get().render();
		int delta = SDL_GetTicks() - start;

		if (delta < desiredDelta) 
			SDL_Delay(desiredDelta - delta);
			
		GraphicEngine::get().handleEvents();
	}

	GraphicEngine::get().quit();
}

int main(int argc, char* argv[]) {

	//parse shapes.csv && update the shapeID-vs-area table (once)

	if (argc != 3) {
		std::cout << "Usage: visualizer <areaFilePath> <placementFilePath>" << "\n";
		return EXIT_SUCCESS;
	}

	if (ActiveShapeBuffer::loadFiles(std::string(argv[1]), std::string(argv[2]))) {
		std::cout << "Loading files..." << "\n";
	} else {
		std::cout << "File loading failed" << "\n";
		return 0;
	}


	InputHandler::get().parseShapeCSV(ActiveShapeBuffer::get().areaFile);

	//first parse
	GraphicEngine::row_count = InputHandler::get().parsePlacementCSV(ActiveShapeBuffer::get().placementFile);
	//std::cout << "grpahicccc count " << GraphicEngine::row_count << "\n";
	GraphicEngine::firstParse = true;
	GraphicEngine::parseCount++;

	/*
	std::cout << ActiveShapeBuffer::get().shapeAreaMap.size() << "\n";
	for (auto x : ActiveShapeBuffer::get().shapeAreaMap)
		std::cout << "here " << x.first << "  " << x.second << "\n";

	

	
	for (auto x : ActiveShapeBuffer::get().shapePlacementMap) {
		std::cout << x.first << " ";
		x.second.show_data();
	}*/
	
	
	if (GraphicEngine::firstParse) {
		engine();
		/*
		for (auto x : ActiveShapeBuffer::get().shapePlacementMap) {
			std::cout << x.first << " ";
			x.second.show_data();
		}
		*/
	}


	return 0;
}


