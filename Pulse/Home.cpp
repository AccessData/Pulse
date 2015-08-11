# include "Home.h"

Home::Home(float xPos, float yPos, ID2D1Factory *pID2D1Factory) : Actor(xPos, yPos)
{
	r = 0.0f;
	g = 1.0f;
	b = 0.0f;
	a = 1.0f;

	HRESULT hr = pID2D1Factory->CreatePathGeometry(&pHomeGeometry);
	if (SUCCEEDED(hr))
	{
		ID2D1GeometrySink *pSink = NULL;

		hr = pHomeGeometry->Open(&pSink);
		if (SUCCEEDED(hr))
		{
			pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

			pSink->BeginFigure(
				D2D1::Point2F(25, -25),
				D2D1_FIGURE_BEGIN_FILLED
				);
			D2D1_POINT_2F points[] = {
				D2D1::Point2F(25, 25),
				D2D1::Point2F(-25, 25),
				D2D1::Point2F(-25, -25)
			};
			pSink->AddLines(points, ARRAYSIZE(points));
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		}
		hr = pSink->Close();
		pSink->Release();

	}

	D2D1_MATRIX_3X2_F mov;
	mov = D2D1::Matrix3x2F::Translation(
		D2D1::SizeF(x, y)
		);

	D2D1_MATRIX_3X2_F transform = mov;
	hr = pID2D1Factory->CreateTransformedGeometry(
		pHomeGeometry,
		&transform,
		&pTransformed
		);
}

Home::~Home()
{
}

void Home::Render(ID2D1DeviceContext *pRenderTarget)
{
	if (a > 0) {
		ID2D1SolidColorBrush *fillBrush;
		pRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(r, g, b, a),
			&fillBrush
			);

		pRenderTarget->FillGeometry(
			pTransformed,
			fillBrush
			);

		ID2D1SolidColorBrush *borderBrush;
		pRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f),
			&borderBrush
			);

		pRenderTarget->DrawGeometry(
			pTransformed,
			borderBrush,
			3.0f
			);

		//pRenderTarget->DrawEllipse(
		//	D2D1::Ellipse(
		//	D2D1::Point2F(x, y),
		//	3.0f,
		//	3.0f),
		//	borderBrush,
		//	3.f);

		fillBrush->Release();
		borderBrush->Release();
	}
}

boolean Home::HitBy(Bullet *bullet)
{
	BOOL hit;
	pTransformed->FillContainsPoint(
		D2D1::Point2F(bullet->GetX(), bullet->GetY()),
		D2D1::Matrix3x2F::Identity(),
		&hit
		);
	return hit == TRUE;
}