#include <iostream>
#include <string>

//LOCAL FILES
#include "GraphicEngine.h"
#include "ActiveShapeBuffer.h"
#include "Shape.h"
#include "window_props.h"
#include <fstream>
#include <sstream>

//IMGUI
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h"

#include <SDL_opengl.h>

bool GraphicEngine::firstParse = false;
int GraphicEngine::col_count = 0;
int GraphicEngine::row_count = 0;

GraphicEngine::GraphicEngine() {
	_is_running = true;
	_is_running = true;
	_window = NULL;
	_surface  = NULL;
	_texture  = NULL;
	_renderer = NULL;
}

GraphicEngine::~GraphicEngine() {}

GraphicEngine& GraphicEngine::get()
{
	static GraphicEngine _GraphicEngine;
	return _GraphicEngine;
}

bool GraphicEngine::init(window_props editor) {
	this->_window_props = editor;


	Uint32 flags = 0;
	time_t curr_time = NULL;


	if (SDL_Init(SDL_INIT_EVERYTHING) >= 0) {
		std::cout << "All SDL subsystems initialized successfully... " << std::endl;
		SDL_WindowFlags editor_window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
		

#if defined(IMGUI_IMPL_OPENGL_ES2)
		// GL ES 2.0 + GLSL 100
		const char* glsl_version = "#version 100";
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
		// GL 3.2 Core + GLSL 150
		const char* glsl_version = "#version 150";
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
		// GL 3.0 + GLSL 130
		const char* glsl_version = "#version 130";
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#endif
		
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

		//create GUI window
		_window = SDL_CreateWindow (
			_window_props.window_name.c_str(),
			_window_props.window_pos_x,
			_window_props.window_pos_y,
			_window_props.window_width,
			_window_props.window_height,
			editor_window_flags
		);

		if (_window) {
			std::cout << "running running running" << std::endl;
			_gl_context = SDL_GL_CreateContext(_window);
			SDL_GL_MakeCurrent(_window, _gl_context);
			SDL_GL_SetSwapInterval(1); // Enable vsync

			// Setup Dear ImGui context
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			myio = io;

			// Setup Dear ImGui style
			ImGui::StyleColorsDark();

			// Setup Platform/Renderer backends
			ImGui_ImplSDL2_InitForOpenGL(_window, _gl_context);
			ImGui_ImplOpenGL3_Init(glsl_version);
		}
		else {
			std::cout << "Window creation failed..." << SDL_GetError() << std::endl;
			return false;
		}
	}
	else {
		std::cout << "Error initializing subsystems... " << SDL_GetError() << std::endl;
		return false;
	}

	std::cout << "Init successful..." << std::endl;
	//Everything initialized successully, start the main loop
	_is_running = true;
	return true;
}

