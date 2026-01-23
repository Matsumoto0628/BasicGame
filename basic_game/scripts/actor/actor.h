#pragma once

class Actor
{
public:
	virtual ~Actor() = default;
	virtual void Setup() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Terminate() = 0;
};