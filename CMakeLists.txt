cmake_minimum_required(VERSION 3.2)
project(svge)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src)

set(SVGE_SRCS
    src/main.cc
    src/core/time.cc
    src/game/game.cc
)

add_executable(svge ${SVGE_SRCS})
target_link_libraries(svge log4cplus)
