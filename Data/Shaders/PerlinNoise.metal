#include <metal_stdlib>
using namespace metal;
#ifdef M_PI
#undef M_PI
#endif
#define M_PI 3.1415926535897932384626433832795


struct PerlinData{
    float2 seed;
    float amplitude;
    float frequency;
    int octave;
    float lacunarity;
    float persistence;
};
struct TexSize{
    int width;
    int height;
};

float2 randomGradientGen(float2 edge){
    edge = float2(dot(edge*M_PI, float2(sin(edge.x*123.4), cos(edge.y*69.47))),
                    dot(edge/M_PI, float2(cos(edge.y*213.7), sin(edge.x*11.25))));
    return 2.0*fract(sin(edge*M_PI)*1770.13) - 1.0;
}

float dotGradient(float2 point, float2 fractal){
    return dot(fractal, randomGradientGen(point));
}


float noise(float2 coord){
    float2 n0 = floor(coord);
    float2 f = fract(coord);
    float2 weight = smoothstep(0.0, 1.0, f);

    return mix(
            mix(
                dotGradient(n0 + float2(0.0, 0.0), f - float2(0.0, 0.0)),
                dotGradient(n0 + float2(1.0, 0.0), f - float2(1.0, 0.0)),
                weight.x
            ),
            mix(
                dotGradient(n0 + float2(0.0, 1.0), f - float2(0.0, 1.0)),
                dotGradient(n0 + float2(1.0, 1.0), f - float2(1.0, 1.0)),
                weight.x
            ),
            weight.y
    );
}

kernel void compute_main(
    constant PerlinData& params [[buffer(0)]],
    constant TexSize& textureSize [[buffer(1)]],
    device uchar4* output [[buffer(2)]],
    device float* outputValues[[buffer(3)]],
    uint2 gid [[thread_position_in_grid]]
    ){
    if(gid.x >= textureSize.width || gid.y >= textureSize.height){return;}
    uint index = gid.y * textureSize.width + gid.x;

    float scale = 1000.0f;
    float2 st = (float2(gid) + params.seed*scale)/float2(textureSize.width, textureSize.height);
    float2 uv = st * float2(float(textureSize.width)/float(textureSize.height), 1);
    float freq = params.frequency;
    float amp = params.amplitude;
    float perlin = 0;
    for(int i=0; i<params.octave; i++){
        perlin += noise(uv * freq) * amp;
        freq *= params.lacunarity;
        amp *= params.persistence;
    }
    perlin = 0.5 - perlin;
    // perlin *= smoothstep(0.0, 1.0, abs(uv.x-0.6));
    outputValues[index] = perlin;

    uchar value = uchar(clamp(perlin, 0.0f, 1.0f) * 255.0f);
    output[index] = uchar4(value, value, value, 255);
}