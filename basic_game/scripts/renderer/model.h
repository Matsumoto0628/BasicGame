#pragma once

class Renderer;

class Mesh;

class Model
{
public:
    Model();
    ~Model();
    bool Setup(Renderer& renderer, const char* filePath);
    void Terminate();
    void Draw(Renderer& renderer);
    void Rotate(Renderer& renderer, float rot);

private:
    Mesh* m_meshes = nullptr;
    unsigned int m_meshNum = 0;
};