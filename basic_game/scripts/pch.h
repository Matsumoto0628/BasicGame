#pragma once 

#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "winmm.lib")
#include <DirectXMath.h>
#define DX_SAFE_RELEASE(x) { if(x) { (x)->Release(); (x)=nullptr; } }
#define SAFE_DELETE(p)    { if(p) { delete p; p = nullptr; }}
#define SAFE_DELETE_ARRAY(p)    { if(p) { delete[] p; p = nullptr; }}