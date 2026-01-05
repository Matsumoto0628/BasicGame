#pragma once
#include "../renderer/model.h"

class Weapon : public Model
{
public:
	Weapon();
	~Weapon();
	void Update() override;

private:
	void setupMaterialSetA(aiMaterial* mat);
	void setupMaterialSetB(aiMaterial* mat);
	void setupMaterialSetC(aiMaterial* mat);

protected:
	void initializeMaterialSet(int idx, aiMaterial* mat) override;
};