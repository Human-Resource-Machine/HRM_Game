cmake_minimum_required(VERSION 3.16.0)
project(game)

include(CTest)
enable_testing()

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/dist/json)
aux_source_directory(./src SrcFiles)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_executable(game ${SrcFiles}
        dist/jsoncpp.cpp)
target_link_libraries(game PRIVATE gdiplus)