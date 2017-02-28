#pragma once
#include <TGUI/TGUI.hpp>
#include "mge/behaviours/AbstractBehaviour.hpp"
#include "mgengine/UI/ImageBehaviour.h"
#include "mge/config.hpp"
#include "mygame\Level.h"
#include "mge\core\World.hpp"
#include <list>
#include <SFML\System\Time.hpp>
using namespace std;
class LevelEditorBehaviour:public AbstractBehaviour
{
public:
	LevelEditorBehaviour(sf::RenderWindow *pWindow, World *pWorld);
	void InitializeHud(tgui::Gui * pGuiRef);
	~LevelEditorBehaviour();
	void UpdateGUIData();
	void NextWave();
	void PrevWave();
	void draw(sf::CircleShape circle);	
	virtual void update(float pStep);
	void DrawUI();
	void DrawGrid();
	void draw(sf::RectangleShape pCircle);
	void DotGrid();
	void DrawReferenceGrid();
	void CheckSaveLevel();
	void CheckLoadLevel();
	void SaveLevel();
	void LoadLevel();
private:
	sf::RenderWindow *_window;
	std::list<RawImage*> _images;
	sf::Clock _time;
	sf::Font _font;
	sf::Text _currentSnapTimeText;
	vector<sf::Text> _textReference;
	Level* _currentLevel;	
	World* _world;	
	float _scrollBar = 0;
	float _scrollLastSecond = 0;
	float _currentSnapTime = 0;
	int _scrollSpeed = 4;
	int _currentWave = 0;
	bool _mousePressed = false;
	bool _gameStarted = false;
	bool _autoScroll = false;
	bool _scrollKeyPressed = false;
	bool _numKeyPressed = false;
	float _secReferenceScrollBar = 0;

	void CreateWaypoint(sf::Vector2i pWayPos);
	void StartGame();
	void UpdateScrolling();
	void UpdateWaypointCreation();
	void UpdateStartLevelPreview();
	void UpdateWaveSelection();

	//GUI
	tgui::Label::Ptr wavesLabel;
};

