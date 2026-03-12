#include "MetalCompute.h"

#ifdef __APPLE__
#include <Metal/Metal.h>
#include <Foundation/Foundation.h>
#include "../Graphic/perlinParametrs.h"
#include "../Graphic/GraphicParametrs.h"

APPLE::Shader::Shader(const std::string& shaderPath): workGroup(16, 16, 1), threadCount(1, 1, 1), activeShader(true){
    @autoreleasepool{
        initializeDevice();
        loadShader(shaderPath);
        createPipeline();
        printf("[METAL] Constructor successful\n");
    }
}


APPLE::Shader::~Shader(){
    printf("[METAL] Destructor starting...\n");
    @autoreleasepool{
        printf("[METAL] Releasing %zu buffers...\n", buffers.size());
        for(auto& [index, buffer] : buffers) {
            if(buffer) {
                printf("[METAL] Releasing buffer at index %d\n", index);
                CFRelease(buffer);
            }
        }
        if(commandQueue) CFRelease(commandQueue);
        if(pipeline) CFRelease(pipeline);
        if(kernelFunction) CFRelease(kernelFunction);
        if(library) CFRelease(library);
        if(device) CFRelease(device);
    }
}

void APPLE::Shader::initializeDevice(){
    @autoreleasepool{
        id<MTLDevice> dev = MTLCreateSystemDefaultDevice();
        if(!dev){
            printf("Failed to load device in shader\n");
            activeShader = false;
            return;
        }
        printf("Shader device has been loaded");
        device = (__bridge_retained void*)dev;
        id<MTLCommandQueue> queue = [dev newCommandQueue];
        commandQueue = (__bridge_retained void*)queue;
    }
}

void APPLE::Shader::loadShader(const std::string& shaderPath){
    if(!activeShader)
        return;
    @autoreleasepool{
        id<MTLDevice> dev = (__bridge id<MTLDevice>) device;

        NSString* path = [NSString stringWithUTF8String:shaderPath.c_str()];

        NSError* error = nil;
        NSString* source = [NSString stringWithContentsOfFile:path encoding:NSUTF8StringEncoding error:&error];
        if(!source){
            printf("Failed to load shader file: %s\n", shaderPath.c_str());
            activeShader = false;
            return;
        }

        id<MTLLibrary> lib = [dev newLibraryWithSource:source options:nil error:&error];
        if(!lib){
            if(error){
                NSString* errorDescription = [error localizedDescription];
                printf("Failed to load shader: %s\n", [errorDescription UTF8String]);
            }
            else{
                printf("Failed to load shader: %s\n", shaderPath.c_str());
            }
            activeShader = false;
            return;
        }

        library = (__bridge_retained void*)lib;
    }
}

void APPLE::Shader::createPipeline(){
    if(!activeShader)
        return;
    @autoreleasepool{
        id<MTLDevice> dev = (__bridge id<MTLDevice>)device;
        id<MTLLibrary> lib = (__bridge id<MTLLibrary>) library;

        id<MTLFunction> func = [lib newFunctionWithName:@"compute_main"];

        if(!func){
            printf("Function 'compute_main' not found in shader\n");
            activeShader = false;
            return;
        }

        kernelFunction = (__bridge_retained void*) func;

        NSError* error = nil;
        id<MTLComputePipelineState> pipel = [dev newComputePipelineStateWithFunction:func error:&error];

        if(!pipel){
            if(error){
                NSString* errorDescription = [error localizedDescription];
                printf("Failed to create pipeline: %s\n", [errorDescription UTF8String]);
            }   
            else{
                printf("Failed to create pipeline in shader\n");
            }
            activeShader = false;
            return;
        }
        pipeline = (__bridge_retained void*)pipel;
    }
}

void APPLE::Shader::setWorkingGroupSize(int x, int y, int z){
    workGroup.x = x;
    workGroup.y = y;
    workGroup.z = z;
}

void APPLE::Shader::setThreadCount(int x, int y, int z){
    threadCount.x = x;
    threadCount.y = y;
    threadCount.z = z;
}

void APPLE::Shader::LoadInputBuffer(const void* data, size_t size, int bindingIndex){
    if(!activeShader)
        return;
    @autoreleasepool{
        id<MTLDevice> dev = (__bridge id<MTLDevice>) device;

        if(buffers.find(bindingIndex) != buffers.end()){
            id<MTLBuffer> existingBuffer = (__bridge id<MTLBuffer>)buffers[bindingIndex];

            if([existingBuffer length] != size){
                CFRelease(buffers[bindingIndex]);
            }else{
                memcpy([existingBuffer contents], data, size);
                return;
            }
        }

        id<MTLBuffer> bufer = [dev newBufferWithBytes:data length:size options:MTLResourceStorageModeShared];
        buffers[bindingIndex] = (__bridge_retained void*)bufer;
    }
}

