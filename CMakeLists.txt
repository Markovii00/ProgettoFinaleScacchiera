cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE release)
endif()

set(CMAKE_CXX_FLAGS, "-02")
set(CMAKE_CXX_FLAGS_RELEASE, "-02")

project(replay)
project(scacchiera)

add_library(board
	    src/include/board.h
	    src/board.cpp)
	
add_library(chessman
	src/include/chessman.h
	src/chessman.cpp)
        
add_library(replayInclude
	src/include/replay.h)
	
                
add_library(logger
	src/include/log/logger.h
        src/include/log/logger.cpp
        src/include/log/logtoolkit.cpp)
        
add_library(bot
	src/include/bot.h
        src/bot.cpp)
        
add_executable(scacchiera
	src/scacchiera.cpp)

add_executable(replay
               src/replay.cpp)
	
target_link_libraries(scacchiera
	logger
	board
	chessman
        bot)

target_link_libraries(replay
	replayInclude
	board
	chessman
        bot)
