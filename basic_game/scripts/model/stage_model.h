#pragma once
#include "model.h"

class StageModel : public Model
{
public:
	StageModel();
	~StageModel();

private:
	void setupMaterialSetA(aiMaterial* mat);
	void setupMaterialSetB(aiMaterial* mat);
	void setupMaterialSetC(aiMaterial* mat);

protected:
	void initializeMaterialSet(int idx, aiMaterial* mat) override;
};