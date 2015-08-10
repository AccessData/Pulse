#pragma once
#include "Actor.h"

class Bullet :
	public Actor
{
private:
	// Target 
	float m_xTar;
	float m_yTar;
	float m_angle;

	// Motion
	float m_speed;
	unsigned long m_time;

	// Lifetime
	boolean m_alive;

	// Struct
	float m_radius;
	D2D1_ELLIPSE circ;

public:
	Bullet(float xPos, float yPos, float xTar, float yTar);
	~Bullet();

	void Die();
	boolean IsAlive();

	void Render(ID2D1DeviceContext *pRenderTarget);
	void Move(unsigned long time);

	float GetX() { return x; }
	float GetY() { return y; }
};

