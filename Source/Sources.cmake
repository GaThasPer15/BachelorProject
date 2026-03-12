# Define sources
file(GLOB COMPONENTS_SOURCE
        "${SOURCE_DIR}/Game/Components/*.h"
        "${SOURCE_DIR}/Game/Components/*.cpp"
        "${SOURCE_DIR}/Game/Components/*.hpp"
)
file(GLOB MATH
        "${SOURCE_DIR}/MathLib/*/*.h"
        "${SOURCE_DIR}/MathLib/*/*.hpp"
        "${SOURCE_DIR}/MathLib/*/*.cpp"
)

target_sources(${PROJECT_NAME} PRIVATE
        # ---
        ${MATH}
        ${SOURCE_DIR}/MathLib/MathLib.h

        ${SOURCE_DIR}/Context.h

        ${SOURCE_DIR}/Game/Component.h

        ${COMPONENTS_SOURCE}
        ${SOURCE_DIR}/UserGame.hpp

        ${SOURCE_DIR}/Game.cpp
        ${SOURCE_DIR}/Game.h

        #Graphic
        ${SOURCE_DIR}/Graphic/perlin.cpp
        ${SOURCE_DIR}/Graphic/perlin.h
        ${SOURCE_DIR}/Graphic/perlinParametrs.h
        ${SOURCE_DIR}/Graphic/VoronoiDiagram.h
        ${SOURCE_DIR}/Graphic/VoronoiDiagram.cpp
        ${SOURCE_DIR}/Graphic/GraphicParametrs.h

        #Add animation class
        ${SOURCE_DIR}/Visuals/AnimatedSprite.cpp
        ${SOURCE_DIR}/Visuals/AnimatedSprite.h

        #Add Input
        ${SOURCE_DIR}/Input/Input.cpp
        ${SOURCE_DIR}/Input/Input.h
        ${SOURCE_DIR}/Input/InputCodes.h

        #Add Physic
        ${SOURCE_DIR}/Physics/Body.h
        ${SOURCE_DIR}/Physics/Body.cpp

        #Add Game
        ${SOURCE_DIR}/Game/Entity.cpp
        ${SOURCE_DIR}/Game/Entity.h
        ${SOURCE_DIR}/Game/Level.cpp
        ${SOURCE_DIR}/Game/Level.h
        ${SOURCE_DIR}/Game/LevelManager.cpp
        ${SOURCE_DIR}/Game/LevelManager.h
        ${SOURCE_DIR}/Game/World.cpp
        ${SOURCE_DIR}/Game/World.h
        ${SOURCE_DIR}/Game/WorldManager.cpp
        ${SOURCE_DIR}/Game/WorldManager.h
)

add_custom_command(TARGET ${PROJECT_NAME} 
        POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/DATA"
        "${CMAKE_BINARY_DIR}/DATA"
        )

if(APPLE)
        target_sources(${PROJECT_NAME} PRIVATE
                ${SOURCE_DIR}/Shaders/MetalCompute.mm
        )
endif()