#pragma once
#include "model.h"

class BoxModel : public Model
{
public:
	BoxModel();
	~BoxModel();
	void ChangeColor(DirectX::XMFLOAT4 color);
	void ChangeShader(Shader* shader);

private:
	void setupMaterialSet(aiMaterial* mat);

protected:
	void initializeMaterialSet(int idx, aiMaterial* mat) override;
};