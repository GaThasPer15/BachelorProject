# Include FetchContent module
include(FetchContent)

set(GRAPHICS_API_OPENGL_43 ON CACHE BOOL "" FORCE)

# Include Metal 
if(APPLE)      
        find_library(METAL_FRAMEWORK Metal)
        find_library(FOUNDATION_FRAMEWORK Foundation)
        find_library(COREGRAPHICS_FRAMEWORK CoreGraphics REQUIRED)

        set(APPLE_FRAMEWORKS
                ${METAL_FRAMEWORK}
                ${FOUNDATION_FRAMEWORK}
                ${COREGRAPHICS_FRAMEWORK}
                PARENT_SCOPE
        )
endif()
# Include Raylib
FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG master
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)
