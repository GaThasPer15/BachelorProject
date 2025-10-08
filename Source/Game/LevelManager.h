#pragma once
#include <string>

namespace RayEngine{
    class Game;
    class LevelManager{
        public:
            explicit LevelManager(Game *game): _Game(game) {}
            void ChangeLevel(const std::string &name) const;
        private:
            Game *_Game = nullptr;
    };
}