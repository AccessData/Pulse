# include "Actor.h"

Actor::Actor(float xPos, float yPos)
{
	x = xPos;
	y = yPos;

	//r = (rand() % 100) / 100.0f;
	//g = (rand() % 100) / 100.0f;
	//b = (rand() % 100) / 100.0f;
	//a = (rand() % 100) / 100.0f;
}

Actor::~Actor()
{
}

void Actor::Render(ID2D1DeviceContext *pRenderTarget)
{
}

void Actor::Update(long long time)
{

}

boolean Actor::IsAlive()
{
	return a > 0;
}
