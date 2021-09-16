#include <iostream>
#include <string>

//LOCAL FILES
#include "ActiveShapeBuffer.h"
#include "GraphicEngine.h"
#include "Shape.h"
#include "window_props.h"
#include <fstream>
#include <sstream>

//IMGUI
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "imgui_internal.h"
#include "imgui_stdlib.h"

#include <SDL2/SDL_opengl.h>

bool GraphicEngine::firstParse = false;
int GraphicEngine::col_count   = 0;
int GraphicEngine::row_count   = 0;
int GraphicEngine::parseCount  = 0;
ImVec4 vis_props::defaultColor = ImVec4(0.45f, 255.0f, 0.60f, 255.0f);
bool vis_props::multicolored   = true;

GraphicEngine::GraphicEngine()
{
  _is_running = true;
  _window     = NULL;
}

GraphicEngine::~GraphicEngine() {}

GraphicEngine& GraphicEngine::get()
{
  static GraphicEngine _GraphicEngine;
  return _GraphicEngine;
}

bool GraphicEngine::init(window_props editor)
{
  this->_window_props = editor;

  Uint32 flags     = 0;
  time_t curr_time = NULL;

  if (SDL_Init(SDL_INIT_EVERYTHING) >= 0)
  {
    std::cout << "All SDL subsystems initialized successfully... "
              << "\n";
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
    _window = SDL_CreateWindow(
    _window_props.window_name.c_str(),
    _window_props.window_pos_x,
    _window_props.window_pos_y,
    _window_props.window_width,
    _window_props.window_height,
    editor_window_flags);

    if (_window)
    {
      //std::cout << "running running running" << "\n";
      _gl_context = SDL_GL_CreateContext(_window);
      SDL_GL_MakeCurrent(_window, _gl_context);
      SDL_GL_SetSwapInterval(1); // Enable vsync

      // Setup Dear ImGui context
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGuiIO& io = ImGui::GetIO();
      (void)io;
      myio = io;

      // Setup Dear ImGui style
      ImGui::StyleColorsDark();

      // Setup Platform/Renderer backends
      ImGui_ImplSDL2_InitForOpenGL(_window, _gl_context);
      ImGui_ImplOpenGL3_Init(glsl_version);
    }
    else
    {
      std::cout << "Window creation failed..." << SDL_GetError() << "\n";
      return false;
    }
  }
  else
  {
    std::cout << "Error initializing subsystems... " << SDL_GetError() << "\n";
    return false;
  }

  std::cout << "Init successful..."
            << "\n";
  //Everything initialized successully, start the main loop
  _is_running = true;
  return true;
}

std::string GraphicEngine::constructShapeIDFromShapeName(std::string str)
{
  auto loc_underscore = str.find('_');
  auto extract        = str.substr(loc_underscore + 1, str.size() - loc_underscore + 1);
  return extract;
}

