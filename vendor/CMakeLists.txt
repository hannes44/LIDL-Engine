set(SDL_TEST OFF)
add_subdirectory(SDL)
add_subdirectory(glm)
add_subdirectory(spdlog)
add_subdirectory(glew-cmake)
add_subdirectory(yaml-cpp)
add_subdirectory(sol2-3.3.0)

add_subdirectory ( ${CMAKE_SOURCE_DIR}/vendor/imgui-cmake ${CMAKE_CURRENT_BINARY_DIR}/imgui )
set_target_properties(imgui PROPERTIES FOLDER external)

set(imgui_src 
	imgui-cmake/backends/imgui_impl_sdl3.cpp
	imgui-cmake/backends/imgui_impl_sdl3.h
	imgui-cmake/backends/imgui_impl_opengl3.cpp
	imgui-cmake/backends/imgui_impl_opengl3.h
)
add_library(imguilib ${imgui_src})
target_include_directories(imguilib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui-cmake/backends)


add_library(vendor INTERFACE)

target_link_libraries(vendor INTERFACE 
	glm::glm
	spdlog::spdlog
	libglew_static
	yaml-cpp
	sol2::sol2
)
