/**
 * @file pch.h
 * @brief プリコンパイル済みヘッダー（DirectX・共通マクロ定義）
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "winmm.lib")
#include <DirectXMath.h>

/** @brief COM オブジェクトを安全に解放してポインタを nullptr にするマクロ */
#define DX_SAFE_RELEASE(x) { if(x) { (x)->Release(); (x)=nullptr; } }

/** @brief ヒープオブジェクトを安全に削除してポインタを nullptr にするマクロ */
#define SAFE_DELETE(p)    { if(p) { delete p; p = nullptr; }}

/** @brief ヒープ配列を安全に削除してポインタを nullptr にするマクロ */
#define SAFE_DELETE_ARRAY(p)    { if(p) { delete[] p; p = nullptr; }}
