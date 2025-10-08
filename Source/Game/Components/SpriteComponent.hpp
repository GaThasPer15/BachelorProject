#pragma once 

#include <iostream>
#include <ostream>
#include <raylib.h>
#include "../Component.h"
#include "../Entity.h"

class SpriteComponent : public RayEngine::Component{
    public:
        static const std::string ID;
        SpriteComponent(const std::string &texturePath, const Color &color = WHITE) : color(color){
            TexturePath = texturePath;
            SpriteTexture = LoadTexture(texturePath.c_str());
            if(!IsTextureValid(SpriteTexture)){
                std::cout << "Failed to load a texture " << texturePath << '\n';
            }
        }
        Component *Clone() const override{
            return new SpriteComponent(TexturePath, color);
        }
        const std::string &GetID() const override {return ID;}
        void OnRender(const RayEngine::RenderContext &context) const override{
            RayEngine::Entity *owner = GetOwner();
            if(owner == nullptr){
                std::cout << "\nOwner is nullptr\n\n";
            }
            float rotation = owner->GetRotation();
            Vector2 position = owner->GetPosition();
            Vector2 scale = owner->GetScale();
            const float textureWidth = (float)SpriteTexture.width;
            const float textureHeight = (float)SpriteTexture.height;
            const Rectangle sourceRect = {0.0f, 0.0f, textureWidth, textureHeight};
            const Vector2 spriteSize = {1.0f * scale.x, 1.0f * scale.y};
            const Rectangle destRect = {position.x, position.y, spriteSize.x, spriteSize.y};
            const Vector2 origin = {spriteSize.x * 0.5f, spriteSize.y * 0.5f};
            DrawTexturePro(SpriteTexture, sourceRect, destRect, origin, rotation, color);
        }
    private:
        std::string TexturePath;
        Texture2D SpriteTexture;
        Color color = WHITE;
};
const std::string SpriteComponent::ID = "SpriteComponent";