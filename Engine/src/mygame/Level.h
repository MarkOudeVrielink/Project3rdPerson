#pragma once
#include "mygame\EnemyWave.h"
#include <list>
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics.hpp>

class Level
{
public:
	Level();
	~Level();
	void CreateWaypoint(sf::Vector2i pWayPos);
	std::vector<sf::CircleShape> _wayPoints;
private:
	std::vector<EnemyWave*> _enemies;
	std::string _LevelName;
	
};

