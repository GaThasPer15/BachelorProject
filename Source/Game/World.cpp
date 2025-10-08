#include "World.h"
#include <iostream>
#include "Components/ColliderComponent.hpp"

const std::string ColliderComponent::ID = "ColliderComponent";

// RayEngine::World::~World(){
//     for(auto entity : Entities){
//         entity->Destroy();
//         delete entity;
//     }
//     Entities.clear();
//     for(auto collider : colliders){
//         delete collider;
//     }
//     colliders.clear();
// }

void RayEngine::World::LoadLevel(const Level &level){
    Entities.reserve(2000);
    level.LoadInto(*this);
}

void RayEngine::World::Spawn(const Entity &entity){
    Entity* entityCopy = new Entity(entity);
    Entities.push_back(entityCopy);
    Entity *newEntity = Entities.back();
    LastEntityID++;
    newEntity->SetID(LastEntityID);
    newEntity->Start();
}
void RayEngine::World::Spawn(Entity *entity){
    Entities.push_back(entity);
    Entity *newEntity = Entities.back();
    LastEntityID++;
    newEntity->SetID(LastEntityID);
    newEntity->Start();
}

void RayEngine::World::Clear(){
    for(auto &entity : Entities){
        entity->Destroy();
    }
    Entities.clear();
}

void RayEngine::World::Update(const UpdateContext &context){
    for(auto &entity : Entities){
        if(!entity->MarkedDorDestruction()){
            entity->Update(context);
        }
    }
    colliders.clear();
    for(auto &entity : Entities){
        ColliderComponent *colliderComponent = static_cast<ColliderComponent*>(entity->GetComponent("ColliderComponent"));
        if(colliderComponent){
            colliders.push_back(colliderComponent->GetCollider());
        }
    }
    for(auto &collider : colliders){
        bool CollisionInfo = false;
        for(auto &otherCollider : colliders){
            if(collider != otherCollider){
                CollisionInfo = collider->CheckCollision(*otherCollider);
            }
            if(CollisionInfo){
                collider->HandleCollision(*otherCollider);
            }
        }
    }
    for(int i = Entities.size() - 1; i >= 0; i--){
        if(Entities.at(i)->MarkedDorDestruction()){
            delete Entities.at(i);
            Entities.erase(Entities.begin() + i);
        }
    }
}

void RayEngine::World::Render(const RenderContext &context) const{
    for(auto &entity : Entities){
        entity->Render(context);
    }
    if(context.bDebug){
        for(auto &collider : colliders){
            collider->RenderCollider();
        }
    }
}

void RayEngine::World::RenderUI(const RenderUiContext &context) const{
    for(auto &entity : Entities){
        entity->RenderUI(context);
    }
}

RayEngine::Entity *RayEngine::World::GetEntity(const std::string &name) const{
    for(auto *entity : Entities){
        if(entity->GetName() == name){
            return entity;
        }
    }
    return nullptr;
}
