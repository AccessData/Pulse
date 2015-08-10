#pragma once
#include <d2d1_1.h>

#define PI (3.141592654f)

class Actor
{
protected:
	// location
	float x;
	float y;
	// colour
	float r;
	float g;
	float b;
	float a;

public:
	Actor(float x, float y);
	~Actor();

	void Render(ID2D1DeviceContext *pRenderTarget);
	void Update(long long time);
	boolean IsAlive();
	float GetX() { return x; };
	float GetY() { return y; };
	float GetA() { return a; };
};