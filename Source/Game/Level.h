#pragma once 
#include <string>
#include <vector>
#include "Entity.h"

namespace RayEngine{
    class World;
    class Level{
        protected:
            explicit Level(const std::string &name): Name(name) {}
        private:
            std::string Name;
            std::vector<Entity> Entities;
        public:
            const std::string &GetName() const {return Name;}
            static Level Create(const std::string &name);
            Level &WithEntity(const Entity &entity);
            void LoadInto(World &world) const;
    };
}