#pragma once
#include "../renderer/model.h"

class Weapon : public Model
{
public:
	Weapon();
	~Weapon();
	void Update() override;
	void Slash();

private:
	void setupMaterialSetA(aiMaterial* mat);
	void setupMaterialSetB(aiMaterial* mat);
	void setupMaterialSetC(aiMaterial* mat);
	bool m_isAnimation = false;
	float m_animationTime = 0.f;
	static constexpr float ANIMATION_DURATION = 0.5f;

protected:
	void initializeMaterialSet(int idx, aiMaterial* mat) override;
};