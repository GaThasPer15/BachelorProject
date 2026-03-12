#pragma once
#include <raylib.h>
#include <iostream>
#include "GraphicParametrs.h"
#include "../Shaders/MetalCompute.h"
#include "../MathLib/MathLib.h"

namespace Voronoi{
    struct VoronoiData{
        public:
            Vector2 *points;
            Color *colors;
            size_t size;
    };

    class Generator{
        APPLE::Shader *shader;
        Graphic::TextureSize size;
        std::vector<unsigned char> bytes;

        public:
            Generator();
            Generator(Graphic::TextureSize _size);
            ~Generator();
            void SetTextureSize(Graphic::TextureSize _size);
            void Generate(Texture2D& tex, VoronoiData data);
    };
}