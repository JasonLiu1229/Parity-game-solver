cmake_minimum_required(VERSION 3.16...3.23)
project(SV-Project)

set(CMAKE_CXX_STANDARD 20)

# Path to third-party packages
set(THIRD_PARTY_PACKAGES ${CMAKE_SOURCE_DIR}/../packages)
set(TOOLS tools/parser.cc tools/renderer.cc tools/tester.cc)

# include dir
include_directories(${THIRD_PARTY_PACKAGES}/spot-2.12.1)
include_directories($ENV{HOME}/usr/include)

# link dir
link_directories(${THIRD_PARTY_PACKAGES}/spot-2.12.1/bin)
link_directories($ENV{HOME}/usr/lib)

# Define executables
add_executable(main main.cc ${TOOLS})
add_executable(test hello.cc)
add_executable(example example_code/object_parser.cc)

# Link Spot library to the targets
target_link_libraries(main PRIVATE spot $ENV{HOME}/usr/lib/libspot.so $ENV{HOME}/usr/lib/libbddx.so)
target_link_libraries(test PRIVATE spot)
target_link_libraries(example PRIVATE spot)
