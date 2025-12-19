#include "triangle.h"

Triangle::Triangle()
{
    vertices[0] = { 0.f, 0.5f, 0.f };
    vertices[1] = { 0.5f, -0.5f, 0.f };
    vertices[2] = { -0.5f, -0.5f, 0.f };
}

Triangle::~Triangle()
{
    DX_SAFE_RELEASE(vertexBuffer);
}

bool Triangle::CreateVertexBuffer(Renderer& renderer)
{
    // å„èq
}

void Triangle::Draw(Renderer& renderer)
{
    // å„èq
}