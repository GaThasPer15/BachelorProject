#pragma once 

#include <raylib.h>
#include <string>
#include "../Context.h"
#include <vector>
#include "Component.h"

namespace RayEngine{
    class Entity{
        public:
            explicit Entity(const std::string &name):Name(name){

            }
            Entity(const Entity &other);
            Entity &operator=(const Entity &other);

            int GetID() {return ID;}
            void SetID(const int id) {ID = id;}
            bool MarkedDorDestruction() const {return bDestroyed;}
            const Vector2 &GetPosition() const {return vPosition;}
            void SetPosition(const Vector2 &position) {vPosition = position;}
            const float &GetRotation() {return fRotation;}
            void SetRotation(const float rotation) {fRotation = rotation;}
            const Vector2 &GetScale() const {return vScale;}
            void SetScale(const Vector2 &scale) {vScale = scale;}
            const std::string &GetName() {return Name;}

            Entity &WithPosition(const Vector2 &position);
            Entity &WithRotation(const float &rotation);
            Entity &WithScale(const Vector2 &scale);
            Entity &WithComponent(Component *component);

            void Start();
            void Update(const UpdateContext &context);
            void Render(const RenderContext &context) const;
            void RenderUI(const RenderUiContext &context) const;
            void Destroy();

            ~Entity() {}
            void AddComponent(Component *component);
            void RemoveComponent(const std::string &id);
            Component *GetComponent(const std::string &id) const;
        private:
            int ID = -1;
            std::string Name;
            bool bDestroyed = false;
            Vector2 vPosition = Vector2{0.0f, 0.0f};
            float fRotation = 0.0f;
            Vector2 vScale = Vector2{1.0f, 1.0f};
            std::vector<Component *> Components;
            std::vector<std::string> ComponentsIDsToRemove;
    };
}