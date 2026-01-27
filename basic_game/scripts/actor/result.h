#pragma once
#include "actor.h"

class Result : public Actor
{
public:
	Result();
	~Result();
	void Setup() override;
	void Update() override;
	void Draw() override;
	void Terminate() override;
};