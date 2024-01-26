set(SDL_TEST OFF)
add_subdirectory(SDL)
add_subdirectory(glm)
add_subdirectory(spdlog)
add_subdirectory(glew-cmake)


add_subdirectory ( ${CMAKE_SOURCE_DIR}/vendor/imgui ${CMAKE_CURRENT_BINARY_DIR}/imgui )
set_target_properties(imgui PROPERTIES FOLDER external)

set(imgui_src 
	imgui/backends/imgui_impl_sdl3.cpp
	imgui/backends/imgui_impl_sdl3.h
	imgui/backends/imgui_impl_opengl3.cpp
	imgui/backends/imgui_impl_opengl3.h
)
add_library(imguilib ${imgui_src})

add_library(vendor INTERFACE)

target_link_libraries(vendor INTERFACE 
	glm::glm
	spdlog::spdlog
	libglew_static
)
