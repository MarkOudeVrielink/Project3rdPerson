#pragma once
#include "mge/behaviours/AbstractBehaviour.hpp"
#include "mgengine/UI/ImageBehaviour.h"
#include "mge/config.hpp"
#include "mygame\Level.h"
#include <list>
using namespace std;
class LevelEditorBehaviour:public AbstractBehaviour
{
public:
	LevelEditorBehaviour(sf::RenderWindow *pWindow);
	~LevelEditorBehaviour();
	void draw(sf::CircleShape circle);
	
	virtual void update(float pStep);
	void DrawUI();
	void DrawGrid();
	void SaveLevel();
	void LoadLevel();
private:
	sf::RenderWindow *_window;
	std::list<RawImage*> _images;
	Level* _currentLevel;

	void CreateWaypoint(sf::Vector2i pWayPos);
};

