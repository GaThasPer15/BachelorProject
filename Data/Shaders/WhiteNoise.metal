// DATA/Shaders/WhiteNoise.metal
#include <metal_stdlib>
using namespace metal;

kernel void compute_main(
    device float4* output [[buffer(0)]],
    uint2 gid [[thread_position_in_grid]])
{
    // Hardcoded 1024x1024
    uint width = 1024;
    uint height = 1024;
    
    // Bounds check
    if (gid.x >= width || gid.y >= height) {
        return;
    }
    
    uint index = gid.y * width + gid.x;
    
    // Normalize coordinates to [0, 1]
    float2 uv = float2(gid) / float2(width, height);
    
    // White noise - pseudo-random hash
    float noise = fract(sin(dot(uv, float2(12.9898, 78.233))) * 43758.5453);
    
    // Output grayscale RGBA
    output[index] = float4(noise, noise, noise, 1.0);
}