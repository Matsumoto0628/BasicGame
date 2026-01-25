#include "box_model.h"
#include "renderer.h"

BoxModel::BoxModel()
{
}

BoxModel::~BoxModel()
{
}

void BoxModel::initializeMaterialSet(int idx, aiMaterial* mat)
{
	switch (idx)
	{
	default:
		setupMaterialSet(mat);
		break;
	}
}

void BoxModel::setupMaterialSet(aiMaterial* mat)
{
	MaterialSet* pMaterialSet = &m_materialSets[0];

	// シェーダーの設定
	pMaterialSet->pShader = &m_pRenderer->SpecularShader;

	// マテリアルの設定
	pMaterialSet->Data.Diffuse =
		DirectX::XMFLOAT4(1, 0.5f, 0.5f, 1);
	pMaterialSet->Data.Specular =
		DirectX::XMFLOAT4(1.f, 0.5f, 0.5f, 1);
	pMaterialSet->Data.Shininess = 0.5f;
}

void BoxModel::ChangeColor(DirectX::XMFLOAT4 color)
{
	MaterialSet* pMaterialSet = &m_materialSets[0];
	pMaterialSet->Data.Diffuse = color;
}