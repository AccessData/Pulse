#pragma once

#include <d2d1_1.h>
#include <d3d11_1.h>

#include <vector>
#include <iostream>

#include "Actor.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Home.h"
#include "Pulsar.h"
#include "Pulse.h"

class Game
{
private:
	// Timing
	LARGE_INTEGER m_start;
	LARGE_INTEGER m_frequency;
	LARGE_INTEGER m_last_fire;
	LARGE_INTEGER m_last_pulse;

	// Factory
	ID2D1Factory *m_pID2D1Factory;

	// Lists
	std::vector<Bullet*> m_bullets;
	std::vector<Enemy*> m_enemies;
	std::vector<Home*> m_homes;
	std::vector<Pulsar*> m_pulsars;
	std::vector<Pulse*> m_pulses;

	// Flags
	bool m_bDraw = true;
	bool m_bFire = true;

public:
	Game(LARGE_INTEGER start, LARGE_INTEGER frequency, ID2D1Factory *pID2D1Factory);
	~Game();

	void Update(LARGE_INTEGER now);
	void Render(ID2D1DeviceContext *pRenderTarget);

	Home *AddHome(short xPos, short yPos);
	Enemy *AddEnemy();
	Pulsar *AddPulsar(short xPos, short yPos);

	Home *RandomHome();
	int EnemyCount();
};

