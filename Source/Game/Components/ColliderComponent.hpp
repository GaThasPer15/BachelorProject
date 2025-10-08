#pragma once 
#include "../Component.h"
#include "../Entity.h"
#include <vector>
#include "../../Physics/Body.h"

class ColliderComponent : public RayEngine::Component{
    public:
        static const std::string ID;
        ColliderComponent(physic::CollisionShape shape, float value1, float value2 = 0): val1(value1), val2(value2){
            collider = physic::PhysicBody(shape, value1, value2);
        }
        void OnStart() override{
            auto *owner = GetOwner();
            collider.SetPosition(owner->GetPosition());
            collider.SetRotation(owner->GetRotation());
        }
        ColliderComponent *Clone() const override {
            return new ColliderComponent(collider.GetShape(), val1, val2);
        }
        const std::string &GetID() const override {return ID;}
        void OnUpdate(const RayEngine::UpdateContext &context) override{
            RayEngine::Entity *owner = GetOwner();
            collider.SetPosition(owner->GetPosition());
            collider.SetRotation(owner->GetRotation());
            collider.Update(context.DeltaTime);
        }
        physic::PhysicBody *GetCollider(){return &collider;}
    private:
        physic::PhysicBody collider;
        float val1;
        float val2;
};