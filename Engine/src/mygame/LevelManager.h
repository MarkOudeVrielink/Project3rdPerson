#pragma once
#include "mygame\EnemyWave.h"
#include <list>
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics.hpp>
#include "Waypoint.h"
#include "mge\core\World.hpp"
#include "mge\behaviours\AbstractBehaviour.hpp"
#include "mygame\Level.h"
#include <SFML\System\Time.hpp>
class LevelManager: public AbstractBehaviour
{
public:
	LevelManager(World *pWorld);
	~LevelManager();
	void StartLevel();
	int getIndexLevel();
	Level * getCurrentLevel();
	Level* NextLevel();
	Level* PreviousLevel();
	Level* NewLevel();
	Level* DeleteLevel();
	virtual void update(float pStep);

	void StartGameFromMenu();

	void InitializeBackground();


	void InitializeAsteroids();

	void LoadLevel();

private:
	void	_updateBackground(float pStep);
	Level*	_currentLevel;//change to private and create get method
	
	std::vector<Level>	_levels;
	std::string			_levelName;
	sf::RenderWindow*	_window;
	int					_indexLevel = 0;
	World*				_world;
	sf::Clock			_time;
	bool				_startGame = false;

	GameObject * _backgroundPlane_0;
	GameObject * _backgroundPlane_1;
	GameObject * _backgroundPlane_2;
	GameObject * _backgroundPlane_3;

	GameObject *		asteroidParent = NULL;
	GameObject *		_planet;
	vector<GameObject*> _asteroids;

};


