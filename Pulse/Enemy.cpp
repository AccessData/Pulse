#include <math.h>

# include "Enemy.h"

Enemy::Enemy(float xPos, float yPos, Home *pTarget, ID2D1Factory *pID2D1Factory) : Actor(xPos, yPos)
{
	r = 1.0f;
	g = 0.0f;
	b = 0.0f;
	a = 1.0f;

	m_pID2D1Factory = pID2D1Factory;

	CreateGeometry();
	SetTarget(pTarget);
	RotateToTarget();
}

Enemy::~Enemy()
{
}

void Enemy::Render(ID2D1DeviceContext *pRenderTarget)
{
	if (a > 0) {
		ID2D1SolidColorBrush *fillBrush;
		pRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(r, g, b, a),
			&fillBrush
			);

		pRenderTarget->FillGeometry(
			m_pTransformed,
			fillBrush
			);

		ID2D1SolidColorBrush *borderBrush;
		pRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f),
			&borderBrush
			);

		pRenderTarget->DrawGeometry(
			m_pTransformed,
			borderBrush,
			3.0f
			);

		pRenderTarget->DrawEllipse(
			D2D1::Ellipse(
				D2D1::Point2F(x, y),
				3.0f,
				3.0f),
			borderBrush,
			3.f);

		fillBrush->Release();
		borderBrush->Release();
	}
}

Bullet *Enemy::Fire()
{
	return new Bullet(x, y, m_pTarget->GetX(), m_pTarget->GetY());
}

void Enemy::SetTarget(Home *pTarget)
{
	m_pTarget = pTarget;
	RotateToTarget();
}

boolean Enemy::IsTargetAlive()
{
	return m_pTarget->IsAlive();
}

void Enemy::RotateToTarget()
{
	float xTar = m_pTarget->GetX();
	float yTar = m_pTarget->GetY();

	D2D1_MATRIX_3X2_F mov;
	mov = D2D1::Matrix3x2F::Translation(
		D2D1::SizeF(x, y)
		);

	float angle;
	if (xTar == x) {
		if (yTar > y)
			angle = 180.0f;
		else
			angle = 0.0f;
	}
	else {
		angle = atanf((yTar - y) / (xTar - x)) * 180 / 3.141592654f;
		if (xTar > x) {
			angle += 90;
		}
		else {
			angle -= 90;
		}
	}


	D2D1_MATRIX_3X2_F rot;
	rot = D2D1::Matrix3x2F::Rotation(
		angle,
		D2D1::Point2F(0.0f, 0.0f)
		);

	D2D1_MATRIX_3X2_F transform = rot * mov;
	m_pID2D1Factory->CreateTransformedGeometry(
		m_pEnemyGeometry,
		&transform,
		&m_pTransformed
		);
}

void Enemy::CreateGeometry()
{
	m_pID2D1Factory->CreatePathGeometry(&m_pEnemyGeometry);

	ID2D1GeometrySink *pSink = NULL;
	m_pEnemyGeometry->Open(&pSink);

	pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

	pSink->BeginFigure(
		D2D1::Point2F(0, -25),
		D2D1_FIGURE_BEGIN_FILLED
		);
	D2D1_POINT_2F points[] = {
		D2D1::Point2F(25, 25),
		D2D1::Point2F(0, 20),
		D2D1::Point2F(-25, 25)
	};
	pSink->AddLines(points, ARRAYSIZE(points));
	pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

	pSink->Close();
	pSink->Release();
}

boolean Enemy::HitBy(Pulse *pulse)
{
	D2D1_GEOMETRY_RELATION relation;

	m_pTransformed->CompareWithGeometry(
		pulse->GetGeometry(),
		NULL,
		&relation);
	
	switch (relation)
	{
	case D2D1_GEOMETRY_RELATION::D2D1_GEOMETRY_RELATION_DISJOINT:
		return false;
	case D2D1_GEOMETRY_RELATION::D2D1_GEOMETRY_RELATION_IS_CONTAINED:
		return false;
	case D2D1_GEOMETRY_RELATION::D2D1_GEOMETRY_RELATION_OVERLAP:
		return true;
	default:
		return false;
	}

}