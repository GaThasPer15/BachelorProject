#include "Game.h"
#include "raymath.h"
#include "Game/WorldManager.h"

RayEngine::Game::Game(): lvlManager(this) {
    worldManager = new WorldManager(&world);
}

void RayEngine::Game::Init(const GameSpec &gameSpec){
    m_WindowSize = gameSpec.WindowSize;
    InitWindow(m_WindowSize.x, m_WindowSize.y, gameSpec.WindowTitle.c_str());
    InitAudioDevice();

    //Resizing Screen
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    WindowResized = false;
    WindowSizeIndex = 0;
    
    const Vector2 screenHalfSize = m_WindowSize * 0.5f;
    m_RenderCamera = {
        .offset = screenHalfSize,
        .target = {0.0f, 0.0f},
        .rotation = 0.0f,
        .zoom = 1.0f
    };

    bDebug = false;
    RegisterLevels(levels);
    std::cout << "\nSize of levels in Game.h: " << levels.size() << " levels\n\n";
    if(!levels.empty()){
        LoadLevel(levels.at(0).GetName());
    }
    worldManager = new WorldManager(&world);

    //FPS Data
    ShowFPS = false;
    std::string FPSData = "FPS:\nCPU frame:\nGPU frame:";
    FPSDataSize = 12;
    FramesCount = 0;
    accumulatedTime = 0;
    gpuTime = 0;
    //Coord Data
    coordText = "x: ____ y: ____";

    //GPU time calc
    glGenQueries(1, &gpuQueryID);
}

void RayEngine::Game::Shutdown(){
    CloseAudioDevice();
    CloseWindow();
}

void RayEngine::Game::Run(){
    m_Running = true;
    double deltaTime = 1.0f / 30.0f;
    SetTargetFPS(60);
    while(m_Running && !WindowShouldClose()){
        glBeginQuery(GL_TIME_ELAPSED, gpuQueryID);
        const double frameStartTime = GetTime();
        m_Input.Handle(m_WindowSize);
        UpdateContext updateContext = {
            .bDebug = bDebug,
            .DeltaTime = deltaTime,
            .Input = &m_Input,
            .LvlManager = &lvlManager,
            .worldManager = worldManager,
            .Camera = &m_RenderCamera
        };
        Update(updateContext);
        BeginDrawing();
        glViewport(0, 0, m_WindowSize.x, m_WindowSize.y);
        Color c;
        c.r = 220;
        c.g = 220;
        c.b = 220;
        c.a = 255;
        ClearBackground(c);
        const Vector2 ScreenHalfSize = m_WindowSize * 0.5f;
        m_RenderCamera.offset = ScreenHalfSize;
        BeginMode2D(m_RenderCamera);
        RenderContext renderContext = {
            .bDebug = bDebug,
            .Camera = &m_RenderCamera
        };
        Render(renderContext);
        EndMode2D();
        RenderUiContext renderUIContext = {
            .bDebug = bDebug,
            .Camera = &m_RenderCamera,
            .ScreenSize = m_WindowSize
        };
        RenderUI(renderUIContext);
        EndDrawing();
        // GPU frame calculate
        GLint64 gpuGetTime = 0;
        glGetQueryObjecti64v(gpuQueryID, GL_QUERY_RESULT, &gpuGetTime);
        gpuTime = gpuGetTime / 1e6;
        //New Frame Calc
        const double frameEndTime = GetTime();
        deltaTime = frameEndTime - frameStartTime;
        deltaTime = std::min(deltaTime, 1.0/60.0f);
        glEndQuery(GL_TIME_ELAPSED);
        if(bLevelChangeRequested){
            bLevelChangeRequested = false;
            LoadLevel(levelToLoad);
        }
    }
}

