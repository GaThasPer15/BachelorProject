#include "perlin.h"

Perlin::TextureGenerator::TextureGenerator(){
    metalShader = new APPLE::Shader("../DATA/Shaders/PerlinNoise.metal");
    size.height = size.width = 512;
    bytes.reserve(size.height * size.width * 4);

    metalShader->LoadOutputBuffer(size.height * size.width * sizeof(float), 2);
    metalShader->LoadOutputBuffer(size.height * size.width * sizeof(float), 3);
    metalShader->LoadParametrsBuffer(size, 1);
}

Perlin::TextureGenerator::TextureGenerator(TextureSize size){
    metalShader = new APPLE::Shader("../DATA/Shaders/PerlinNoise.metal");
    this->size = size;
    bytes.reserve(size.height * size.width * 4);

    metalShader->LoadOutputBuffer(size.height * size.width * sizeof(float), 2);
    metalShader->LoadOutputBuffer(size.height * size.width * sizeof(float), 3);
    metalShader->LoadParametrsBuffer(size, 1);
}

Perlin::TextureGenerator::~TextureGenerator(){
    delete metalShader;
}

void Perlin::TextureGenerator::SetTextureSize(TextureSize newSize){
    size = newSize;
    bytes.clear();
    bytes.reserve(size.height * size.width * 4);

    metalShader->LoadOutputBuffer(size.height * size.width * sizeof(float), 2);
    metalShader->LoadParametrsBuffer(size, 1);
}

void Perlin::TextureGenerator::Generate(Texture2D &tex, PerlinData data){
    metalShader->LoadParametrsBuffer(data, 0);
    metalShader->execute(size.width, size.height);
    metalShader->DownloadOutputBuffer(bytes, 2);
    UpdateTexture(tex, bytes.data());
}

std::vector<float> Perlin::TextureGenerator::GetBytes(){
    std::vector<float> data;
    metalShader->DownloadOutputBuffer(data, 3);
    return data;
}