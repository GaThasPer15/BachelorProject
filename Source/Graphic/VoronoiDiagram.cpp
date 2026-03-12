#include "VoronoiDiagram.h"

Voronoi::Generator::Generator(Graphic::TextureSize _size): size(_size){
    shader = new APPLE::Shader("../DATA/Shaders/VoronoiDiagram.metal");
    shader->LoadOutputBuffer(size.width * size.height * sizeof(uint8_t) * 4, 4);
    shader->LoadOutputBuffer(size.height * size.width * sizeof(float), 5);
    shader->LoadParametrsBuffer(size, 3);

    bytes.reserve(size.height * size.width * 4);
}
Voronoi::Generator::Generator(): size({512, 512}){
    shader = new APPLE::Shader("../DATA/Shaders/VoronoiDiagram.metal");
    shader->LoadOutputBuffer(size.width * size.height * sizeof(uint8_t) * 4, 4);
    shader->LoadParametrsBuffer(size, 3);

    bytes.reserve(size.height * size.width * 4);
}

Voronoi::Generator::~Generator(){
    delete shader;
}

void Voronoi::Generator::Generate(Texture2D &tex, Voronoi::VoronoiData data){
    shader->LoadInputBuffer(data.points, sizeof(Vector2) * data.size, 0);
    shader->LoadInputBuffer(data.colors, sizeof(Color) * data.size, 1);
    unsigned int size_value = static_cast<unsigned int>(data.size);
    shader->LoadInputBuffer(&size_value, sizeof(unsigned int), 2);

    shader->execute(size.width, size.height);

    shader->DownloadOutputBuffer(bytes, 4);
    UpdateTexture(tex, bytes.data());
}

std::vector<float> Voronoi::Generator::GetDistanceVector(){
    std::vector<float> data;
    shader->DownloadOutputBuffer(data, 5);
    return data;
}