void APPLE::Shader::LoadOutputBuffer(size_t size, int bindingIndex){
    if(!activeShader)
        return;
    @autoreleasepool{
        id<MTLDevice> dev = (__bridge id<MTLDevice>) device;

        if(buffers.find(bindingIndex) != buffers.end()){
            id<MTLBuffer> existingBuffer = (__bridge id<MTLBuffer>)buffers[bindingIndex];

            if([existingBuffer length] != size){
                CFRelease(buffers[bindingIndex]);
            }else return;
        }
        id<MTLBuffer> buffer = [dev newBufferWithLength:size options:MTLResourceStorageModeShared];
        buffers[bindingIndex] = (__bridge_retained void*)buffer;
        
    }
}

void APPLE::Shader::executeInternal(int x, int y, int z){
    if(!activeShader) return;
    @autoreleasepool{
        id<MTLDevice> dev = (__bridge id<MTLDevice>)device;
        id<MTLCommandQueue>queue = (__bridge id<MTLCommandQueue>) commandQueue;
        id<MTLComputePipelineState>pipel = (__bridge id<MTLComputePipelineState>)pipeline;
        
        id<MTLCommandBuffer> commandBuffer = [queue commandBuffer];
        id<MTLComputeCommandEncoder> encoder = [commandBuffer computeCommandEncoder];
        
        [encoder setComputePipelineState:pipel];
        
        for(auto& [index, buffer] : buffers){
            id<MTLBuffer> buf = (__bridge id<MTLBuffer>)buffer;
            [encoder setBuffer:buf offset:0 atIndex:index];
        }
        
        MTLSize threadgroupSize = MTLSizeMake(workGroup.x, workGroup.y, workGroup.z);
        MTLSize threadgroups = MTLSizeMake(
            (x + workGroup.x - 1) / workGroup.x,
            (y + workGroup.y - 1) / workGroup.y,
            (z + workGroup.z - 1) / workGroup.z
        );
        
        [encoder dispatchThreadgroups:threadgroups threadsPerThreadgroup:threadgroupSize];
        [encoder endEncoding];
        
        [commandBuffer commit];
        
        [commandBuffer waitUntilCompleted];
    }
}

void APPLE::Shader::execute(){
    executeInternal(threadCount.x, threadCount.y, threadCount.z);
}
void APPLE::Shader::execute(int threads){
    executeInternal(threads, 1, 1);
}
void APPLE::Shader::execute(int x, int y){
    executeInternal(x, y, 1);
}
void APPLE::Shader::execute(int x, int y, int z){
    executeInternal(x, y, z);
}

template <typename T>
    void APPLE::Shader::LoadParametrsBuffer(const T& parametrs, int bindingIndex){
        if(!activeShader)
            return;
        @autoreleasepool{
            id<MTLDevice> dev = (__bridge id<MTLDevice>) device;
            if(buffers.find(bindingIndex) != buffers.end()){
                id<MTLBuffer> existingBuffer = (__bridge id<MTLBuffer>)buffers[bindingIndex];

                if([existingBuffer length] != sizeof(T)){
                    CFRelease(buffers[bindingIndex]);
                } else {
                    memcpy([existingBuffer contents], &parametrs, sizeof(T));
                    return;
                }
            }

            id<MTLBuffer> buffer = [dev newBufferWithBytes:&parametrs length:sizeof(T) options:MTLResourceStorageModeShared];
            buffers[bindingIndex] = (__bridge_retained void*)buffer;
        }
    }

    template <typename T>
    void APPLE::Shader::DownloadOutputBuffer(std::vector<T>& outData, int bindingIndex){
        if(!activeShader) return;
        @autoreleasepool{
            if(buffers.find(bindingIndex) == buffers.end()){
                printf("Buffer not found at binding index\n");
                return;
            }
            id<MTLBuffer> buffer = (__bridge id<MTLBuffer>)buffers[bindingIndex];

            T* data = (T*)[buffer contents];
            size_t count = [buffer length] / sizeof(T);
            outData.assign(data, data + count);
        }
    }

template void APPLE::Shader::LoadParametrsBuffer<float>(const float&, int);
template void APPLE::Shader::LoadParametrsBuffer<int>(const int&, int);

template void APPLE::Shader::DownloadOutputBuffer<float>(std::vector<float>&, int);
template void APPLE::Shader::DownloadOutputBuffer<unsigned char>(std::vector<unsigned char>&, int);

template void APPLE::Shader::LoadParametrsBuffer<Perlin::PerlinData>(const Perlin::PerlinData&, int);
template void APPLE::Shader::LoadParametrsBuffer<Graphic::TextureSize>(const Graphic::TextureSize&, int);
template void APPLE::Shader::DownloadOutputBuffer<Perlin::PerlinData>(std::vector<Perlin::PerlinData>&, int);
template void APPLE::Shader::DownloadOutputBuffer<Graphic::TextureSize>(std::vector<Graphic::TextureSize>&, int);

#endif