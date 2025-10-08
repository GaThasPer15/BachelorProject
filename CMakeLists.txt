# Define project
cmake_minimum_required(VERSION 3.16)
project(Ray LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include subdirectories
add_subdirectory(Libs)
add_subdirectory(Source)