void RayEngine::Game::Update(const UpdateContext &context){
    world.Update(context);
    //F keys binds
    FKeysFunc();
    //Resize window
    if(IsWindowResized()){
        m_WindowSize = {(float)GetScreenWidth(), (float)GetScreenHeight()};
        WindowResized = true;
    }
    //FPS, CPU frame and GPU calculate
    FPSDataCalc(context.DeltaTime);
    Camera2D &cameraHolder = *context.Camera;
    coordText = "x: " + std::to_string(context.Input->GetCursorWorldPosition(cameraHolder).x) + " y: " + std::to_string(context.Input->GetCursorWorldPosition(cameraHolder).y);
}

void RayEngine::Game::Render(const RenderContext &context) const{
    if(context.bDebug){
        DrawLine(-1000.0f, 0.0f, 1000.0f, 0.0f, ColorAlpha(RED, 0.5f));
        DrawLine(0.0, -1000.0f, 0.0f, 1000.0f, ColorAlpha(WHITE, 0.5f));
    }
    world.Render(context);
}

void RayEngine::Game::RenderUI(const RenderUiContext &context) const{
    world.RenderUI(context);
    //Draw FPS, CPU and GPU stats
    if(ShowFPS){
        const Vector2 textSize = MeasureTextEx(
            GetFontDefault(), FPSData.c_str(), FPSDataSize, 2.0f
        );
        DrawText(FPSData.c_str(), m_WindowSize.x - textSize.x + 13.0f, 5.0f, FPSDataSize, BLACK);
    }
    const Vector2 coordSize = MeasureTextEx(
        GetFontDefault(), coordText.c_str(), FPSDataSize, 2.0f);
        DrawText(coordText.c_str(), 5.0f, context.ScreenSize.y - coordSize.y - 13.0f, FPSDataSize, BLACK);
}

void RayEngine::Game::FKeysFunc(){
    if(m_Input.GetKey(KeyCode::F3, InputState::Pressed)){
        ShowFPS = !ShowFPS;
    }
    if(m_Input.GetKey(KeyCode::F4, InputState::Pressed)){
        m_Running = false;
    }
    if(m_Input.GetKey(KeyCode::F5, InputState::Pressed)){
        WindowSizeIndex++;
        if(WindowResized){
            WindowSizeIndex = 0;
        }
        switch(WindowSizeIndex){
            case 0:
                m_WindowSize = {1280, 720};
                SetWindowSize(m_WindowSize.x, m_WindowSize.y);
            break;
            case 1:
                m_WindowSize = {(float)GetMonitorWidth(0), (float)GetMonitorHeight(0)};
                SetWindowSize(m_WindowSize.x, m_WindowSize.y);
            break;
            case 2:
                m_WindowSize = {720, 480};
                SetWindowSize(m_WindowSize.x, m_WindowSize.y);
                WindowSizeIndex = -1;
            break;
        }
        WindowResized = false;
    }
    if(m_Input.GetKey(KeyCode::F10, InputState::Pressed)){
        bDebug = !bDebug;
    }
}

void RayEngine::Game::FPSDataCalc(const double deltaTime){
    if(ShowFPS){
        accumulatedTime += deltaTime;
        FramesCount++;
        if(accumulatedTime > 1.0f){
            FPSData = "FPS: " + std::to_string(FramesCount);
            FPSData+= "\nCPU frame: " + std::to_string((accumulatedTime/FramesCount) * 1000) + "ms";
            FPSData += "\nGPU frame: " + std::to_string(gpuTime) + "ms";
            FramesCount = 0;
            accumulatedTime = 0;
        }
    }
}

void RayEngine::Game::LoadLevel(const std::string &levelName){
    world.Clear();
    const Level *levelToLoad = nullptr;
    for(auto &level : levels){
        if(level.GetName() == levelName){
            levelToLoad = &level;
            break;
        }
    }
    if(!levelToLoad){
        throw std::runtime_error("\nFailed to load level " + levelName + '\n');
    }
    world.LoadLevel(*levelToLoad);
    worldManager = new WorldManager(&world);
}
void RayEngine::Game::RequestLevelChange(const std::string &levelName){
    levelToLoad = levelName;
    bLevelChangeRequested = true;
}

