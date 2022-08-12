cmake_minimum_required(VERSION 3.23)

project(zxvm)

add_executable(zsvm src/chunk.c src/debug.c src/main.c src/memory.c src/value.c)
