#include "stage.h"
#include <assimp/scene.h>
#include <WICTextureLoader.h>
#include "renderer.h"
#include "string_converter.h"

Stage::Stage()
{
}

Stage::~Stage() 
{
}

void Stage::Update()
{

}

void Stage::initializeMaterialSet(int idx, aiMaterial* mat)
{
	switch (idx)
	{
	case 0:
		setupMaterialSetA(mat);
		break;
	case 1:
		setupMaterialSetB(mat);
		break;
	default:
		setupMaterialSetA(mat);
		break;
	}
}

void Stage::setupMaterialSetA(aiMaterial* mat)
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
		DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1);
	pMaterialSet->Data.Shininess = 1;

	// テクスチャの設定
	aiString path;
	std::string texPath;
	if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
	{
		texPath = std::string("models/field/") + path.C_Str();
	}

	DirectX::CreateWICTextureFromFile(
		m_pRenderer->GetDevice(),
		m_pRenderer->GetDeviceContext(),
		StringToWString(texPath).c_str(),
		nullptr,
		&pMaterialSet->DiffuseTex
	);
}

void Stage::setupMaterialSetB(aiMaterial* mat)
{
	MaterialSet* pMaterialSet = &m_materialSets[1];

	// シェーダーの設定
	pMaterialSet->pShader = &m_pRenderer->TextureShader;

	// マテリアルの設定
	aiColor4D diffuse, specular;
	float shininess = 0.0f;

	aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
	aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &specular);
	aiGetMaterialFloat(mat, AI_MATKEY_SHININESS, &shininess);

	pMaterialSet->Data.Diffuse =
		DirectX::XMFLOAT4(1, 1, 1, 1);
	pMaterialSet->Data.Specular =
		DirectX::XMFLOAT4(1, 1, 1, 1);
	pMaterialSet->Data.Shininess = 100;

	// テクスチャの設定
	aiString path;
	std::string texPath;
	if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
	{
		texPath = std::string("models/field/") + path.C_Str();
	}

	DirectX::CreateWICTextureFromFile(
		m_pRenderer->GetDevice(),
		m_pRenderer->GetDeviceContext(),
		StringToWString(texPath).c_str(),
		nullptr,
		&pMaterialSet->DiffuseTex
	);
}

void Stage::setupMaterialSetC(aiMaterial* mat)
{
}