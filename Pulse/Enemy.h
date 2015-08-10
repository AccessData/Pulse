#pragma once

#include <d2d1_1.h>
#include <d3d11_1.h>

#include "Actor.h"
#include "Bullet.h"
#include "Home.h"
#include "Pulse.h"

class Enemy : public Actor
{
private:
	// target
	Home *m_pTarget;

	// Factory
	ID2D1Factory *m_pID2D1Factory;

	// struct
	ID2D1PathGeometry *m_pEnemyGeometry;
	ID2D1TransformedGeometry *m_pTransformed;

public:
	Enemy(float xPos, float yPos, Home *pTarget, ID2D1Factory *pID2D1Factory);
	~Enemy();

	void SetTarget(Home *pHome);
	boolean IsTargetAlive();

	void CreateGeometry();
	void RotateToTarget();
	void Render(ID2D1DeviceContext *pRenderTarget);
	
	Bullet *Fire();

	boolean HitBy(Pulse *pulse);
	void TakeHit(float damage) { a -= damage; }
};