void GraphicEngine::drawGUI()
{
  static int run = 0;

  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();

  //Show input window
  {
    if (GraphicEngine::firstParse)
    {
      static float f       = 0.0f;
      static int row_count = GraphicEngine::row_count;

      ImGui::Begin("Editor Window");

      static ImGuiTableFlags flags = ImGuiTableFlags_Resizable |
                                     ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable |
                                     ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV;

      static std::vector<std::string> cmdVec;
      static std::vector<std::string> shapeVec;
      static std::vector<std::string> refVec;
      static std::vector<int> offsetXVec;
      static std::vector<int> offsetYVec;

      if (ImGui::BeginTable("table1", 5, flags))
      {
        ImGui::TableSetupColumn("cmd");
        ImGui::TableSetupColumn("Shape Name");
        ImGui::TableSetupColumn("Reference Shape");
        ImGui::TableSetupColumn("Pos X");
        ImGui::TableSetupColumn("Pos Y");
        ImGui::TableHeadersRow();

        if (run == 0)
        {
          //populate table fields with existing inputs

          //cmd
          for (auto i = 0; i < row_count; i++)
            cmdVec.push_back("place");

          //shape
          for (auto i = 0; i < row_count; i++)
            shapeVec.push_back(_current_draw_list[i]);

          //ref
          for (auto i = 0; i < row_count; i++)
          {
            auto shape = _current_draw_list[i];
            auto x     = ActiveShapeBuffer::get().shapePlacementMap[shape];
            refVec.push_back(x.get_ref_id());
          }

          //offsetX
          for (auto i = 0; i < row_count; i++)
          {
            auto shape = _current_draw_list[i];
            auto x     = ActiveShapeBuffer::get().shapePlacementMap[shape];
            offsetXVec.push_back(x.getoffsetX());
          }

          //offsetY
          for (auto i = 0; i < row_count; i++)
          {
            auto shape = _current_draw_list[i];
            auto x     = ActiveShapeBuffer::get().shapePlacementMap[shape];
            offsetYVec.push_back(x.getoffsetY());
          }
        }

        if (run > 0)
        {
          for (int row = 0; row < row_count; row++)
          {
            ImGui::TableNextRow();
            for (int column = 0; column < 5; column++)
            {
              ImGui::TableSetColumnIndex(column);
              switch (column)
              {
                case 0:
                  ImGui::PushID(column + row * 10);
                  ImGui::InputText("##label", &cmdVec[row]);
                  ImGui::PopID();
                  break;
                case 1:
                  ImGui::PushID(column + row * 10);
                  ImGui::InputText("##label", &shapeVec[row]);
                  ImGui::PopID();
                  break;
                case 2:
                  ImGui::PushID(column + row * 10);
                  ImGui::InputText("##label", &refVec[row]);
                  ImGui::PopID();
                  break;
                case 3:
                  ImGui::PushID(column + row * 10);
                  ImGui::InputInt("##label", &offsetXVec[row]);
                  ImGui::PopID();
                  break;
                case 4:
                  ImGui::PushID(column + row * 10);
                  ImGui::InputInt("##label", &offsetYVec[row]);
                  ImGui::PopID();
                  break;
              }
            }

            Shape* temp = new Shape;

            temp->setcmd("place");
            temp->setID(shapeVec[row]);
            temp->set_ref_shape_id(refVec[row]);
            temp->set_offset_x(offsetXVec[row]);
            temp->set_offset_y(offsetYVec[row]);

            //keep the color same
            auto x = ActiveShapeBuffer::get().shapePlacementMap[temp->getID()];
            temp->setColor(x.getColor());

            //calculate absolute coordinates
            if (temp->get_ref_id() == "top")
            {
              temp->setposX(temp->getoffsetX());
              temp->setposY(temp->getoffsetY());
            }
            else
            {
              //get coordinates of reference shape
              int ref_x = ActiveShapeBuffer::get().shapePlacementMap[temp->get_ref_id()].getposX();
              int ref_y = ActiveShapeBuffer::get().shapePlacementMap[temp->get_ref_id()].getposY();
              temp->setposX(temp->getoffsetX() + ref_x);
              temp->setposY(temp->getoffsetY() + ref_y);
            }

            //get & set the shape's area from the id-vs-area map
            int area = ActiveShapeBuffer::get().shapeAreaMap[temp->getID()];
            temp->setArea(area);

            //calculate shape's side length from its area
            temp->set_len_in_x(sqrt(temp->getArea()));
            temp->set_len_in_y(sqrt(temp->getArea()));

            //update values;
            Shape& ss = ActiveShapeBuffer::get().shapePlacementMap[temp->getID()];
            ss        = *temp;
            delete temp;
          }
        }
        ImGui::EndTable();
      }
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
      ImGui::End();
    }
  }

  //features
  {
    ImGui::Begin("Display Options");
    ImGui::SliderFloat("Zoom", &this->_props.zoom, 0.0f, 5.0f);
    ImGui::SliderFloat("Move X", &this->_props.moveX, 100.0f, 1000.0f);
    ImGui::SliderFloat("Move Y", &this->_props.moveY, 100.0f, 1000.0f);
    ImGui::SliderFloat("Thickness", &this->_props.thickness, 1.0f, 2.0f);
    ImGui::ShowStyleSelector("Color Theme##Selector");

    //Multicolored
    static int style_idx = -1;
    if (ImGui::Combo("Shape Color##Selector", &style_idx, "MonoChrome\0Multicolored\0"))
    {
      switch (style_idx)
      {
        case 0:
          _props.multicolored = false;
          break;
        case 1:
          _props.multicolored = true;
          break;
      }
    }

    if (!_props.multicolored)
      ImGui::ColorEdit4("Default Color", (float*)&_props.defaultColor);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
  }

  //Visualizer Window
  {
    auto visualizer_window_flags = ImGuiWindowFlags_AlwaysVerticalScrollbar |
                                   ImGuiWindowFlags_AlwaysHorizontalScrollbar;

    ImVec2 scrolling_child_size = ImVec2(0, ImGui::GetFrameHeightWithSpacing() * 7 + 30);

    ImGui::Begin("Visualizer Window", nullptr, visualizer_window_flags);
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImGui::Text("Mouse Right: drag to scroll, click for context menu.");

    //drawShape(draw_list);
    for (auto x : ActiveShapeBuffer::get().shapePlacementMap)
    {
      if (!x.second.getID().empty())
        drawShape(x.second, draw_list, this->_props);
    }

    ImGui::End();
  }

  //Legend
  if (this->_props.legend)
  {
    ImGui::Begin("Legend", &this->_props.legend);
    if (ImGui::Button("Close"))
      this->_props.legend = false;
    ImGui::End();
  }
  run++;
}

