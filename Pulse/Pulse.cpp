#include "Pulse.h"



Pulse::Pulse(float xPos, float yPos, ID2D1Factory *pID2D1Factory) : Actor(xPos, yPos)
{
	radius = 50;

	r = 1.0f;
	g = 1.0f;
	b = 0.0f;
	a = 1.0f;

	m_pID2D1Factory = pID2D1Factory;
	
	m_pID2D1Factory->CreateEllipseGeometry(
		D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius),
		&m_pGeometry
		);
}


Pulse::~Pulse()
{
}

void Pulse::Render(ID2D1DeviceContext *pRenderTarget)
{
	if (a > 0) {

		ID2D1SolidColorBrush *borderBrush;
		pRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(r, g, b, a),
			&borderBrush
			);

		pRenderTarget->DrawGeometry(
			m_pGeometry,
			borderBrush,
			3.0f
			);

		borderBrush->Release();
	}
}

void Pulse::Grow() 
{
	radius += 1.5f;
	a -= 0.005f;

	m_pID2D1Factory->CreateEllipseGeometry(
		D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius),
		&m_pGeometry
		);
}

ID2D1Geometry *Pulse::GetGeometry()
{
	return m_pGeometry;
}