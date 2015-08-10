#pragma once
#include "Actor.h"
#include "Bullet.h"

class Home : public Actor
{
private:
	// struct
	ID2D1PathGeometry *pHomeGeometry;
	ID2D1TransformedGeometry *pTransformed;

public:
	Home(float xPos, float yPos, ID2D1Factory *pID2D1Factory);
	~Home();

	void Render(ID2D1DeviceContext *pRenderTarget);

	boolean HitBy(Bullet *bullet);
	void TakeHit() { a -= 0.05f; }
};