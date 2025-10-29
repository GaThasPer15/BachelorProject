#pragma once
#include "Game.h"
#include "Game/Components/SpriteComponent.hpp"
#include "Game/Components/PerlinPlateComponent.hpp"
#include "Game/Components/ColliderComponent.hpp"

class UserGame : public RayEngine::Game{
    protected:
        void RegisterLevels(std::vector<RayEngine::Level> &outLevels) const override{
            auto level1 = RayEngine::Level::Create("Level 1")
                .WithEntity(
                    RayEngine::Entity("Plate")
                    .WithPosition(Vector2{0.0, 0.0})
                    .WithScale(Vector2{500, 500.0})
                    .WithComponent(new GTP::PerlinPlateComponent())
                    .WithComponent(new GTP::ColliderComponent(physic::CollisionShape::Rectangle, 500.0f, 500.0f))
                );

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