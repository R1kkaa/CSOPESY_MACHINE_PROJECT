set(CMAKE_CXX_STANDARD 20)
project(CSOPESY_OS)
cmake_minimum_required(VERSION 3.31)

add_executable(CSOPESY_OS main.cpp
        src/shell/shell.cpp
        src/shell/shell.h
        src/screen/process.cpp
        src/screen/process.h
        src/screen/screen.cpp
        src/screen/screen.h
        src/shell/util.cpp
        src/shell/util.h)
