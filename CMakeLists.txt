cmake_minimum_required(VERSION 3.2)
project(svge)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fsanitize=address -fsanitize=undefined")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=leak")

find_package(glm REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

include_directories(src)

set(SVGE_SRCS
    src/main.cc
    src/core/init.cc
    src/core/time.cc
    src/game/game.cc
    src/game/video.cc
    src/game/scene/gameplay.cc
)

add_executable(svge ${SVGE_SRCS})
target_link_libraries(svge ${GLFW_LIBRARIES})

set(SVGE_INSTALL_DIR "${CMAKE_BINARY_DIR}/install")

set(CMAKE_INSTALL_PREFIX ${SVGE_INSTALL_DIR})

install(TARGETS svge  
    RUNTIME DESTINATION bin
)

install(FILES src/sanitizers/leak.supp
    DESTINATION ${SVGE_INSTALL_DIR}/sanitizers
)

install(PROGRAMS src/scripts/run.sh
    DESTINATION ${SVGE_INSTALL_DIR}
)
