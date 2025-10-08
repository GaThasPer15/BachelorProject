#pragma once
#include <string>
#include "Entity.h"

namespace RayEngine{
    class World;
    class WorldManager {
        public: 
            explicit WorldManager(World *world): _world(world){}
            void Spawn(Entity *entity);
            Entity *FindEntity(const std::string &name);
        private:
            World *_world = nullptr;

    };
}