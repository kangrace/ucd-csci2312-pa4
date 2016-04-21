cmake_minimum_required(VERSION 3.3)
project(ucd-csci2312-pa4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp
        game.cpp Game.h
        piece.cpp Piece.h
        agent.cpp Agent.h
        simple.cpp Simple.h
        strategic.cpp Strategic.h
        resource.cpp Resource.h
        food.cpp Food.h
        advantage.cpp Advantage.h
        GamingTests.cpp GamingTests.h
        ErrorContext.cpp ErrorContext.h
        exceptions.cpp Exceptions.h
        Strategy.h
        defaultAgentStrategy.cpp DefaultAgentStrategy.h
        Gaming.h aggressiveAgentStrategy.cpp AggressiveAgentStrategy.h)
add_executable(ucd-csci2312-pa4 ${SOURCE_FILES})