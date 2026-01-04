#pragma once
#include "../renderer/model.h"

class Stage : public Model
{
public:
	Stage();
	~Stage();
	void Update() override;

private:
	void setupMaterialSetA(aiMaterial* mat);
	void setupMaterialSetB(aiMaterial* mat);
	void setupMaterialSetC(aiMaterial* mat);

protected:
	void initializeMaterialSet(int idx, aiMaterial* mat) override;
};