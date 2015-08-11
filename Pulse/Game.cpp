#include "Game.h"

Game::Game(LARGE_INTEGER start, LARGE_INTEGER frequency, ID2D1Factory *pID2D1Factory)
{
	m_start = m_last_fire = m_last_pulse = start;
	m_frequency = frequency;
	m_pID2D1Factory = pID2D1Factory;

	srand(GetTickCount());
}


Game::~Game()
{
}


void Game::Render(ID2D1DeviceContext *pRenderTarget)
{
	pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

	for (unsigned int i = 0; i < m_enemies.size(); i++) {
		m_enemies[i]->Render(pRenderTarget);
	}

	for (unsigned int i = 0; i < m_bullets.size(); i++) {
		m_bullets[i]->Render(pRenderTarget);
	}

	for (unsigned int i = 0; i < m_pulsars.size(); i++) {
		m_pulsars[i]->Render(pRenderTarget);
	}

	for (unsigned int i = 0; i < m_pulses.size(); i++) {
		m_pulses[i]->Render(pRenderTarget);
	}

	for (unsigned int i = 0; i < m_homes.size(); i++) {
		m_homes[i]->Render(pRenderTarget);
	}

}


void Game::Update(LARGE_INTEGER now)
{
	// fire
	if (now.QuadPart > m_last_fire.QuadPart + m_frequency.QuadPart * 0.5f) {

		if (m_homes.size() > 0) {

			for (unsigned int i = 0; i < m_enemies.size(); i++) {
				m_bullets.push_back(m_enemies[i]->Fire());
			}
		}

		m_last_fire.QuadPart = now.QuadPart;
	}

	// pulse
	if (now.QuadPart > m_last_pulse.QuadPart + m_frequency.QuadPart * 2.0f) {
		for (unsigned int i = 0; i < m_pulsars.size(); i++) {
			m_pulses.push_back(new Pulse(m_pulsars[i]->GetX(), m_pulsars[i]->GetY(), m_pID2D1Factory));
		}

		m_last_pulse.QuadPart = now.QuadPart;
	}

	// grow pulses
	if (m_pulses.empty() == false) {
		for (long i = m_pulses.size() - 1; i >= 0; i--) {
			m_pulses[i]->Grow();
		}
	}
	
	// move bullets
	if (m_bullets.empty() == false) {
		for (long i = m_bullets.size() - 1; i >= 0; i--) {
			m_bullets[i]->Move(now.LowPart);
		}
	}

	// bullets out of bounds
	if (m_bullets.empty() == false) {
		for (long i = m_bullets.size() - 1; i >= 0; i--) {
			if (m_bullets[i]->GetX() < 0 || m_bullets[i]->GetX() > 800 || m_bullets[i]->GetY() < 0 || m_bullets[i]->GetY() > 600) {
				m_bullets.erase(m_bullets.begin() + i);
			}
		}
	}

	// bullet hits
	if (m_bullets.empty() == false) {
		for (long i = m_bullets.size() - 1; i >= 0; i--) {
			// check each home
			if (m_homes.empty() == false){
				for (long j = m_homes.size() - 1; j >= 0; j--) {
					if (m_homes[j]->HitBy(m_bullets[i])) {
						m_homes[j]->TakeHit();
						m_bullets[i]->Die();
					}
				}
			}
		}
	}

	// pulse hits
	if (m_pulses.empty() == false) {
		for (long i = m_pulses.size() - 1; i >= 0; i--) {
			// check each enemy
			if (m_enemies.empty() == false) {
				for (long j = m_enemies.size() - 1; j >= 0; j--) {
					if (m_enemies[j]->HitBy(m_pulses[i])) {
						m_enemies[j]->TakeHit(m_pulses[i]->GetA() / 100);
					}
				}
			}
		}
	}

	// remove dead bullets
	if (m_bullets.empty() == false) {
		for (long i = m_bullets.size() - 1; i >= 0; i--) {
			if (!m_bullets[i]->IsAlive()) {
				m_bullets.erase(m_bullets.begin() + i);
			}
		}
	}

	// remove dead pulses
	if (m_pulses.empty() == false) {
		for (long i = m_pulses.size() - 1; i >= 0; i--) {
			if (!m_pulses[i]->IsAlive()) {
				m_pulses.erase(m_pulses.begin() + i);
			}
		}
	}

	// remove dead homes
	if (m_homes.empty() == false) {
		for (long i = m_homes.size() - 1; i >= 0; i--) {
			if (!m_homes[i]->IsAlive()) {
				m_homes.erase(m_homes.begin() + i);
			}
		}
	}

	// remove dead enemies
	if (m_enemies.empty() == false) {
		for (long i = m_enemies.size() - 1; i >= 0; i--) {
			if (!m_enemies[i]->IsAlive()) {
				m_enemies.erase(m_enemies.begin() + i);
			}
		}
	}

	// if the target is dead obtain a new one
	for (long i = m_enemies.size() - 1; i >= 0; i--) {
		if (!m_enemies[i]->IsTargetAlive() && m_homes.size() > 0) {
			m_enemies[i]->SetTarget(RandomHome());
		}
	}

}

Home *Game::AddHome(short xPos, short yPos) 
{

	float x = xPos;// (float)((rand() % 600) + 50);
	float y = yPos;// (float)((rand() % 400) + 50);
	Home *h = new Home(
		x,
		y,
		m_pID2D1Factory);

	m_homes.push_back(h);

	AddEnemy();

	return h;
}

Enemy *Game::AddEnemy() 
{
	Enemy *e;
	
	e = new Enemy(
		(float)((rand() % 600) + 50),
		(float)((rand() % 400) + 50),
		RandomHome(),
		m_pID2D1Factory);

	m_enemies.push_back(e);

	return e;
}

Pulsar *Game::AddPulsar(short xPos, short yPos)
{

	float x = xPos;// (float)((rand() % 600) + 50);
	float y = yPos;// (float)((rand() % 400) + 50);
	Pulsar *p = new Pulsar(
		x,
		y);

	m_pulsars.push_back(p);

	return p;
}
int Game::EnemyCount()
{
	return m_enemies.size();
}

Home *Game::RandomHome()
{
	if (m_homes.empty() == false) {
		long i = rand() % m_homes.size();
		return m_homes[i];
	}
	return NULL;
}