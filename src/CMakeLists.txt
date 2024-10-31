cmake_minimum_required(VERSION 3.16...3.23)
project(SV-Project)

set(CMAKE_CXX_STANDARD 20)

# Path to third-party packages
set(THIRD_PARTY_PACKAGES ${CMAKE_SOURCE_DIR}/../packages)

# include dir
include_directories(${THIRD_PARTY_PACKAGES}/spot-2.12.1)
include_directories($ENV{HOME}/usr/include)

# link dir
link_directories(${THIRD_PARTY_PACKAGES}/spot-2.12.1/bin)
link_directories($ENV{HOME}/usr/lib)

# Define executables
add_executable(main main.cc)
add_executable(test hello.cc)
add_executable(example examples/object_parser.cc)

# Link Spot library to the targets
target_link_libraries(main PRIVATE spot)
target_link_libraries(test PRIVATE spot)
target_link_libraries(example PRIVATE spot)
