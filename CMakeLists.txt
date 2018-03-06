cmake_minimum_required(VERSION 3.6)
project(syspro1)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -ansi -g3")

set(SOURCE_FILES main.c trie.c postinglist.c interface.c util.c)
add_executable(syspro1 ${SOURCE_FILES})