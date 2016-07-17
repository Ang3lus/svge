cmake_minimum_required(VERSION 3.2)
project(svge)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fsanitize=address -fsanitize=undefined")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=leak")

include_directories(src)

set(SVGE_SRCS
    src/main.cc
    src/core/time.cc
    src/game/game.cc
    src/game/scene/gameplay.cc
)

add_executable(svge ${SVGE_SRCS})
target_link_libraries(svge)
