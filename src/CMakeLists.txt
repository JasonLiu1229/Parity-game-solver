add_subdirectory(cpphoafparser)


find_package(fmt CONFIG)
find_package(spdlog CONFIG)
find_package(CLI11 CONFIG REQUIRED)

add_executable(main main.cpp)

target_link_libraries(
  intro
  PUBLIC project_options project_warnings
  PRIVATE CLI11::CLI11 fmt::fmt spdlog::spdlog
)

target_include_directories(intro PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
