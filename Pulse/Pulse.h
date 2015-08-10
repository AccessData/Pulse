#pragma once
#include "Actor.h"

class Pulse : public Actor
{
private:
	float radius;
	ID2D1Factory *m_pID2D1Factory;
	ID2D1EllipseGeometry *m_pGeometry;

public:
	Pulse(float xPos, float yPos, ID2D1Factory *pID2D1Factory);
	~Pulse();

	void Render(ID2D1DeviceContext *pRenderTarget);
	void Grow();

	ID2D1Geometry *GetGeometry();
};

