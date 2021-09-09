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

std::mutex m;


void engine() {
	window_props window_props(std::string("iHack-CAD"),
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		1280, 1280
	);

	if (!GraphicEngine::get().init(window_props)) {
		std::cout << "Error" << std::endl;
		EXIT_FAILURE;
	} else {}

	int fps = 60;
	int desiredDelta = 1000 / fps;
	
	
	while (GraphicEngine::get().isRunning()) {
		//non-blocking lock
		if (m.try_lock()) {
			int start = SDL_GetTicks();
			GraphicEngine::get().drawGUI();
			GraphicEngine::get().render();
			int delta = SDL_GetTicks() - start;

			if (delta < desiredDelta) {
				SDL_Delay(desiredDelta - delta);
			}
			m.unlock();
		}
		GraphicEngine::get().handleEvents();
	}

	GraphicEngine::get().quit();
}


void parser() {
	//producer producing shape data
	using namespace std::literals::chrono_literals;

	//parse shapes.csv && update the shapeID-vs-area table (once)
	InputHandler::get().parseShapeCSV(ActiveShapeBuffer::get().areaFile);

	//first parse
	GraphicEngine::row_count = InputHandler::get().parsePlacementCSV(ActiveShapeBuffer::get().placementFile);
	std::cout << "grpahicccc count" << GraphicEngine::row_count << std::endl;
	GraphicEngine::firstParse = true;
	
	bool editor_is_running = true;
	while (editor_is_running) {
		if (m.try_lock()) {
 			InputHandler::get().parsePlacementCSV(ActiveShapeBuffer::get().placementFile);
			m.unlock();
		}
	}
	std::cout << "done" << std::endl;
}

int main(int argc, char* argv[]) {
	std::thread producer(parser);
	engine();
	producer.join();
	return 0;
}


