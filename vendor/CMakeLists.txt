set(SDL_TEST OFF)
add_subdirectory(SDL)
add_subdirectory(glm)

add_library(vendor INTERFACE)

target_link_libraries(vendor INTERFACE SDL3::SDL3)
target_link_libraries(vendor INTERFACE glm::glm)