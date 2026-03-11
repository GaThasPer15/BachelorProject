# Define project
cmake_minimum_required(VERSION 3.16)
project(Engine LANGUAGES C CXX)

if(APPLE)

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w")
    set(CMAKE_OBJC_FLAGS "${CMAKE_OBJC_FLAGS} -w")
endif()


# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include subdirectories
add_subdirectory(Libs)
add_subdirectory(Source)
