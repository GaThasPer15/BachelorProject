#pragma once 
#include <raylib.h>
#include <iostream>
#include "perlinParametrs.h"


namespace Perlin{
    class TextureGenerator{
        public:
            TextureGenerator();
            TextureGenerator(TextureSize size);
            ~TextureGenerator();
            std::vector<float> GetBytes();
            void SetTextureSize(TextureSize newSize);
            void Generate(Texture2D& tex, PerlinData data);

        private:
            APPLE::Shader *metalShader;
            TextureSize size;
            std::vector<unsigned char> bytes;


    };
}