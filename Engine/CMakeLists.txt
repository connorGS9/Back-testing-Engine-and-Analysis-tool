cmake_minimum_required(VERSION 3.16)
project(engine)
set(CMAKE_CXX_STANDARD 20)

add_executable(engine src/main.cpp src/Loader.cpp src/Simulator.cpp src/Writer.cpp)