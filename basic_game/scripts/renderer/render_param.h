#pragma once
#include "cb_transform.h"
#include "cb_projection.h"
#include "cb_view.h"

class Renderer;

struct RenderParam
{
public:
    CbTransformSet  CbTransformSet;
    CbProjectionSet CbProjectionSet;
	CbViewSet CbViewSet;

    bool Initialize(Renderer& renderer);
    void Terminate(Renderer& renderer);

private:
    bool initConstantBuffer(Renderer& renderer);
};