void GraphicEngine::drawGUI() {
	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	//Show input window
	{
		static float f = 0.0f;
		static int row_count = GraphicEngine::row_count;

		ImGui::Begin("Editor Window"); 

		static ImGuiTableFlags flags = ImGuiTableFlags_Resizable | 
			ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | 
			ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV;
		

		//if (GraphicEngine::firstParse) {

			if (ImGui::BeginTable("table1", 5, flags))
			{
				ImGui::TableSetupColumn("cmd");
				ImGui::TableSetupColumn("Shape Name");
				ImGui::TableSetupColumn("Reference Shape");
				ImGui::TableSetupColumn("Pos X");
				ImGui::TableSetupColumn("Pos Y");
				ImGui::TableHeadersRow();
				
				std::string cmd;
				std::string shape_name;
				std::string ref_shape;
				int pos_x = 0, pos_y = 0;

				for (int row = 0; row < row_count; row++)
				{
					ImGui::TableNextRow();
					for (int column = 0; column < 5; column++)
					{
						//std::cout << column + row * 10 << std::endl;
						ImGui::TableSetColumnIndex(column);
						switch (column) {
						case 0:
							ImGui::PushID(column + row * 10);
							ImGui::InputText("##label", &cmd);
							ImGui::PopID();
							break;
						case 1:
							ImGui::PushID(column + row * 10);
							ImGui::InputText("##label", &shape_name);
							ImGui::PopID();
							break;
						case 2:
							ImGui::PushID(column + row * 10);
							ImGui::InputText("##label", &ref_shape);
							ImGui::PopID();
							break;
						case 3:
							ImGui::PushID(column + row * 10);
							ImGui::InputInt("##label", &pos_x);
							ImGui::PopID();
							break;
						case 4:
							ImGui::PushID(column + row * 10);
							ImGui::InputInt("##label", &pos_y);
							ImGui::PopID();
							break;
						}
					}

					rapidcsv::Document doc = ActiveShapeBuffer::get().placementFile;
					/*
					std::cout << "Writing the values: "
						<< cmd << " " << shape_name << " " << ref_shape << " " << pos_x << " " << pos_y << std::endl;
					doc.SetRow<std::string>(row, std::vector<std::string>{cmd, shape_name, ref_shape, 
											std::to_string(pos_x), std::to_string(pos_y)});
					doc.Save();*/
				}
				ImGui::EndTable();
			//}

			if (ImGui::Button("Add 5 more"))
				row_count += 5;
		}

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	//features
	{
		static float zoom = 0.0f;
		static float moveX = 0.0f;
		static float moveY = 0.0f;
		static int counter = 0;

		ImGui::Begin("Display options");                          
		ImGui::SliderFloat("Zoom", &zoom, 0.0f, 1.0f);            
		ImGui::ColorEdit3("BG color", (float*)&clear_color); 
		/*
		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);
		*/

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	{
		auto visualizer_window_flags = ImGuiWindowFlags_AlwaysVerticalScrollbar | 
				ImGuiWindowFlags_AlwaysHorizontalScrollbar;

		ImVec2 scrolling_child_size = ImVec2(0, ImGui::GetFrameHeightWithSpacing() * 7 + 30);
		ImGui::Begin("Visualizer Window", nullptr, visualizer_window_flags);
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		
		//drawShape(draw_list);
		for (auto x : ActiveShapeBuffer::get().shapePlacementMap) {
			drawShape(x.second, draw_list);
		}
	
		ImGui::End();
	}
}


void GraphicEngine::drawShape(Shape& drawable, ImDrawList *draw_list, vis_props props) {
	
	//Debug
	std::stringstream outputss;
	std::fstream output_file("output.txt", std::ios::app);
	outputss << "Currently drawing: " << drawable.getID() << std::endl;
	drawable.show_data(outputss);
	output_file << outputss.str();
	std::cout << outputss.str() << std::endl;
	

	// vertices
	float offset = 500.0f;
	//float offsetx = 500.0f;
	int lower_left[2] = { drawable.getposX(), -1 * drawable.getposY() + offset };
	int lower_right[2] = { drawable.getposX() + drawable.get_len_in_x(), -1 * drawable.getposY() + offset };
	int upper_left[2] = { drawable.getposX(),  -1 * (drawable.getposY() + drawable.get_len_in_y()) + offset };
	int upper_right[2] = { drawable.getposX() + drawable.get_len_in_x(),
						   -1 * (drawable.getposY() + drawable.get_len_in_y()) + offset };

	const ImVec2 p = ImGui::GetCursorScreenPos();
	float x = p.x + props.moveX;
	float y = p.y + props.moveY;

	draw_list->AddLine(ImVec2(x + lower_left[0] * props.zoom, y + lower_left[1] * props.zoom), 
			ImVec2(x + upper_left[0] * props.zoom, y + upper_left[1] * props.zoom), IM_COL32(0, 255, 0, 255), 0.0f);
	draw_list->AddLine(ImVec2(x + upper_left[0] * props.zoom, y + upper_left[1] * props.zoom), 
			ImVec2(x + upper_right[0] * props.zoom, y + upper_right[1] * props.zoom), IM_COL32(0, 255, 0, 255), 0.0f);
	draw_list->AddLine(ImVec2(x + upper_left[0] * props.zoom, y + upper_left[1] * props.zoom), 
			ImVec2(x + upper_right[0] * props.zoom, y + upper_right[1] * props.zoom), IM_COL32(0, 255, 0, 255), 0.0f);
	draw_list->AddLine(ImVec2(x + upper_right[0] * props.zoom, y + upper_right[1] * props.zoom), 
			ImVec2(x + lower_right[0] * props.zoom, y + lower_right[1] * props.zoom), IM_COL32(0, 255, 0, 255), 0.0f);
	draw_list->AddLine(ImVec2(x + lower_right[0] * props.zoom, y + lower_right[1] * props.zoom), 
			ImVec2(x + lower_left[0] * props.zoom, y + lower_left[1] * props.zoom), IM_COL32(0, 255, 0, 255), 0.0f);

}

void GraphicEngine::render()
{
	//Editor Rendering
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 0.00f);
	ImGui::Render();
	glViewport(0, 0, (int)myio.DisplaySize.x, (int)myio.DisplaySize.y);
	glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(_window);
}

void GraphicEngine::handleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		if (event.type == SDL_QUIT)
			_is_running = false;
		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(_window))
			_is_running = false;
	}
}

void GraphicEngine::quit() {
	std::cout << "Shutting down GraphicEngine..." << std::endl;
	_is_running= false;
	_is_running = false;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	
	SDL_GL_DeleteContext(_gl_context);
	//SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

void GraphicEngine::update()
{
}