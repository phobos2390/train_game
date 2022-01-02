cmake_minimum_required(VERSION 3.5.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_helpers/")

set(REPO_NAME "train_game")
set(PROJ_DIR ${CMAKE_CURRENT_SOURCE_DIR})

project(phobos2390-${REPO_NAME})

set(SRC_ROOT_DIR ${PROJ_DIR}/src/${REPO_NAME})
set(INC_ROOT_DIR ${PROJ_DIR}/src)

set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_C_FLAGS "-g -m64")
set(CMAKE_CXX_FLAGS "-g -m64 -Wa,-mbig-obj -std=c++14")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${INC_ROOT_DIR} ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${PROJ_DIR}/catch2/single_include)

set(source_files "")
foreach(source_dir ${SRC_ROOT_DIR})
  message("Finding everything in ${source_dir}")
  file(GLOB dir_src_files ${source_dir}/*.c*)
  set(source_files ${source_files}
                   ${dir_src_files})
endforeach()

message("Building source files ${source_files}")
add_library(game_library SHARED ${source_files})
target_link_libraries(game_library ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

add_executable(train_game ${SRC_ROOT_DIR}/executable/main.c)
target_link_libraries(train_game game_library ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

add_executable(line_segment_demo ${SRC_ROOT_DIR}/executable/line_segments.cpp)
target_link_libraries(line_segment_demo game_library ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

add_executable(intersection_demo ${SRC_ROOT_DIR}/executable/intersection_demo.cpp)
target_link_libraries(intersection_demo game_library ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

add_executable(segment_drawing ${SRC_ROOT_DIR}/executable/segment_drawing.cpp)
target_link_libraries(segment_drawing game_library ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

file(GLOB library_test_files ${SRC_ROOT_DIR}/test/*.c*)
add_executable(library_tests ${library_test_files})
target_link_libraries(library_tests game_library ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
