#pragma once
#include "SDL.h"
#include "Shape.h"
#include "window_props.h"
#include <vector>
#include "rapidcsv.h"

//IMGUI
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_sdl.h"

#include <SDL_opengl.h>


//visualizer window properties
struct vis_props {
	float zoom;
	float moveX;
	float moveY;
	float  thickness;
	bool  legend;
	static bool multicolored;
	bool  reset;
	static ImVec4 defaultColor;

	vis_props() {
		zoom = 1.0f;
		moveX = 200.0f;
		moveY = 200.0f;
		thickness = 1.0f;
		legend = false;
		//multicolored = false;
		reset = false;
		//defaultColor = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
	}

	void reset_props() {
		zoom = 1.0f;
		moveX = 0.0f;
		moveY = 0.0f;
		legend = false;
	}
};

class GraphicEngine {
public:
	static GraphicEngine& get();
	static bool firstParse;
	//inline SDL_Renderer* getRenderer() { return this->_renderer; }
	bool init(window_props editor);

	void update();
	void render();
	void handleEvents();
	void drawGUI();
	void drawShape(Shape& drawable, ImDrawList *draw_list, vis_props props);

	inline bool isRunning() { return this->_is_running; }
	void quit();

	static int col_count;
	static int row_count;
	std::vector<std::string>  _current_draw_list;
	static int parseCount;


private:
	GraphicEngine();
	~GraphicEngine();
	ImGuiIO myio;

	SDL_Window*	   _window;
	vis_props	   _props;
	SDL_GLContext      _gl_context;
	window_props	   _window_props;
	bool		   _is_running;
	int		   _screen_width;
	int		   _screen_height;

	/*
	std::vector <std::string> cmdVec;
	std::vector <std::string> shapeVec;
	std::vector <std::string> refVec;
	std::vector <int>	  offsetXVec;
	std::vector <int>	  offsetYVec;
	*/
};

