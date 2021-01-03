cmake_minimum_required(VERSION 3.16)
project(shafa C)

set(CMAKE_C_STANDARD 11)

#set(GCC_COVERAGE_COMPILE_FLAGS "-std=gnu11 -Wall -Wextra -pedantic-errors -O0")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

add_executable(shafa main.c a.c a.h b.c b.h c.c d.c dados.h utils.h utils.c dados.c fsize.h a.h c.h d.h)

target_link_libraries(shafa m)