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
	float zoom = 1.0f;
	float moveX = 0.0f;
	float moveY = 0.0f;
	bool legend = false;
	bool show_different_colors = false;
	bool reset = false;

	void reset_props() {
		zoom = 1.0f;
		moveX = 0.0f;
		moveY = 0.0f;
		legend = false;
		show_different_colors = false;
	}	
};

class GraphicEngine {
public:
	static GraphicEngine& get();
	static bool firstParse;
	inline SDL_Renderer* getRenderer() { return this->_renderer; }
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

private:
	GraphicEngine();
	~GraphicEngine();
	ImGuiIO myio;

	SDL_Window*	_window;
	SDL_Surface*	_surface;
	SDL_Texture*	_texture;
	SDL_Renderer*	_renderer;
	SDL_GLContext   _gl_context;
	window_props	_window_props;
	bool		_is_running;
	int		_screen_width;
	int		_screen_height;
};