cmake_minimum_required(VERSION 3.16)
project(shafa C)

set(CMAKE_C_STANDARD 11)

set(GCC_COVERAGE_COMPILE_FLAGS "-std=gnu11 -Wall -Wextra -pedantic-errors -O0")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

add_executable(shafa main.c ModuloA/a.c ModuloA/a.h ModuloB/b.c ModuloB/b.h ModuloC/c.c ModuloD/d.c Common/dados.h Common/utils.h Common/utils.c Common/dados.c Common/debug.h Common/debug.c Common/fsize.h ModuloA/a.h ModuloC/c.h ModuloD/d.h)

target_link_libraries(shafa m)