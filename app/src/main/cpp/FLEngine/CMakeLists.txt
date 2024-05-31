cmake_minimum_required(VERSION 3.10.2)
project(FLEngine VERSION 1.0.0)
add_subdirectory(Audio)
add_subdirectory(Graphics)
add_subdirectory(Physics)
add_subdirectory(Networking)
add_subdirectory(Includes)
add_library(FLEngine SHARED FLCore.cpp)

