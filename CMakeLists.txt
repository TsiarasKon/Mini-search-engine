cmake_minimum_required(VERSION 3.6)
project(syspro1)

set(CMAKE_CXX_STANDARD 99)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -std=99 -g3 -Wall")

set(SOURCE_FILES main.c trie.c postinglist.c interface.c util.c pairingheap.c pairingheap.h)
add_executable(syspro1 ${SOURCE_FILES})

target_link_libraries(syspro1 m)