void GraphicEngine::drawShape(Shape& drawable, ImDrawList* draw_list, vis_props props)
{
  // vertices
  float offset = 400.0f;
  //float offsetx = 500.0f;
  int lower_left[2]  = { drawable.getposX(), -1 * drawable.getposY() + offset };
  int lower_right[2] = { drawable.getposX() + drawable.get_len_in_x(), -1 * drawable.getposY() + offset };
  int upper_left[2]  = { drawable.getposX(), -1 * (drawable.getposY() + drawable.get_len_in_y()) + offset };
  int upper_right[2] = { drawable.getposX() + drawable.get_len_in_x(),
                         -1 * (drawable.getposY() + drawable.get_len_in_y()) + offset };

  static ImVec2 scrolling(0.0f, 0.0f);
  const ImVec2 p = ImGui::GetCursorScreenPos();

  float x = p.x + props.moveX;
  float y = p.y + props.moveY;

  ImVec4 color = drawable.getColor();

  //ImGui::Text(drawable.getID().c_str());
  draw_list->AddLine(
  ImVec2(x + lower_left[0] * props.zoom, y + lower_left[1] * props.zoom),
  ImVec2(x + upper_left[0] * props.zoom, y + upper_left[1] * props.zoom),

  (_props.multicolored ?
   IM_COL32(color.x, color.y, color.z, color.w) :
   IM_COL32(
   _props.defaultColor.x * props.defaultColor.w,
   _props.defaultColor.y * props.defaultColor.w,
   _props.defaultColor.z * props.defaultColor.w,
   _props.defaultColor.w)),
  props.thickness);

  draw_list->AddLine(
  ImVec2(x + upper_left[0] * props.zoom, y + upper_left[1] * props.zoom),
  ImVec2(x + upper_left[0] * props.zoom, y + upper_left[1] * props.zoom),

  (_props.multicolored ?
   IM_COL32(color.x, color.y, color.z, color.w) :
   IM_COL32(
   _props.defaultColor.x * props.defaultColor.w,
   _props.defaultColor.y * props.defaultColor.w,
   _props.defaultColor.z * props.defaultColor.w,
   _props.defaultColor.w)),
  props.thickness);

  draw_list->AddLine(
  ImVec2(x + upper_left[0] * props.zoom, y + upper_left[1] * props.zoom),
  ImVec2(x + upper_right[0] * props.zoom, y + upper_right[1] * props.zoom),

  (_props.multicolored ?
   IM_COL32(color.x, color.y, color.z, color.w) :
   IM_COL32(
   _props.defaultColor.x * props.defaultColor.w,
   _props.defaultColor.y * props.defaultColor.w,
   _props.defaultColor.z * props.defaultColor.w,
   _props.defaultColor.w)),
  props.thickness);

  draw_list->AddLine(
  ImVec2(x + upper_right[0] * props.zoom, y + upper_right[1] * props.zoom),
  ImVec2(x + lower_right[0] * props.zoom, y + lower_right[1] * props.zoom),

  (_props.multicolored ?
   IM_COL32(color.x, color.y, color.z, color.w) :
   IM_COL32(
   _props.defaultColor.x * props.defaultColor.w,
   _props.defaultColor.y * props.defaultColor.w,
   _props.defaultColor.z * props.defaultColor.w,
   _props.defaultColor.w)),
  props.thickness);

  draw_list->AddLine(
  ImVec2(x + lower_right[0] * props.zoom, y + lower_right[1] * props.zoom),
  ImVec2(x + lower_left[0] * props.zoom, y + lower_left[1] * props.zoom),

  (_props.multicolored ?
   IM_COL32(color.x, color.y, color.z, color.w) :
   IM_COL32(
   _props.defaultColor.x * props.defaultColor.w,
   _props.defaultColor.y * props.defaultColor.w,
   _props.defaultColor.z * props.defaultColor.w,
   _props.defaultColor.w)),
  props.thickness);
}

void GraphicEngine::render()
{
  //Editor Rendering
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
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

void GraphicEngine::quit()
{
  std::cout << "Shutting down GraphicEngine..."
            << "\n";
  _is_running = false;
  _is_running = false;

  /* log output */
  std::fstream file("output.txt", std::ios::out);

  for (int i = 0; i < row_count; i++)
  {
    std::string shape = "shape_" + std::to_string(i);
    auto x            = ActiveShapeBuffer::get().shapePlacementMap[shape];
    file << shape << " : (" << x.getposX() << ", " << x.getposY() << ")\n";
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_GL_DeleteContext(_gl_context);
  SDL_DestroyWindow(_window);
  SDL_Quit();
}

void GraphicEngine::update()
{
}