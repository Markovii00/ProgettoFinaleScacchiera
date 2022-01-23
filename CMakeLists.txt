cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE release)
endif()

set(CMAKE_CXX_FLAGS, "-02")
set(CMAKE_CXX_FLAGS_RELEASE, "-02")

project(replay)
project(scacchiera)

include_directories(src
                    include
                    log)

add_library(board
	    board.h
	    board.cpp)
	
add_library(chessman
	chessman.h
	chessman.cpp)
        
add_library(replayInclude
	replay.h)
	
                
add_library(logger
	logger.h
        logger.cpp
        loggertoolkit.h)
        
add_library(bot
	bot.h
        bot.cpp)
        
add_executable(scacchiera
	scacchiera.cpp)

add_executable(replay
               replay.cpp)
	
target_link_libraries(scacchiera
	logger
	board
	chessman
        bot)

target_link_libraries(replayInclude
	board
	chessman
        bot)
