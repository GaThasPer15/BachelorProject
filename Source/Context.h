 #pragma once

 #include <raylib.h>
 #include "Game/LevelManager.h"
 #include "Input/Input.h"

 namespace RayEngine{
    class Game;
    class WorldManager;
    struct UpdateContext{
        bool bDebug;
        double DeltaTime;
        Input *Input;
        LevelManager *LvlManager;
        WorldManager *worldManager;
        Camera2D *Camera;
    };
    struct RenderContext{
        bool bDebug;
        Camera2D *Camera;
    };
    struct RenderUiContext{
        bool bDebug;
        Vector2 ScreenSize;
        Camera2D *Camera;
    };
 }