set(CMAKE_CXX_STANDARD 20)
cmake_minimum_required(VERSION 3.31)
project(CSOPESY_OS)

set(SOURCE_FILES main.cpp)
add_executable(CSOPESY_OS main.cpp
        src/shell/shell.cpp
        src/shell/shell.h
        src/screen/process.cpp
        src/screen/process.h
        src/shell/util.cpp
        src/shell/util.h
        src/screen/ICommand.cpp
        src/screen/ICommand.h
        src/screen/Thread.cpp
        src/screen/Thread.h
        src/screen/CPUCore.cpp
        src/screen/CPUCore.h
        src/screen/Scheduler.cpp
        src/screen/Scheduler.h
        src/screen/PrintCommand.cpp
        src/screen/PrintCommand.h
        src/screen/SleepCommand.cpp
        src/screen/SleepCommand.h
        src/screen/ForCommand.cpp
        src/screen/ForCommand.h
        src/screen/generateprocess.cpp
        src/screen/generateprocess.h
        src/screen/DeclareCommand.cpp
        src/screen/DeclareCommand.h
        src/screen/AddCommand.cpp
        src/screen/AddCommand.h
        src/screen/SubCommand.cpp
        src/screen/SubCommand.h
        src/screen/Memory.cpp
        src/screen/Memory.h)
