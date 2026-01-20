#pragma once
#include "model.h"

class WeaponModel : public Model
{
public:
	WeaponModel();
	~WeaponModel();

private:
	void setupMaterialSetA(aiMaterial* mat);
	void setupMaterialSetB(aiMaterial* mat);
	void setupMaterialSetC(aiMaterial* mat);

protected:
	void initializeMaterialSet(int idx, aiMaterial* mat) override;
};