cmake_minimum_required(VERSION 3.2)
project(svge)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# We want c++14 and not gnu++14
set(CMAKE_CXX_EXTENSIONS OFF)

set(SVGE_COMPILE_WITH_SANITIZERS ON CACHE BOOL "If compile with sanitizers")

if(${SVGE_COMPILE_WITH_SANITIZERS})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fsanitize=address -fsanitize=undefined")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=leak")
endif(${SVGE_COMPILE_WITH_SANITIZERS})

find_package(glm REQUIRED)
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Bullet REQUIRED)

# If we use ${SDL2_LIBRARIES} in the target_link_libraries then:
# Target "svge" links to item "-L/usr/lib -lSDL2 " which has leading or trailing whitespace.
#
# That is why we are passing this to the CXX_FLAGS
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SDL2_LIBRARIES}")

include_directories(src ${GLM_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} ${BULLET_INCLUDE_DIRS})

set(SVGE_SRCS
    src/main.cc
    src/core/init.cc
    src/core/opengl_helpers.cc
    src/core/time.cc
    src/game/events.cc
    src/game/game.cc
    src/game/input.cc
    src/game/video.cc
    src/game/scene/gameplay.cc
    src/third_party/glLoadGen/gl_core_3_3.c
)

add_executable(svge ${SVGE_SRCS})

target_link_libraries(svge ${OPENGL_gl_LIBRARY} ode ${BULLET_LIBRARIES})


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
