#pragma once
#include <string>

struct GameText
{
    std::wstring text;
    float x;
    float y;
	DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	bool visible = true;

    GameText() {}
    GameText(const std::wstring& t, float px, float py, DirectX::XMFLOAT4 c)
        : text(t), x(px), y(py), color(c) {
    }
};