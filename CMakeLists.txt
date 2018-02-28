cmake_minimum_required(VERSION 3.6)
project(syspro1)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -ansi")

set(SOURCE_FILES main.c)
add_executable(syspro1 ${SOURCE_FILES})