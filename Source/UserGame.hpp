#pragma once
#include "Game.h"
#include "Game/Components/SpriteComponent.hpp"

class UserGame : public RayEngine::Game{
    protected:
        void RegisterLevels(std::vector<RayEngine::Level> &outLevels) const override{
            auto level1 = RayEngine::Level::Create("Level 1");

            outLevels.emplace_back(level1);
        }
};

inline RayEngine::Game *User_CreateGame(){
    const RayEngine::GameSpec gameSpec{
        .WindowSize = {1280, 720},
        .WindowTitle = "Map"
    };
    RayEngine::Game *game = new UserGame();
    game->Init(gameSpec);
    return game;
}