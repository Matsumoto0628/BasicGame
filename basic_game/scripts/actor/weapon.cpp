#include "weapon.h"
#include <assimp/scene.h>
#include <WICTextureLoader.h>
#include "renderer.h"
#include "string_converter.h"
#include "euler_converter.h"

Weapon::Weapon()
{
}

Weapon::~Weapon()
{
}

void Weapon::Update()
{	
	if (m_isAnimation) 
	{
		if (m_animationTime >= ANIMATION_DURATION) 
		{
			m_isAnimation = false;
			m_animationTime = 0.f;
			SetPivotRotation(EulerToQuaternion(DirectX::XMFLOAT3(0, 0, 0)));
			return;
		}
		m_animationTime += 0.017f;
		float timeDecay = 1 - m_animationTime / ANIMATION_DURATION;
		SetPivotRotation(EulerToQuaternion(DirectX::XMFLOAT3(400.f * m_animationTime * timeDecay, 0, 0.f)));
	}
}

void Weapon::Slash()
{
	m_isAnimation = true;
}

void Weapon::initializeMaterialSet(int idx, aiMaterial* mat)
{
	switch (idx)
	{
	case 0:
		setupMaterialSetA(mat);
		break;
	case 1:
		setupMaterialSetB(mat);
		break;
	case 2:
		setupMaterialSetC(mat);
		break;
	default:
		setupMaterialSetA(mat);
		break;
	}
}

void Weapon::setupMaterialSetA(aiMaterial* mat)
{
	MaterialSet* pMaterialSet = &m_materialSets[0];

	// シェーダーの設定
	pMaterialSet->pShader = &m_pRenderer->TextureSpecularShader;

	// マテリアルの設定
	aiColor4D diffuse, specular;
	float shininess = 0.0f;

	aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
	aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &specular);
	aiGetMaterialFloat(mat, AI_MATKEY_SHININESS, &shininess);

	pMaterialSet->Data.Diffuse =
		DirectX::XMFLOAT4(1, 1, 1, 1);
	pMaterialSet->Data.Specular =
		DirectX::XMFLOAT4(0, 0, 0, 1);
	pMaterialSet->Data.Shininess = 1.0f;

	// テクスチャの設定
	aiString path;
	std::string texPath;
	if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
	{
		texPath = std::string("models/weapon/") + path.C_Str();
	}

	DirectX::CreateWICTextureFromFile(
		m_pRenderer->GetDevice(),
		m_pRenderer->GetDeviceContext(),
		StringToWString(texPath).c_str(),
		nullptr,
		&pMaterialSet->DiffuseTex
	);
}

void Weapon::setupMaterialSetB(aiMaterial* mat)
{
	MaterialSet* pMaterialSet = &m_materialSets[1];

	// シェーダーの設定
	pMaterialSet->pShader = &m_pRenderer->TextureSpecularShader;

	// マテリアルの設定
	aiColor4D diffuse, specular;
	float shininess = 0.0f;

	aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
	aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &specular);
	aiGetMaterialFloat(mat, AI_MATKEY_SHININESS, &shininess);

	pMaterialSet->Data.Diffuse =
		DirectX::XMFLOAT4(1, 0.5f, 0.5f, 1);
	pMaterialSet->Data.Specular =
		DirectX::XMFLOAT4(2.5f, 1.5f, 1.5f, 1);
	pMaterialSet->Data.Shininess = 2.f;

	// テクスチャの設定
	aiString path;
	std::string texPath;
	if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
	{
		texPath = std::string("models/weapon/") + path.C_Str();
	}

	DirectX::CreateWICTextureFromFile(
		m_pRenderer->GetDevice(),
		m_pRenderer->GetDeviceContext(),
		StringToWString(texPath).c_str(),
		nullptr,
		&pMaterialSet->DiffuseTex
	);
}

void Weapon::setupMaterialSetC(aiMaterial* mat)
{
	MaterialSet* pMaterialSet = &m_materialSets[2];

	// シェーダーの設定
	pMaterialSet->pShader = &m_pRenderer->TextureSpecularShader;

	// マテリアルの設定
	aiColor4D diffuse, specular;
	float shininess = 0.0f;

	aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
	aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &specular);
	aiGetMaterialFloat(mat, AI_MATKEY_SHININESS, &shininess);

	pMaterialSet->Data.Diffuse =
		DirectX::XMFLOAT4(1, 1, 1, 1);
	pMaterialSet->Data.Specular =
		DirectX::XMFLOAT4(2, 2, 2, 1);
	pMaterialSet->Data.Shininess = 1;

	// テクスチャの設定
	aiString path;
	std::string texPath;
	if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
	{
		texPath = std::string("models/weapon/") + path.C_Str();
	}

	int len = MultiByteToWideChar(CP_UTF8, 0, texPath.c_str(), -1, nullptr, 0);
	std::wstring w(len, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, texPath.c_str(), -1, &w[0], len);
	w.pop_back();

	DirectX::CreateWICTextureFromFile(
		m_pRenderer->GetDevice(),
		m_pRenderer->GetDeviceContext(),
		StringToWString(texPath).c_str(),
		nullptr,
		&pMaterialSet->DiffuseTex
	);
}