#pragma once 
#include <iostream>
#include <ostream>
#include <raylib.h>
#include "rlgl.h"
#include "../Component.h"
#include "../Entity.h"
#include "../../Graphic/perlinParametrs.h"
#include "../../Shaders/MetalCompute.h"
#include "../../MathLib/MathLib.h"


namespace VORONOI{
    struct VoronoiData{
        public:
            Vector2 *points;
            Color *colors;
            size_t size;
    };
}

namespace GPT{
    class VoronoiComponent : public RayEngine::Component{
        public:
            static const std::string ID;
            VoronoiComponent(){
                Image img = GenImageColor(1024, 1024, BLANK);
                plate = LoadTextureFromImage(img);
                UnloadImage(img);
                data = VORONOI::VoronoiData();
                texSize = {1024, 1024};
                
                GenerateVoronoiData();
                


            }
            Component *Clone() const override{
                return new VoronoiComponent();
            }
            const std::string &GetID() const override {return ID;}
            void OnRender(const RayEngine::RenderContext &context) const override{
                
            }
            void OnDestroy() override{
                delete data.colors;
                delete data.points;
                UnloadTexture(plate);
            }
        private:
        Texture2D plate;
        VORONOI::VoronoiData data;
        Perlin::TextureSize texSize;

        APPLE::Shader *generator;

        void GenerateVoronoiData(){
            GTPMath::Random::HashRNGGenerator *numGen = new GTPMath::Random::HashRNGGenerator(1);
                data.size = 20;
                if(data.points != nullptr) delete data.points;
                data.points = new Vector2 [data.size];
                if(data.colors != nullptr) delete data.colors;
                data.colors = new Color [data.size];
                for(int i=0; i<data.size; i++){
                     data.points[i] = numGen->Vector2(0.0f, 0.0f);
                     data.colors[i] = Color{
                        static_cast<unsigned char>(numGen->Int(static_cast<int32_t>(data.points[i].x), static_cast<int32_t>(data.points[i].y))%256),
                        static_cast<unsigned char>(numGen->Int(static_cast<int32_t>(data.points[i].x), static_cast<int32_t>(data.points[i].y))%256),
                        static_cast<unsigned char>(numGen->Int(static_cast<int32_t>(data.points[i].x), static_cast<int32_t>(data.points[i].y))%256),
                        255
                     };
                }
                delete numGen;
        }
    };
    const std::string VoronoiComponent::ID = "VoronoiComponent";
}