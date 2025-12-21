#pragma once 

#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#include <DirectXMath.h>
#define DX_SAFE_RELEASE(x) { if(x) { (x)->Release(); (x)=nullptr; } }