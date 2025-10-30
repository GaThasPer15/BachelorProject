#pragma once
#include "Game.h"
#include "Game/Components/SpriteComponent.hpp"
#include "Game/Components/PerlinPlateComponent.hpp"
#include "Game/Components/ColliderComponent.hpp"
#include "Game/Components/MapComponent.hpp"
#include "Game/Components/CameraMoveComponent.hpp"

class UserGame : public RayEngine::Game{
    protected:
        void RegisterLevels(std::vector<RayEngine::Level> &outLevels) const override{
            auto level1 = RayEngine::Level::Create("Perlin")
                .WithEntity(
                    RayEngine::Entity("Plate")
                    .WithPosition(Vector2{0.0, 0.0})
                    .WithScale(Vector2{500, 500.0})
                    .WithComponent(new GTP::PerlinPlateComponent())
                    .WithComponent(new GTP::ColliderComponent(physic::CollisionShape::Rectangle, 500.0f, 500.0f))
                );

            auto level2 = RayEngine::Level::Create("Board")
            .WithEntity(
                RayEngine::Entity("Map")
                .WithPosition(Vector2{0.0f, 0.0f})
                .WithRotation(0.0)
                .WithComponent(new GTP::MapComponent(20, 15, 40))
                .WithComponent(new GTP::CameraMoveComponent())
            );
            outLevels.emplace_back(level2);
            outLevels.emplace_back(level1);
        }
};

inline RayEngine::Game *User_CreateGame(){
    const RayEngine::GameSpec gameSpec{
        .WindowSize = {1280, 720},
        .WindowTitle = "MapGenerator"
    };
    RayEngine::Game *game = new UserGame();
    game->Init(gameSpec);
    return game;
}