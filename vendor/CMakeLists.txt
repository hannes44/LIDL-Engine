set(SDL_TEST OFF)
add_subdirectory(SDL)
add_subdirectory(glm)
add_subdirectory(spdlog)
add_subdirectory(uuid_v4)

add_library(vendor INTERFACE)

target_link_libraries(vendor INTERFACE SDL3::SDL3)
target_link_libraries(vendor INTERFACE glm::glm)
target_link_libraries(vendor INTERFACE spdlog::spdlog)
target_link_libraries(vendor INTERFACE uuid_v4::uuid_v4)
