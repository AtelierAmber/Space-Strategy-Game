#pragma once

#include "Fleet.h"

class AI
{
public:
	AI(Fleet* playerFleet, std::string team, MainGUI* gui);
	AI(){ /* Emtpy */ }
	~AI();

	void init(Fleet* playerFleet, std::string team, MainGUI* gui);

	Fleet* getFleet(){ return &m_fleet; }

	void update();

	void nextWave();

private:
	Fleet m_fleet;
	int m_currentWave = 0;
	Fleet* m_playerFleet = nullptr;
};

