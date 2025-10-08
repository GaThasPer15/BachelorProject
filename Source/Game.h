#pragma once

#if defined(__APPLE__)
    #include <OpenGL/gl3.h>
#else
    #include <GL/glew.h>
#endif
#include "raylib.h"
#include "Input/Input.h"

#include "Context.h"
#include "Game/Level.h"
#include "Game/World.h"

#include <iostream>

namespace RayEngine{

    struct GameSpec{
        std::string WindowTitle;
        Vector2 WindowSize;
    };
    class Game{
        public:
            Game();
            virtual ~Game(){}
            void Init(const GameSpec &gameSpec);
            void Run();
            void Shutdown();
            void LoadLevel(const std::string &levelName);
            void RequestLevelChange(const std::string &levelName);

        private:
            void Update(const UpdateContext &context);
            void Render(const RenderContext &context) const;
            void RenderUI(const RenderUiContext &constext) const;

        protected:
            virtual void RegisterLevels(std::vector<Level> &outLevels) const = 0;

        private:
            bool m_Running;
            Vector2 m_WindowSize;
            Camera2D m_RenderCamera;
            Input m_Input;
            bool bDebug;
            World world;
            std::vector<Level> levels;
            LevelManager lvlManager;
            WorldManager *worldManager;
            bool bLevelChangeRequested;
            std::string levelToLoad;
            Shader m_GrayscaleShader;
            Shader m_SwimmerShader;

        private:
            bool ShowFPS;
            std::string FPSData;
            int FPSDataSize;
            int FramesCount;
            double accumulatedTime;
            double gpuTime;
            GLuint gpuQueryID;
            std::string coordText;

        //Methods
        private:
            void FKeysFunc();
            void FPSDataCalc(const double deltaTime);

        private:
            bool WindowResized;
            int WindowSizeIndex;
            Vector2 m_cursorWorldPosition;
            Vector2 m_CursorPosition;
    };
}

RayEngine::Game *User_CreateGame();