#include <metal_stdlib>
#include <metal_math>
using namespace metal;
#ifdef M_PI
#undef M_PI
#endif
#define M_PI 3.1415926535897932384626433832795


struct TexSize{
    int width;
    int height;
};

kernel void compute_main(
    device const float2 *points [[buffer(0)]],
    device const uchar4 *colors [[buffer(1)]],
    constant uint &size [[buffer(2)]],
    constant TexSize& textureSize [[buffer(3)]],
    device uchar4* output [[buffer(4)]],
    device float* distanceOutput [[buffor(5)]],
    uint2 gid [[thread_position_in_grid]]
    ){
    if(gid.x >= textureSize.width || gid.y >= textureSize.height){return;}
    uint index = gid.y * textureSize.width + gid.x;

    uint VoronoiIndex = -1;
    float bestDistance = MAXFLOAT;
    float secoundBestDistance = MAXFLOAT;
    float2 uv = float2(gid.x, gid.y) / float2(textureSize.width, textureSize.height);
    for(uint i=0; i<size; i++){
        float2 diff = points[i] - uv;
        float val = diff.x * diff.x + diff.y * diff.y;
        if(val < bestDistance){
            secoundBestDistance = bestDistance;
            bestDistance = val;
            VoronoiIndex = i;
        }
        else if(val < secoundBestDistance){
            secoundBestDistance = val;
        }
    }
    float edgeDist = secoundBestDistance - bestDistance;
    float normFactor = 1.0 / (1.0 / sqrt(float(size)));
    distanceOutput[index] = clamp(edgeDist * sqrt(float(size)) * 2.0, 0.0, 1.0);
    output[index] = colors[VoronoiIndex];
}