#include <iostream>

#include "Bullet.h"


Bullet::Bullet(float xPos, float yPos, float xTar, float yTar) : Actor(xPos, yPos)
{
	r = 1.0f;
	g = 1.0f;
	b = 1.0f;
	a = 1.0f;

	m_alive = true;
	
	m_radius = 3;
	circ = D2D1::Ellipse(D2D1::Point2F(x, y), m_radius, m_radius);

	if (xTar == xPos) {
		if (yTar > yPos)
			m_angle = 90.0f;
		else
			m_angle = -90.0f;
	}
	else {
		m_angle = atanf((yTar - yPos) / (xTar - xPos)) * 180 / PI;
		if (xTar > xPos) {
			//m_angle += 90;
		}
		else {
			m_angle += 180;
		}
	}

}

Bullet::~Bullet()
{
}

void Bullet::Render(ID2D1DeviceContext *pRenderTarget)
{
	if (a > 0) {

		ID2D1SolidColorBrush *fillBrush;
		pRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(r, g, b, a),
			&fillBrush
			);

		pRenderTarget->FillEllipse(
			&circ,
			fillBrush
			);

		fillBrush->Release();
	}
}

void Bullet::Move(unsigned long time)
{
	m_time += time;
	//std::cout << m_time << "\n\r";

	float cos_a = cosf(m_angle * PI / 180);
	float sin_a = sinf(m_angle * PI / 180);

	x += 2 * cos_a;
	y += 2 * sin_a;
	circ.point = D2D1::Point2F(x, y);
}

void Bullet::Die()
{
	m_alive = false;
}

boolean Bullet::IsAlive()
{
	return m_alive;
}