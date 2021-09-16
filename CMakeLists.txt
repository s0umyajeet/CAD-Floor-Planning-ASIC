cmake_minimum_required(VERSION 3.18)
project(visualizer)

add_library(imgui STATIC)

target_sources(imgui PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui/imgui.h"
					 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui/imgui.cpp"
					 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui/imgui_stdlib.h"
					 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui/imgui_stdlib.cpp"
					 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui/imconfig.h"
					 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui/imgui_draw.cpp"
					 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui/imgui_impl_opengl3.h"
					 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui/imgui_impl_opengl3.cpp"
					 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui/imgui_impl_opengl3_loader.h"
					 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui/imgui_impl_sdl.h"
					 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui/imgui_impl_sdl.cpp"
					 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui/imgui_internal.h"
					 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui/imgui_tables.cpp"
					 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui/imgui_widgets.cpp"
					 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui/imstb_rectpack.h"
					 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui/imstb_textedit.h"
					 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui/imstb_truetype.h")
					 
target_include_directories(imgui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui"
								 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/deps/SDL2/include")

target_link_directories(imgui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/deps/SDL2/lib/x86")

target_link_libraries(imgui PUBLIC SDL2.lib
							PUBLIC SDL2main.lib
							PUBLIC opengl32.lib)
							
							
add_executable(visualizer)

target_sources(visualizer PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/ActiveShapeBuffer.h"
						  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/ActiveShapeBuffer.cpp"
						  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/GraphicEngine.h"
						  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/GraphicEngine.cpp"
						  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/iHack-CAD-SDL.cpp"
						  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/InputHandler.h"
						  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/InputHandler.cpp"
						  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/rapidcsv.h"
						  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/Shape.h"
						  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/Shape.cpp"
						  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/window_props.h")
						  
target_include_directories(visualizer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(visualizer PUBLIC imgui)
add_custom_command(TARGET visualizer POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/deps/SDL2/lib/x86/SDL2.dll" "${CMAKE_CURRENT_BINARY_DIR}")