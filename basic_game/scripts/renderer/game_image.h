#pragma once
#include <string>
#include <d2d1_1.h>

struct GameImage
{
    ID2D1Bitmap* pBmp;
    float x;
    float y;
    float w;
    float h;
	bool visible = true;

    GameImage(ID2D1Bitmap* b, float px, float py, float pw, float ph)
        : pBmp(b), x(px), y(py), w(pw), h(ph) {
    }
};