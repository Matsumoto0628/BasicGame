#pragma once
#include "actor.h"

class Title : public Actor
{
public:
	Title();
	~Title();
	void Setup() override;
	void Update() override;
	void Draw() override;
	void Terminate() override;
};