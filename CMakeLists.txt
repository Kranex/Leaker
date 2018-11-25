cmake_minimum_required(VERSION 3.12)
project(leaker C)
set(CMAKE_C_STANDARD 99)

add_library(leaker STATIC src/leaker.c src/hash_map.c src/linked_list.c)

install(TARGETS leaker DESTINATION lib)
install(FILES src/leaker.h DESTINATION include)