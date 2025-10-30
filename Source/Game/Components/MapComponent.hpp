#pragma once 

#include <iostream>
#include <ostream>
#include <raylib.h>
#include "../Component.h"
#include "../Entity.h"
#include "../Map/MapFunction.hpp"


namespace GTP{
    class MapComponent : public RayEngine::Component{
        public:
            static const std::string ID;
            MapComponent(int Width, int Heigh, float Scale): mapWidth(Width), mapHeigh(Heigh), HexScale(Scale){
                
                Map::CalcTileCoord(&tilesCoords, iSize, mapWidth, mapHeigh, HexScale);
                Hexagon = LoadTexture("../../Data/Sprites/Hexagon_2.png");
                randColor = LoadShader("../../Data/Shaders/RandColor.vs","../../Data/Shaders/RandColor.fs");
            }
            Component *Clone() const override{
                return new MapComponent(mapWidth, mapHeigh, HexScale);
            }
            const std::string &GetID() const override {return ID;}
            void OnUpdate(const RayEngine::UpdateContext &context) override{

            }
            void OnRender(const RayEngine::RenderContext &context) const override{
                const float textureWidth = (float)Hexagon.width;
                const float textureHeight = (float)Hexagon.height;
                const Rectangle sourceRec = {0.0f, 0.0f, textureWidth, textureHeight};
                const Vector2 spriteSize = {1.0f * HexScale, 1.16f * HexScale};
                const Vector2 origin = {spriteSize.x * 0.5f, spriteSize.y * 0.5f};
                for(int i=0; i<iSize; i++){
                    const Rectangle destRect = {tilesCoords[i].x, tilesCoords[i].y, spriteSize.x, spriteSize.y};
                    SetShaderValue(randColor, GetShaderLocation(randColor, "hexCoord"), &tilesCoords[i], SHADER_UNIFORM_VEC2);
                    BeginShaderMode(randColor);
                    DrawTexturePro(Hexagon, sourceRec, destRect, origin, 0.0f, WHITE);
                    EndShaderMode();
                }
            }
            void OnRenderUI(const RayEngine::RenderUiContext &context)const override{
                
            }
            void OnDestroy() override {
                delete tilesCoords;
                
            }
        private:
            Vector2* tilesCoords;
            Texture2D Hexagon;
            int iSize;
            float HexScale;
            int mapWidth, mapHeigh;
            Shader randColor;
    };
    const std::string MapComponent::ID = "MapComponent";
}