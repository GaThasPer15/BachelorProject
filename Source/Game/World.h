#pragma once
#include "Entity.h"
#include "Level.h"
#include "../Physics/Body.h"

namespace RayEngine{
    class World{
        public:
            void LoadLevel(const Level &level);
            void Spawn(const Entity &entity);
            void Spawn(Entity *entity);
            void Clear();

            void Update(const UpdateContext &constext);
            void Render(const RenderContext &constext) const;
            void RenderUI(const RenderUiContext &constext) const;

            Entity *GetEntity(const std::string &name) const;
            // ~World();
        private:
            int LastEntityID = 0;
            std::vector<Entity*> Entities;
            std::vector<physic::PhysicBody*> colliders;
    };
}