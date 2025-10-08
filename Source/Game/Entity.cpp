#include "Entity.h"
#include <iostream>
#include <ostream>
#include <cmath>

RayEngine::Entity::Entity(const Entity &other){
    *this = other;
}

RayEngine::Entity &RayEngine::Entity::operator=(const Entity &other){
    ID =other.ID;
    Name = other.Name;
    vPosition = other.vPosition;
    fRotation = other.fRotation;
    vScale = other.vScale;
    bDestroyed = other.bDestroyed;
    Components.clear();
    for(auto *component : other.Components){
        AddComponent(component->Clone());
    }
    return *this;
}

RayEngine::Entity &RayEngine::Entity::WithPosition(const Vector2 &position){
    vPosition = position;
    return *this;
}

RayEngine::Entity &RayEngine::Entity::WithRotation(const float &rotation){
    fRotation = rotation;
    return *this;
}

RayEngine::Entity &RayEngine::Entity::WithScale(const Vector2 &scale){
    vScale = scale;
    return *this;
}

RayEngine::Entity &RayEngine::Entity::WithComponent(Component *component){
    AddComponent(component);
    return *this;
}

void RayEngine::Entity::Start(){
    for(auto *component : Components){
        component->OnStart();
    }
}

void RayEngine::Entity::Update(const UpdateContext &context){
    for(auto *component : Components){
        component->OnUpdate(context);
    }
    for(const std::string &id : ComponentsIDsToRemove){
        for(int i=0; i < Components.size(); i++){
            if(Components.at(i)->GetID() == id){
                Components.at(i)->OnDestroy();
                Components.erase(Components.begin() + i);
                break;
            }
        }
    }
    ComponentsIDsToRemove.clear();
}

void RayEngine::Entity::Render(const RenderContext &context) const{
    for(auto *component : Components){
        component->OnRender(context);
    }
}

void RayEngine::Entity::RenderUI(const RenderUiContext &context) const{
    if(context.bDebug){
        const Vector2 screenPos = GetWorldToScreen2D(Vector2{vPosition.x, vPosition.y}, *context.Camera);
        DrawText(Name.c_str(), screenPos.x, screenPos.y, 16, ORANGE);
    }
    for(auto *component : Components){
        component->OnRenderUI(context);
    }
}

void RayEngine::Entity::Destroy(){
    for(auto *component : Components){
        component->OnDestroy();
    }
    bDestroyed = true;
}

void RayEngine::Entity::AddComponent(Component *component){
    const std::string &id = component->GetID();
    for(const auto *existiongComponent : Components){
        if(existiongComponent->GetID() == id){
            std::cout << "Entity " << Name << " already has a component with ID = " << id << '\n';
            return;
        }
    }
    component->Init(this);
    Components.emplace_back(component);
}

void RayEngine::Entity::RemoveComponent(const std::string &id){
    ComponentsIDsToRemove.push_back(id);
}

RayEngine::Component *RayEngine::Entity::GetComponent(const std::string &id) const{
    for(auto *component : Components){
        if(component->GetID() == id){
            return component;
        }
    }
    return nullptr;
}

