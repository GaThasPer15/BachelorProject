#pragma once 
#include <raylib.h>
#include <iostream>
#include "perlinParametrs.h"
#include "GraphicParametrs.h"


namespace Perlin{
    class TextureGenerator{
        public:
            TextureGenerator();
            TextureGenerator(Graphic::TextureSize size);
            ~TextureGenerator();
            std::vector<float> GetBytes();
            void SetTextureSize(Graphic::TextureSize newSize);
            void Generate(Texture2D& tex, PerlinData data);

        private:
            APPLE::Shader *metalShader;
            Graphic::TextureSize size;
            std::vector<unsigned char> bytes;


    };
}