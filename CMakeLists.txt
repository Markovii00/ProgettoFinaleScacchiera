cmake_minimum_required(VERSION 3.21)
project(Progetto_Finale_Scacchiera_Elettronica)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/include)
include_directories(src/include/log)

add_executable(Progetto_Finale_Scacchiera_Elettronica
        src/include/log/logger.cpp
        src/include/log/logger.hpp
        src/include/log/logtoolkit.hpp
        src/include/board.h
        src/include/bot.h
        src/include/chessman.h
        src/board.cpp
        src/bot.cpp
        src/chessman.cpp
        src/scacchiera.cpp)
