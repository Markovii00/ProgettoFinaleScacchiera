cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE release)
endif()

project(scacchiera)
project(replay)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS, "-02")
set(CMAKE_CXX_FLAGS_DEBUG, "-02")
set(CMAKE_CXX_FLAGS_RELEASE, "-02")

include_directories(include)
include_directories(include/log)

add_library(Board
        include/board.h
        src/board.cpp
)

add_library(Chessman
        include/chessman.h
        src/chessman.cpp
)

add_library(Bot
        include/bot.h
        src/bot.cpp
)

add_library(Logger
        include/log/logger.cpp
        include/log/logger.h
        include/log/logtoolkit.h
        include/replay.h
)

add_executable(replay
        src/replay.cpp
)

add_executable(scacchiera
        src/scacchiera.cpp
)

target_link_libraries(scacchiera Logger Board Bot Chessman)
target_link_libraries(replay Logger Board Bot Chessman)