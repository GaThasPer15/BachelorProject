#pragma once

#ifdef __APPLE__    
#include <string>
#include <map>
#include <vector>
#include "../MathLib/MathLib.h"

namespace APPLE{

    class Shader{
    public:
        Shader(const std::string& shaderPath);
        ~Shader();

        template<typename T>
        void LoadParametrsBuffer(const T& parametrs, int bindingIndex = 0);

        void LoadInputBuffer(const void* data, size_t size, int bindingIndex);
        void LoadOutputBuffer(size_t size, int bindingIndex);

        template<typename T>
        void DownloadOutputBuffer(std::vector<T>& outData, int bindingIndex);

        void setWorkingGroupSize(int x, int y = 1, int z = 1);

        void setThreadCount(int x, int y=1, int z = 1);

        void execute();
        void execute(int threads);
        void execute(int x, int y);
        void execute(int x, int y, int z);
    private:
        void* device = nullptr;
        void* library = nullptr;
        void* kernelFunction = nullptr;
        void* pipeline = nullptr;
        void* commandQueue = nullptr;

        bool activeShader;
        
        std::map<int, void*> buffers;

        GTPMath::Vec3<int> workGroup;
        GTPMath::Vec3<int> threadCount;


        void initializeDevice();
        void loadShader(const std::string& shaderPath);
        void createPipeline();
        void executeInternal(int x, int y, int z);
    };

}

#endif