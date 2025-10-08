#include "WorldManager.h"
#include "World.h"

void RayEngine::WorldManager::Spawn(Entity *entity) {
    _world->Spawn(entity);
}

RayEngine::Entity *RayEngine::WorldManager::FindEntity(const std::string &name){
    return _world->GetEntity(name);
}