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
	static float checkTextInBox(tgui::EditBox::Ptr pBox);
	static int checkComboBox(tgui::ComboBox::Ptr pBox);
	void UpdateGUIDataAtWaveChange();
	static void setBoxDefault(tgui::EditBox::Ptr pBox, float pX);
	void setComboBoxDefault(tgui::ComboBox::Ptr pBox, int pX);
	void NextWave();
	void PrevWave();
	void NewWave();
	void ClearWave();
	void NextLevel();
	void PrevLevel();
	void NewLevel();
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
	void LoadLevelInitialize();
	void setActive(bool pActive);
	bool getActive();
private:
	sf::RenderWindow *_window;
	std::list<RawImage*> _images;
	sf::Clock _time;
	sf::Font _font;
	sf::Text _currentSnapTimeText;
	vector<sf::Text> _textReference;

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


	Level* _currentLevel;
	std::vector<Level*> _levels;
	int _indexLevel = 0;
	//GUI
	tgui::Panel::Ptr _panel;
	tgui::Label::Ptr wavesLabel;
	tgui::Label::Ptr levelsLabel;
	tgui::EditBox::Ptr editQuantityBox;
	tgui::EditBox::Ptr timeStartBox;
	tgui::EditBox::Ptr delayBetweenEBox;
	tgui::EditBox::Ptr speedBox;
	tgui::EditBox::Ptr shootRBox2;
	tgui::ComboBox::Ptr enemyTypeBox;
	tgui::ComboBox::Ptr behaviourBox;
	tgui::EditBox::Ptr healthBox;
	bool _active = true;
};

