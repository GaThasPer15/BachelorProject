#pragma once 

#include <iostream>
#include <ostream>
#include <raylib.h>
#include "../Component.h"
#include "../Entity.h"
#include "../Map/MapFunction.hpp"
#include "../../Graphic/Graphic.h"


namespace GTP{
    class MapComponent : public RayEngine::Component{
        public:
            static const std::string ID;
            MapComponent(int Width, int Heigh, float Scale): mapWidth(Width), mapHeigh(Heigh), HexScale(Scale){
                if(Heigh >= Width) throw std::invalid_argument("Map heigh cannot be bigger than width\n");
                whichPlate = true;
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
                
                Graphic::TextureSize texSize {1024, 1024};
                perlin = new Perlin::TextureGenerator(texSize);
                
                Image img = GenImageColor(1024, 1024, BLANK);
                Pplate = LoadTextureFromImage(img);
                Vplate = LoadTextureFromImage(img);
                UnloadImage(img);

                Pdata.frequency=8.0f;
                perlin->Generate(Pplate, Pdata);
                
                Vdata = Voronoi::VoronoiData();
                Vdata.size = 20;
                Vseed = 1;
                GenerateVoronoiData();
                voronoi = new Voronoi::Generator(texSize);
                voronoi->Generate(Vplate, Vdata);
                VupdatedFlag = false;

                size_t size = mapHeigh*mapWidth;
                type = new int[size];
                GenMap();

            }
            Component *Clone() const override{
                return new MapComponent(mapWidth, mapHeigh, HexScale);
            }
            const std::string &GetID() const override {return ID;}
            void OnUpdate(const RayEngine::UpdateContext &context) override{
                if(context.Input->GetKey(RayEngine::KeyCode::N, RayEngine::InputState::Pressed)){
                    whichPlate?maxValue=2:maxValue=7;
                    controlState=0;
                    whichPlate = !whichPlate;
                }
                if(context.Input->GetKey(RayEngine::KeyCode::Up, RayEngine::InputState::Pressed)){
                    controlState==0?:controlState--;
                }
                else if(context.Input->GetKey(RayEngine::KeyCode::Down, RayEngine::InputState::Pressed)){
                    controlState++;
                    controlState%=maxValue;
                }
                if(whichPlate){
                    switch(controlState){
                        case 0:   
                            if(context.Input->GetKey(RayEngine::KeyCode::Right, RayEngine::InputState::Held)){
                                Pdata.frequency +=0.1;
                                mapUpdate = true;
                            }
                            else if(context.Input->GetKey(RayEngine::KeyCode::Left, RayEngine::InputState::Held)){
                                Pdata.frequency -=0.1;
                                mapUpdate = true;
                            }
                        break;
                        case 1:
                            if(context.Input->GetKey(RayEngine::KeyCode::Right, RayEngine::InputState::Held)){
                                Pdata.amplitude += 0.01;
                                mapUpdate = true;
                            }
                            else if(context.Input->GetKey(RayEngine::KeyCode::Left, RayEngine::InputState::Held)){
                                Pdata.amplitude -= 0.01;
                                mapUpdate = true;
                            }
                        break;
                        case 2:
                            if(context.Input->GetKey(RayEngine::KeyCode::L, RayEngine::InputState::Held)){
                                Pdata.seed.x += 0.025;
                                mapUpdate = true;
                            }
                            else if(context.Input->GetKey(RayEngine::KeyCode::J, RayEngine::InputState::Held)){
                                Pdata.seed.x -= 0.025;
                                mapUpdate = true;
                            }
                            if(context.Input->GetKey(RayEngine::KeyCode::I, RayEngine::InputState::Held)){
                                Pdata.seed.y -= 0.025;
                                mapUpdate = true;
                            }
                            else if(context.Input->GetKey(RayEngine::KeyCode::K, RayEngine::InputState::Held)){
                                Pdata.seed.y += 0.025;
                                mapUpdate = true;
                            }
                        break;
                        case 3:
                            if(context.Input->GetKey(RayEngine::KeyCode::Right, RayEngine::InputState::Pressed)){
                                Pdata.octave += 1;
                                mapUpdate = true;
                            }
                            else if(context.Input->GetKey(RayEngine::KeyCode::Left, RayEngine::InputState::Pressed)){
                                Pdata.octave-= 1;
                                mapUpdate = true;
                            }
                        break;
                        case 4:
                            if(context.Input->GetKey(RayEngine::KeyCode::Right, RayEngine::InputState::Held)){
                                Pdata.lacunarity += 0.01;
                                mapUpdate = true;
                            }
                            else if(context.Input->GetKey(RayEngine::KeyCode::Left, RayEngine::InputState::Held)){
                                Pdata.lacunarity -= 0.01;
                                mapUpdate = true;
                            }
                        break;
                        case 5:
                            if(context.Input->GetKey(RayEngine::KeyCode::Right, RayEngine::InputState::Held)){
                                Pdata.persistence += 0.01;
                                mapUpdate = true;
                            }
                            else if(context.Input->GetKey(RayEngine::KeyCode::Left, RayEngine::InputState::Held)){
                                Pdata.persistence -= 0.01;
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
                }
                else{
                    switch(controlState){
                        case 0:
                            if(context.Input->GetKey(RayEngine::KeyCode::Right, RayEngine::InputState::Pressed)){
                                Vdata.size++;
                                mapUpdate = true;
                            }
                            if(context.Input->GetKey(RayEngine::KeyCode::Left, RayEngine::InputState::Pressed)){
                                Vdata.size--;
                                mapUpdate = true;
                            }
                        break;
                        case 1:
                            if(context.Input->GetKey(RayEngine::KeyCode::Right, RayEngine::InputState::Pressed)){
                                Vseed++;
                                mapUpdate = true;
                            }
                            if(context.Input->GetKey(RayEngine::KeyCode::Left, RayEngine::InputState::Pressed)){
                                Vseed--;
                                mapUpdate = true;
                            }
                        break;
                    }
                }
                if(context.Input->GetKey(RayEngine::KeyCode::R, RayEngine::InputState::Pressed)){
                    if(whichPlate){
                        Pdata.seed.x = rand()%10000;
                        Pdata.seed.y = rand()%10000;
                    }
                    else{
                        Vseed = rand()%10000;
                    }
                    mapUpdate = true;
                }


                if(mapUpdate){
                    perlin->Generate(Pplate, Pdata);
                    GenerateVoronoiData();
                    voronoi->Generate(Vplate, Vdata);
                    mapUpdate = false;

                    GenMap();
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

                const Vector2 sprSize = {static_cast<float>(Pplate.width), static_cast<float>(Pplate.height)};
                const Rectangle sourRec = {0.0f, 0.0f, sprSize.x, sprSize.y};
                const Vector2 ori = {sprSize.x, sprSize.y};
                const Rectangle destRe = {-0.5f * mapWidth * HexScale - 100, static_cast<float>(mapHeigh * HexScale * 0.5f), sprSize.x, sprSize.y};
                if(whichPlate)
                    DrawTexturePro(Pplate, sourRec, destRe, ori, 0.0f, WHITE);
                else
                    DrawTexturePro(Vplate, sourRec, destRe, ori, 0.0f, WHITE);
            }
            void OnRenderUI(const RayEngine::RenderUiContext &context)const override{
                if(whichPlate){
                    std::string text = "Frequency: " + std::to_string(Pdata.frequency);
                    DrawText(text.c_str(), 10, 10, 15, BLACK);
                    text = "Amplitude: " + std::to_string(Pdata.amplitude);
                    DrawText(text.c_str(), 10, 30, 15, BLACK);
                    text = "Seed: {" + std::to_string(Pdata.seed.x) + "," + std::to_string(Pdata.seed.y) + "}";
                    DrawText(text.c_str(), 10, 50, 15, BLACK);
                    text = "Octave: " + std::to_string(Pdata.octave);
                    DrawText(text.c_str(), 10, 70, 15, BLACK);
                    text = "Lacunatity: " + std::to_string(Pdata.lacunarity);
                    DrawText(text.c_str(), 10, 90, 15, BLACK);
                    text = "Persistence: " + std::to_string(Pdata.persistence);
                    DrawText(text.c_str(), 10, 110, 15, BLACK);
                    text = "Border: " + std::to_string(border);
                    DrawText(text.c_str(), 10, 130, 15, BLACK);
                }
                else{
                    std::string text = "Points: " + std::to_string(Vdata.size);
                    DrawText(text.c_str(), 10, 10, 15, BLACK);
                    text = "Seed: " + std::to_string(Vseed);
                    DrawText(text.c_str(), 10, 30, 15, BLACK);
                }
                // DrawTexture(tex, 100, 100, WHITE);
            }
            void OnDestroy() override {
                delete tilesCoords;
                delete perlin;
                delete voronoi;
                delete Vdata.points;
                delete Vdata.colors;
                UnloadTexture(Pplate);
                UnloadTexture(Vplate);
            }
        private:
            Vector2* tilesCoords;
            int* type;
            std::vector<Texture2D> Hexagon;
            int iSize;
            float HexScale;
            int mapWidth, mapHeigh;

            Perlin::TextureGenerator* perlin;
            Perlin::PerlinData Pdata;
            Texture2D Pplate;
            float border;

            Voronoi::VoronoiData Vdata;
            Voronoi::Generator *voronoi;
            Texture2D Vplate;
            size_t Vseed;
            bool VupdatedFlag;

            int controlState;
            int maxValue;
            bool mapUpdate;

            bool whichPlate;

            void GenMap(){
                std::vector<float> MapByBytes = perlin->GetBytes();
                std::vector<float> voronoiDist = voronoi->GetDistanceVector();
                for(int i=0; i<iSize; i++){
                    float normX = (tilesCoords[i].x + 0.5f * mapWidth * HexScale) / (mapWidth * HexScale);
                    float normY = (tilesCoords[i].y + 0.5f * mapHeigh* HexScale) / (mapHeigh * HexScale);
                    int pixelX = std::clamp(static_cast<int>(normX * 1024), 0, 1024);
                    int pixelY = std::clamp(static_cast<int>(normY * 1024), 0, 1024);
                    
                    //Circle method
                    float value = 0;
                    int radius = static_cast<int>(Pplate.width/mapWidth/2);
                    int count = 0;
                    for(int k=pixelY-radius; k<=pixelY+radius; k++){
                        for(int l=pixelX-radius; l<=pixelX+radius; l++){
                            int distX = l-pixelX;
                            int distY = k-pixelY;
                            if(distX * distX + distY * distY <= radius*radius){
                                int cx = std::clamp(l, 0, Pplate.width);
                                int cy = std::clamp(k, 0, Pplate.height);
                                int id = cy * Pplate.width + cx;
                                // value += std::max(MapByBytes[id], voronoiDist[id]);
                                value += MapByBytes[id] * 0.6f + voronoiDist[id] * 0.4f;
                                ++count;
                            }
                        }
                    }
                    value /= count;
                    if(value > border){
                    //Point metod
                    // int id = pixelY * 1024 + pixelX;
                    // float val = MapByBytes[id] * voronoiDist[id];
                    // if(val > border){
                        type[i] = 0;
                    }
                    else{
                        type[i] = 1;
                    }
                }
            }

            private:
                void GenerateVoronoiData(){
                    GTPMath::Random::HashRNGGenerator *numGen = new GTPMath::Random::HashRNGGenerator(Vseed);
                        
                        if(Vdata.points != nullptr) delete Vdata.points;
                        Vdata.points = new Vector2 [Vdata.size];
                        if(Vdata.colors != nullptr) delete Vdata.colors;
                        Vdata.colors = new Color [Vdata.size];
                        for(int i=0; i<Vdata.size; i++){
                            Vdata.points[i] = numGen->Vector2(0.0f, 0.0f);
                            Vdata.colors[i] = Color{
                                static_cast<unsigned char>(numGen->Int(static_cast<int32_t>(Vdata.points[i].x), static_cast<int32_t>(Vdata.points[i].y))%256),
                                static_cast<unsigned char>(numGen->Int(static_cast<int32_t>(Vdata.points[i].x), static_cast<int32_t>(Vdata.points[i].y))%256),
                                static_cast<unsigned char>(numGen->Int(static_cast<int32_t>(Vdata.points[i].x), static_cast<int32_t>(Vdata.points[i].y))%256),
                                255
                            };
                        }
                        delete numGen;
                }
    };
    const std::string MapComponent::ID = "MapComponent";
}