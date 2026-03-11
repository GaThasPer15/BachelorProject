#pragma once
#include <raylib.h>
#include <iostream>
#include "../Shaders/MetalCompute.h"

namespace Perlin{

    struct PerlinData{
        Vector2 seed = {0, 0};
        float amplitude = 0.5f;
        float frequency = 2.0f;
        int octave = 1;
        float lacunarity = 2.0f;
        float persistence = 0.5f;
    };

    struct TextureSize{
        int width = 512;
        int height = 512;
    };
}

namespace APPLE{
    
}