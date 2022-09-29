cmake_minimum_required(VERSION 3.18)
project(pelproject)

set(CMAKE_CXX_STANDARD 14)

add_executable(pelproject main.cpp player.cpp player.hpp play.cpp verify.cpp MOVE_REDO.cpp)
