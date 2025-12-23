#pragma once
#include "cb_transform.h"

class Renderer;

struct RenderParam
{
    CbTransformSet  CbTransformSet;

    bool Initialize(Renderer& renderer);
    void Terminate(Renderer& renderer);

private:
    bool initConstantBuffer(Renderer& renderer);
};