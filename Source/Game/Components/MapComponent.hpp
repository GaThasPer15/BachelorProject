#pragma once 

#include <iostream>
#include <ostream>
#include <raylib.h>
#include "../Component.h"
#include "../Entity.h"
#include "../Map/MapFunction.hpp"
#include "../../Graphic/perlin.h"


namespace GTP{
    class MapComponent : public RayEngine::Component{
        public:
            static const std::string ID;
            MapComponent(int Width, int Heigh, float Scale): mapWidth(Width), mapHeigh(Heigh), HexScale(Scale){
                controlState = 0;
                maxValue = 7;
                border = 0.5f;
                mapUpdate = false;
                Map::CalcTileCoord(&tilesCoords, iSize, mapWidth, mapHeigh, HexScale);
                
                Hexagon.push_back(LoadTexture("../Data/Sprites/hexagon_water.png"));
                Hexagon.push_back(LoadTexture("../Data/Sprites/hexagon_plane.png"));
                Hexagon.push_back(LoadTexture("../Data/Sprites/hexagon_mountain.png"));
                Hexagon.push_back(LoadTexture("../Data/Sprites/hexagon_hill.png"));
                Hexagon.push_back(LoadTexture("../Data/Sprites/hexagon_desert.png"));
                
                Perlin::TextureSize texSize {1024, 1024};
                generator = new Perlin::TextureGenerator(texSize);
                
                Image img = GenImageColor(1024, 1024, BLANK);
                tex = LoadTextureFromImage(img);
                UnloadImage(img);

                data.frequency=8.0f;
                generator->Generate(tex, data);
                
                size_t size = mapHeigh*mapWidth;
                type = new int[size];
                GenMap();

            }
            Component *Clone() const override{
                return new MapComponent(mapWidth, mapHeigh, HexScale);
            }
            const std::string &GetID() const override {return ID;}
            void OnUpdate(const RayEngine::UpdateContext &context) override{
                if(context.Input->GetKey(RayEngine::KeyCode::Up, RayEngine::InputState::Pressed)){
                    controlState==0?:controlState--;
                }
                else if(context.Input->GetKey(RayEngine::KeyCode::Down, RayEngine::InputState::Pressed)){
                    controlState++;
                    controlState%=maxValue;
                }
                switch(controlState){
                    case 0:   
                        if(context.Input->GetKey(RayEngine::KeyCode::Right, RayEngine::InputState::Held)){
                            data.frequency +=0.1;
                            mapUpdate = true;
                        }
                        else if(context.Input->GetKey(RayEngine::KeyCode::Left, RayEngine::InputState::Held)){
                            data.frequency -=0.1;
                            mapUpdate = true;
                        }
                    break;
                    case 1:
                        if(context.Input->GetKey(RayEngine::KeyCode::Right, RayEngine::InputState::Held)){
                            data.amplitude += 0.01;
                            mapUpdate = true;
                        }
                        else if(context.Input->GetKey(RayEngine::KeyCode::Left, RayEngine::InputState::Held)){
                            data.amplitude -= 0.01;
                            mapUpdate = true;
                        }
                    break;
                    case 2:
                        if(context.Input->GetKey(RayEngine::KeyCode::L, RayEngine::InputState::Held)){
                            data.seed.x += 0.025;
                            mapUpdate = true;
                        }
                        else if(context.Input->GetKey(RayEngine::KeyCode::J, RayEngine::InputState::Held)){
                            data.seed.x -= 0.025;
                            mapUpdate = true;
                        }
                        if(context.Input->GetKey(RayEngine::KeyCode::I, RayEngine::InputState::Held)){
                            data.seed.y -= 0.025;
                            mapUpdate = true;
                        }
                        else if(context.Input->GetKey(RayEngine::KeyCode::K, RayEngine::InputState::Held)){
                            data.seed.y += 0.025;
                            mapUpdate = true;
                        }
                    break;
                    case 3:
                        if(context.Input->GetKey(RayEngine::KeyCode::Right, RayEngine::InputState::Pressed)){
                            data.octave += 1;
                            mapUpdate = true;
                        }
                        else if(context.Input->GetKey(RayEngine::KeyCode::Left, RayEngine::InputState::Pressed)){
                            data.octave-= 1;
                            mapUpdate = true;
                        }
                    break;
                    case 4:
                        if(context.Input->GetKey(RayEngine::KeyCode::Right, RayEngine::InputState::Held)){
                            data.lacunarity += 0.01;
                            mapUpdate = true;
                        }
                        else if(context.Input->GetKey(RayEngine::KeyCode::Left, RayEngine::InputState::Held)){
                            data.lacunarity -= 0.01;
                            mapUpdate = true;
                        }
                    break;
                    case 5:
                        if(context.Input->GetKey(RayEngine::KeyCode::Right, RayEngine::InputState::Held)){
                            data.persistence += 0.01;
                            mapUpdate = true;
                        }
                        else if(context.Input->GetKey(RayEngine::KeyCode::Left, RayEngine::InputState::Held)){
                            data.persistence -= 0.01;
                            mapUpdate = true;
                        }
                    break;
                    case 6:
                        if(context.Input->GetKey(RayEngine::KeyCode::Right, RayEngine::InputState::Pressed)){
                            border += 0.01;
                            mapUpdate = true;
                        }
                        else if(context.Input->GetKey(RayEngine::KeyCode::Left, RayEngine::InputState::Pressed)){
                            border -= 0.01;
                            mapUpdate = true;
                        }
                    break;
                }
                if(context.Input->GetKey(RayEngine::KeyCode::R, RayEngine::InputState::Pressed)){
                    data.seed.x = (rand()%1000000000)%10000;
                    data.seed.y = (rand()%1000000000)%10000;
                    mapUpdate = true;
                }

                if(mapUpdate){
                    generator->Generate(tex, data);
                    GenMap();
                    mapUpdate = false;
                }
            }
            void OnRender(const RayEngine::RenderContext &context) const override{
                
                const float textureWidth = (float)Hexagon.at(0).width;
                const float textureHeight = (float)Hexagon.at(0).height;
                const Rectangle sourceRec = {0.0f, 0.0f, textureWidth, textureHeight};
                const Vector2 spriteSize = {1.0f * HexScale, 1.16f * HexScale};
                const Vector2 origin = {spriteSize.x * 0.5f, spriteSize.y * 0.5f};
                
                for(int i=0; i<iSize; i++){
                    const Rectangle destRect = {tilesCoords[i].x, tilesCoords[i].y, spriteSize.x, spriteSize.y};
                    DrawTexturePro(Hexagon.at(type[i]), sourceRec, destRect, origin, 0.0f, WHITE);
                }

                const Vector2 sprSize = {static_cast<float>(tex.width), static_cast<float>(tex.height)};
                const Rectangle sourRec = {0.0f, 0.0f, sprSize.x, sprSize.y};
                const Vector2 ori = {sprSize.x, sprSize.y};
                const Rectangle destRe = {-0.5f * mapWidth * HexScale - 100, static_cast<float>(mapHeigh * HexScale * 0.5f), sprSize.x, sprSize.y};
                DrawTexturePro(tex, sourRec, destRe, ori, 0.0f, WHITE);
            }
            void OnRenderUI(const RayEngine::RenderUiContext &context)const override{
                std::string text = "Frequency: " + std::to_string(data.frequency);
                DrawText(text.c_str(), 10, 10, 15, BLACK);
                text = "Amplitude: " + std::to_string(data.amplitude);
                DrawText(text.c_str(), 10, 30, 15, BLACK);
                text = "Seed: {" + std::to_string(data.seed.x) + "," + std::to_string(data.seed.y) + "}";
                DrawText(text.c_str(), 10, 50, 15, BLACK);
                text = "Octave: " + std::to_string(data.octave);
                DrawText(text.c_str(), 10, 70, 15, BLACK);
                text = "Lacunatity: " + std::to_string(data.lacunarity);
                DrawText(text.c_str(), 10, 90, 15, BLACK);
                text = "Persistence: " + std::to_string(data.persistence);
                DrawText(text.c_str(), 10, 110, 15, BLACK);
                text = "Border: " + std::to_string(border);
                DrawText(text.c_str(), 10, 130, 15, BLACK);

                // DrawTexture(tex, 100, 100, WHITE);
            }
            void OnDestroy() override {
                delete tilesCoords;
                delete generator;
                UnloadTexture(tex);
            }
        private:
            Vector2* tilesCoords;
            int* type;
            std::vector<Texture2D> Hexagon;
            int iSize;
            float HexScale;
            int mapWidth, mapHeigh;

            Perlin::TextureGenerator* generator;
            Perlin::PerlinData data;
            Texture2D tex;
            float border;

            int controlState;
            int maxValue;
            bool mapUpdate;

            void GenMap(){
                std::vector<float> MapByBytes = generator->GetBytes();
                for(int i=0; i<iSize; i++){
                    float normX = (tilesCoords[i].x + 0.5f * mapWidth * HexScale) / (mapWidth * HexScale);
                    float normY = (tilesCoords[i].y + 0.5f * mapHeigh* HexScale) / (mapHeigh * HexScale);
                    int pixelX = std::clamp(static_cast<int>(normX * 1024), 0, 1024);
                    int pixelY = std::clamp(static_cast<int>(normY * 1024), 0, 1024);
                    if(MapByBytes.at(pixelY * 1024 + pixelX) > border){
                        type[i] = 0;
                    }
                    else{
                        type[i] = 1;
                    }
                }
            }
    };
    const std::string MapComponent::ID = "MapComponent";
}