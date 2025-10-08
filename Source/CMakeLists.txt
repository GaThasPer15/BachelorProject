#Set Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Create executable
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(${PROJECT_NAME} ${SOURCE_DIR}/Main.cpp)

include(${CMAKE_CURRENT_SOURCE_DIR}/Sources.cmake)

# Add libraries include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${lua_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${sol2_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${box2d_SOURCE_DIR}/include)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
        raylib
        box2d
        Lua_static
        sol2
)

# Sleep OpenGL on apple warnings (Apple thiks its outdated)
add_definitions(-DGL_SILENCE_DEPRECATION)