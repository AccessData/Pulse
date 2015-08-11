# include "Pulsar.h"

Pulsar::Pulsar(float xPos, float yPos) : Actor(xPos, yPos)
{
	radius = 30;
	r = 1.0f;
	g = 1.0f;
	b = 0.0f;
	a = 1.0f;
	circ = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
}

Pulsar::~Pulsar()
{
}

void Pulsar::Render(ID2D1DeviceContext *pRenderTarget)
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

		ID2D1SolidColorBrush *borderBrush;
		pRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f),
			&borderBrush
			);

		pRenderTarget->DrawEllipse(
			&circ,
			borderBrush,
			3.0f
			);

		fillBrush->Release();
		borderBrush->Release();
	}	
}