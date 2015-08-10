#pragma once
#include "Actor.h"

class Pulsar : public Actor
{
private:
	// struct
	float radius;
	D2D1_ELLIPSE circ;

public:
	Pulsar(float xPos, float yPos);
	~Pulsar();

	void Render(ID2D1DeviceContext *pRenderTarget);
};