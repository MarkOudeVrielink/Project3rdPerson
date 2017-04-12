#include "LevelEditor.h"
#include <SFML/Window/Mouse.hpp>
#include <SFML\System\Vector2.hpp>
#include <iostream>
#include <TGUI/TGUI.hpp>
#include <TGUI/Widgets/devel/Table.hpp>
#include <glm.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\System\Vector2.hpp>
#include "mgengine\Parser\LevelParser.h"
using namespace std;
LevelEditorBehaviour::LevelEditorBehaviour(sf::RenderWindow *pWindow, World *pWorld) :AbstractBehaviour()
{
	_window = pWindow;	
	_world = pWorld;
	LoadLevelInitialize();
	if (!_font.loadFromFile(config::MGE_FONT_PATH + "arial.ttf")) {
		cout << "Could not load font, exiting..." << endl;
	}

	//Start Draw the time of the current Snap
	_currentSnapTimeText.setFont(_font);
	_currentSnapTimeText.setCharacterSize(16);
	_currentSnapTimeText.setFillColor(sf::Color::White);
	_currentSnapTimeText.setString("Sec: 0");
	_currentSnapTimeText.setPosition(sf::Vector2f(250, 30));
	glActiveTexture(GL_TEXTURE0);
	_window->pushGLStates();
	_window->draw(_currentSnapTimeText);
	_window->popGLStates();
	//Finish Draw the time of the current Snap
	int counter = 0;
	for (int i = 1; i < 20000;i++)
	{
		//the text at the left with the time at x position
		counter++;
		if (counter == 100)
		{
			sf::Text text = sf::Text();

			text.setFont(_font);
			text.setCharacterSize(16);
			text.setFillColor(sf::Color::White);

			text.setString(std::to_string(i));
			text.setPosition(sf::Vector2f((float)350, (float)1080 - i));
			glActiveTexture(GL_TEXTURE0);
			_window->pushGLStates();
			_window->draw(text);
			_window->popGLStates();
			counter = 0;
			_textReference.push_back(text);

		}

	}
	setReferenceWorld();
	
	//LoadLevelInitialize();

}
LevelEditorBehaviour::~LevelEditorBehaviour()
{
	/*if (_currentLevel) {
		delete _currentLevel;
		_currentLevel = NULL;
	}
	*/
	_currentLevel = NULL;
	for (auto level : _levels)
	{
		if(level!= NULL)
		delete level;
		level = NULL;
	}

	if (_window)
		_window = nullptr;
	

	if (_world) {
		// _world;
		_world = NULL;
	}
}


//Create all the GUI elements and add them to the gui
void LevelEditorBehaviour::InitializeHud(tgui::Gui* pGuiRef)
{
	auto theme = tgui::Theme::create("TGUI-0.7/widgets/TransparentGrey.txt");
	_panel = tgui::Panel::create();
	_panel->setSize(1920, 1080);
	_panel->setBackgroundColor(tgui::Color(0, 0, 0, 0));
	pGuiRef->add(_panel);
#pragma region Waves
	#pragma region Waves Selection
		#pragma region Waves counter
			auto windowWidth = tgui::bindWidth(*pGuiRef);
			auto windowHeight = tgui::bindHeight(*pGuiRef);
			wavesLabel = theme->load("label");
			wavesLabel->setText("Wave: " + std::to_string(_currentWave));
			wavesLabel->setSize(350, 50);
			wavesLabel->setPosition(windowWidth*0.855, 0.005f * windowHeight);
			wavesLabel->setTextSize(28);
			wavesLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
			_panel->add(wavesLabel);
	#pragma endregion Waves counter //No layout
		#pragma region Waves Buttons
			tgui::HorizontalLayout::Ptr layout0 = tgui::HorizontalLayout::create();
			layout0->setSize(350, 30);
			layout0->setPosition(windowWidth*0.8, 0.05f * windowHeight);
			_panel->add(layout0);

			auto prevWaveButton = tgui::Button::create();
			prevWaveButton->setText("Prev");
			layout0->add(prevWaveButton);

			auto newWaveButton = tgui::Button::copy(prevWaveButton);
			newWaveButton->setText("New");
			layout0->add(newWaveButton);


			auto clearWaveButton = tgui::Button::copy(prevWaveButton);
			clearWaveButton->setText("Clear");
			layout0->add(clearWaveButton);

			auto deleteWaveButton = tgui::Button::copy(prevWaveButton);
			deleteWaveButton->setText("Delete");
			layout0->add(deleteWaveButton);

			auto nextWaveButton = tgui::Button::copy(prevWaveButton);
			nextWaveButton->setText("Next");
			layout0->add(nextWaveButton);
			prevWaveButton->connect("pressed", &LevelEditorBehaviour::PrevWave, this);
			nextWaveButton->connect("pressed", &LevelEditorBehaviour::NextWave, this);
			newWaveButton->connect("pressed", &LevelEditorBehaviour::NewWave, this);
			clearWaveButton->connect("pressed", &LevelEditorBehaviour::ClearWave, this);
			deleteWaveButton->connect("pressed", &LevelEditorBehaviour::DeleteWave, this);

		#pragma endregion end of 0, wave buttons 
	#pragma endregion end of TOP of the waves
	#pragma region WaveProperties 
		#pragma region Quantity of enemies
			tgui::HorizontalLayout::Ptr layout1 = tgui::HorizontalLayout::create();
			layout1->setSize(350, 50);

			layout1->setPosition(windowWidth*0.8, 0.12f * windowHeight);
			_panel->add(layout1);

			tgui::Label::Ptr quantityLabel = theme->load("label");
			quantityLabel->setText("Number of Enemies: ");
			quantityLabel->setTextSize(24);
			layout1->add(quantityLabel);

			editQuantityBox = tgui::EditBox::create();
			editQuantityBox->setDefaultText(std::to_string(*_currentLevel->getCurrentWave()->getSizeWave()));
			editQuantityBox->setTextSize(24);
			layout1->add(editQuantityBox, "quantity");
		#pragma endregion end of 1, enemies quantity
		#pragma region Time Start
			tgui::HorizontalLayout::Ptr layout2 = tgui::HorizontalLayout::create();
			layout2->setSize(350, 30);
			layout2->setPosition(windowWidth*0.8, 0.17f * windowHeight);
			_panel->add(layout2);

			tgui::Label::Ptr timeStart = theme->load("label");
			timeStart->setText("Time Start: ");
			timeStart->setTextSize(24);
			timeStart->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
			layout2->add(timeStart);

			timeStartBox = tgui::EditBox::create();
			timeStartBox->setDefaultText("0");//TODO add variable with actual number
			timeStartBox->setTextSize(24);
			layout2->add(timeStartBox, "timeS");//timeStart
		#pragma endregion end of 2
		#pragma region Time End
			tgui::HorizontalLayout::Ptr layout3 = tgui::HorizontalLayout::create();
			layout3->setSize(350, 30);
			layout3->setPosition(windowWidth*0.8, 0.2f * windowHeight);
			_panel->add(layout3);

			tgui::Label::Ptr delayBetweenE = theme->load("label");
			delayBetweenE->setText("Delay E:");
			delayBetweenE->setTextSize(24);
			delayBetweenE->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
			layout3->add(delayBetweenE);

			delayBetweenEBox = tgui::EditBox::create();
			delayBetweenEBox->setDefaultText("-1");//TODO add variable with actual number
			delayBetweenEBox->setTextSize(24);
			layout3->add(delayBetweenEBox, "timeE");//timeStart
		#pragma endregion end of 3
		#pragma region Enemies Speed
			tgui::HorizontalLayout::Ptr layout4 = tgui::HorizontalLayout::create();
			layout4->setSize(350, 30);
			layout4->setPosition(windowWidth*0.8, 0.23f * windowHeight);
			_panel->add(layout4);

			tgui::Label::Ptr speed = theme->load("label");
			speed->setText("Speed: ");
			speed->setTextSize(24);
			speed->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
			layout4->add(speed);

			speedBox = tgui::EditBox::create();
			speedBox->setDefaultText("20 for example");//TODO add variable with actual number
			speedBox->setTextSize(24);
			layout4->add(speedBox, "speed");//timeStart
		#pragma endregion end of 4
		#pragma region Shoot Ratio
			tgui::HorizontalLayout::Ptr layout5 = tgui::HorizontalLayout::create();
			layout5->setSize(350, 30);
			layout5->setPosition(windowWidth*0.8, 0.26f * windowHeight);
			_panel->add(layout5);

			tgui::Label::Ptr shootR = theme->load("label");
			shootR->setText("Shoot Ratio:");
			shootR->setTextSize(20);
			shootR->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
			layout5->add(shootR);

			shootRBox2 = tgui::EditBox::create();
			shootRBox2->setDefaultText("1.5");//TODO add variable with actual number
			shootRBox2->setTextSize(24);
			layout5->add(shootRBox2, "shootRatioBox2");//timeStart
		#pragma endregion end of 5
		#pragma region EnemyTypes
			tgui::HorizontalLayout::Ptr layout6 = tgui::HorizontalLayout::create();
			layout6->setSize(350, 30);
			layout6->setPosition(windowWidth*0.8, 0.29f * windowHeight);
			_panel->add(layout6);

			tgui::Label::Ptr enemyType = theme->load("label");
			enemyType->setText("Enemy Type:");
			enemyType->setTextSize(24);
			enemyType->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
			layout6->add(enemyType);

			enemyTypeBox = tgui::ComboBox::create();
			enemyTypeBox->addItem("Player");
			enemyTypeBox->addItem("Enemy");
			enemyTypeBox->addItem("Yogurt");
			enemyTypeBox->addItem("Sushi");
			enemyTypeBox->addItem("Sandwich");
			enemyTypeBox->addItem("Potato");
			enemyTypeBox->addItem("Pizza");
			enemyTypeBox->addItem("Muffin");
			enemyTypeBox->addItem("Boss");
			enemyTypeBox->setSelectedItem("Sandwich");
			layout6->add(enemyTypeBox);
		#pragma endregion end of 6
		#pragma region Enemy behaviours
			tgui::HorizontalLayout::Ptr layout7 = tgui::HorizontalLayout::create();
			layout7->setSize(350, 30);
			layout7->setPosition(windowWidth*0.8, 0.32f * windowHeight);
			_panel->add(layout7);

			tgui::Label::Ptr behaviourLabel = theme->load("label");
			behaviourLabel->setText("Behaviour:");
			behaviourLabel->setTextSize(24);
			behaviourLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
			layout7->add(behaviourLabel);

			behaviourBox = tgui::ComboBox::create();
			behaviourBox->addItem("Default");
			behaviourBox->addItem("Kamikase");
			behaviourBox->addItem("Boss");
			behaviourBox->setSelectedItem("Default");
			layout7->add(behaviourBox);
		#pragma endregion end of 7
		#pragma region Health
			tgui::HorizontalLayout::Ptr layout8 = tgui::HorizontalLayout::create();
			layout8->setSize(350, 30);
			layout8->setPosition(windowWidth*0.8, 0.35f * windowHeight);
			_panel->add(layout8);

			tgui::Label::Ptr health = theme->load("label");
			health->setText("Health:");
			health->setTextSize(24);
			health->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
			layout8->add(health);

			healthBox = tgui::EditBox::create();
			healthBox->setDefaultText("2 for example");//TODO add variable with actual number
			healthBox->setTextSize(24);
			layout8->add(healthBox, "health");//timeStart
		#pragma endregion end of 8
		#pragma region Enemies table

			//TODO: Create in child window and add more options
			auto table = std::make_shared<tgui::Table>();
			table->setSize({ 350, 40 });
			table->setPosition(windowWidth*0.8, 0.38f * windowHeight);
			table->setHeaderColumns({ "Enemy", "Health", "Shoot R", "Type" });
			table->setBackgroundColor({ 203,201,207 });
			table->setFixedColumnWidth(0, 80);
			table->setStripesRowsColor({ 135,206,250 }, { 233,233,233 });
			_panel->add(table);

			tgui::ComboBox::Ptr button3 = tgui::ComboBox::create();
			button3->addItem("Type 1");
			button3->addItem("Type 2");
			button3->addItem("Type 3");
			button3->setSelectedItem("Type 1");
			button3->setSize({ 80,30 });
			button3->setTextSize(10);

			//	button3->connect("pressed", &LevelEditorBehaviour::NextWave, this);

			auto tableRow = std::make_shared<tgui::TableRow>();
			tableRow->addItem("Enemy 1: ");
			tableRow->addItem("1");
			tableRow->addItem("1");
			tableRow->add(button3, true);
			//tableRow->addItem("1.5");

			tableRow->setItemsHorizontalAlign(tgui::TableItem::Center);


			//Make it tableRow to modify alignment
			table->add(tableRow);
			table->addRow({ "Enemy 2: ", "1", "1","1" });
			table->addRow({ "Enemy 3: ", "1", "1","1" });
			table->addRow({ "Enemy 4: ", "1", "1" ,"1" });

		#pragma endregion end of 9
	#pragma endregion end of the wave properties
#pragma endregion
#pragma region Level
	#pragma region Parser... Load/Save

				tgui::HorizontalLayout::Ptr layoutParser = tgui::HorizontalLayout::create();
				layoutParser->setSize(350, 30);
				layoutParser->setPosition(windowWidth*0.8, 0.8f * windowHeight);
				_panel->add(layoutParser);

				auto saveButton = tgui::Button::create();
				saveButton->setText("Save");
				layoutParser->add(saveButton);
				auto loadButton = tgui::Button::copy(saveButton);
				loadButton->setText("Load");
				layoutParser->add(loadButton);

				saveButton->connect("pressed", &LevelEditorBehaviour::SaveLevel, this);
				loadButton->connect("pressed", &LevelEditorBehaviour::LoadLevel, this);
	#pragma endregion  end of LayoutParser Buttons
	#pragma region Level counter
				tgui::HorizontalLayout::Ptr layoutLevelName = tgui::HorizontalLayout::create();
				layoutLevelName->setSize(350, 30);
				layoutLevelName->setPosition(windowWidth*0.8, 0.83f * windowHeight);
				_panel->add(layoutLevelName);

				levelsLabel = theme->load("label");
				levelsLabel->setText("Level: " + std::to_string(_indexLevel));
				levelsLabel->setSize(350, 50);
				levelsLabel->setTextSize(28);
				levelsLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
				layoutLevelName->add(levelsLabel, "levelsLabel");
	#pragma endregion Waves counter //No layout
	#pragma region Level Select

				tgui::HorizontalLayout::Ptr layoutLevelSelect = tgui::HorizontalLayout::create();
				layoutLevelSelect->setSize(350, 30);
				layoutLevelSelect->setPosition(windowWidth*0.8, 0.86f * windowHeight);
				_panel->add(layoutLevelSelect);

				auto prevLevelButton = tgui::Button::create();
				prevLevelButton->setText("Prev");
				layoutLevelSelect->add(prevLevelButton);

				auto newLevelButton = tgui::Button::copy(prevLevelButton);
				newLevelButton->setText("New");
				layoutLevelSelect->add(newLevelButton);

				auto nextLevelButton = tgui::Button::copy(prevLevelButton);
				nextLevelButton->setText("Next");
				layoutLevelSelect->add(nextLevelButton);
				prevLevelButton->connect("pressed", &LevelEditorBehaviour::PrevLevel, this);
				nextLevelButton->connect("pressed", &LevelEditorBehaviour::NextLevel, this);
				newLevelButton->connect("pressed", &LevelEditorBehaviour::NewLevel, this);
	#pragma endregion  end of LayoutParser Buttons
#pragma endregion
	UpdateGUIDataAtWaveChange();
}
#pragma region TGUI Methods
void LevelEditorBehaviour::UpdateGUIData()
{
	wavesLabel->setText("Wave: " + std::to_string(_currentWave));
	levelsLabel->setText("Level: " + std::to_string(_indexLevel));
	if (checkTextInBox(editQuantityBox) != -1) {
		_currentLevel->getCurrentWave()->setSizeWave(checkTextInBox(editQuantityBox));
	}
	if (checkTextInBox(timeStartBox) != -1) {
		_currentLevel->getCurrentWave()->setStartTime(checkTextInBox(timeStartBox));
	}
	if (checkTextInBox(delayBetweenEBox) != -1) {
		_currentLevel->getCurrentWave()->setDelayBetweenEnemies(checkTextInBox(delayBetweenEBox));//TODO:: ADD End Time per wave
	}
	if (checkTextInBox(speedBox) != -1) {
		_currentLevel->getCurrentWave()->setSpeed(checkTextInBox(speedBox));//TODO: Add speed in waves
	}
	if (checkTextInBox(shootRBox2) != -1) {
		
		_currentLevel->getCurrentWave()->setShootRatio(checkTextInBox(shootRBox2));//TODO: Add shootRatio in waves
		
	}
	if (checkComboBox(enemyTypeBox) != -1){
		_currentLevel->getCurrentWave()->setEnemyType(Materials::ID((int)checkComboBox(enemyTypeBox)));//TODO: Add enemies types in waves
	}
	if (checkComboBox(behaviourBox) != -1) {
		_currentLevel->getCurrentWave()->setEnemyBehaviour(checkComboBox(behaviourBox));//TODO: Add enemies behaviour in waves
	}
	if (checkTextInBox(healthBox) != -1) {
		_currentLevel->getCurrentWave()->setHealth(checkTextInBox(healthBox));//TODO: Add enemies health in waves
	}
}
void LevelEditorBehaviour::UpdateGUIDataAtWaveChange()
{
	setBoxDefault(editQuantityBox, *_currentLevel->getCurrentWave()->getSizeWave());
	setBoxDefault(timeStartBox, *_currentLevel->getCurrentWave()->getStartTime());
	setBoxDefault(speedBox, *_currentLevel->getCurrentWave()->getSpeed());
	setBoxDefault(shootRBox2, *_currentLevel->getCurrentWave()->getShootRatio());
	//cout << *_currentLevel->getCurrentWave()->getShootRatio() << endl;
	//cout << *_currentLevel->getCurrentWave()->getShootRatio() << endl;
	//cout << *_currentLevel->getCurrentWave()->getShootRatio() << endl;
	//cout << *_currentLevel->getCurrentWave()->getShootRatio() << endl;
	setBoxDefault(healthBox, *_currentLevel->getCurrentWave()->getHealth());
	setBoxDefault(delayBetweenEBox, *_currentLevel->getCurrentWave()->getDelayBetweenEnemies());
	setComboBoxDefault(enemyTypeBox, *_currentLevel->getCurrentWave()->getEnemyType());
	setComboBoxDefault(behaviourBox, *_currentLevel->getCurrentWave()->getEnemyBehaviour());
	//setBoxDefault(timeStartBox, *_currentLevel->getCurrentWave()->getStartTime());
	//TODO:: ADD End Time per wave
	//DONE: Add speed in waves
	//TODO: Add enemies Types
	//TODO: Add enemies behaviour
	//DONE: Add enemies Health

}

float LevelEditorBehaviour::checkTextInBox(tgui::EditBox::Ptr pBox)
{
	float number = std::stof((string)pBox->getDefaultText());
	if (pBox->getText() != "" && pBox->getText() != ".") {
		number = std::stof((string)pBox->getText());
		return number;
	}
	return -1;
}
int LevelEditorBehaviour::checkComboBox(tgui::ComboBox::Ptr pBox)
{
	int number = pBox->getSelectedItemIndex();
	if (number != -1) {
		return number;
	}
	return -1;
}
void LevelEditorBehaviour::setBoxDefault(tgui::EditBox::Ptr pBox, float pX)
{
	pBox->setText("");
	pBox->setDefaultText(std::to_string(pX));

}
void LevelEditorBehaviour::setComboBoxDefault(tgui::ComboBox::Ptr pBox, int pX)
{
	
	pBox->setSelectedItemByIndex((pX));

}
#pragma region Wave
void LevelEditorBehaviour::NextWave()
{
	_currentLevel->NextEnemyWave();
	_currentWave = _currentLevel->getIndexWave();
	//cout << _currentWave << endl;
	UpdateGUIDataAtWaveChange();
}
void LevelEditorBehaviour::PrevWave()
{
	//cout << _currentWave << endl;
	_currentLevel->PreviousWave();
	_currentWave = _currentLevel->getIndexWave();
	UpdateGUIDataAtWaveChange();
}
void LevelEditorBehaviour::NewWave()
{
	//cout << _currentWave << endl;
	_currentLevel->NewWave();
	_currentWave = _currentLevel->getIndexWave();
	UpdateGUIDataAtWaveChange();
}
void LevelEditorBehaviour::ClearWave()
{
	 _currentLevel->ClearWave();
	//NextWave();
}
void LevelEditorBehaviour::DeleteWave()
{
	_currentLevel->DeleteWave();
	NextWave();
}
#pragma endregion
#pragma region Level
void LevelEditorBehaviour::NextLevel()
{	
		if (_indexLevel < _levels.size() - 1)
		{
			_indexLevel++;
			//cout << _indexLevel << "<NEXT index " << endl;
			_currentLevel = _levels.at(_indexLevel);
			_currentWave = _currentLevel->getIndexWave();
			//cout << _levels.size() << "<levels size " << endl;
			UpdateGUIDataAtWaveChange();
		}
		else {
			cout << "We should not keep going up in the levels" << endl;
			
		}	
}
void LevelEditorBehaviour::PrevLevel()
{
	if (_indexLevel > 0)
	{
		_indexLevel--;
		cout << _indexLevel << "<New Level Index " << endl;
		_currentLevel = _levels.at(_indexLevel);
		_currentWave = _currentLevel->getIndexWave();
		UpdateGUIDataAtWaveChange();
	}
	else
	{
		cout << "We should not keep going low in the levels" << endl;
	
	}
}
void LevelEditorBehaviour::NewLevel()
{
	_levels.push_back(new Level(_window));
	_indexLevel++;
	_currentLevel = _levels.at(_indexLevel);
	_currentLevel->ReferenceWorld(_world, _owner);
	_currentWave = _currentLevel->getIndexWave();
	cout << _levels.size() << "<waves size " << endl;

	UpdateGUIDataAtWaveChange();
}
#pragma endregion
#pragma endregion
void LevelEditorBehaviour::setReferenceWorld()
{
	_currentLevel->ReferenceWorld(_world, _owner);
}
void LevelEditorBehaviour::update(float pStep)
{
	//Keyboard shortcut for saving and loading levels
	CheckSaveLevel();
	CheckLoadLevel();

	if (_gameStarted)
	{
		//_renderReference = false;
		_secReferenceScrollBar = _time.getElapsedTime().asSeconds();
		_currentLevel->RunLevel(&_time.getElapsedTime());
	}
	else
	{
		_secReferenceScrollBar = _scrollBar;
		_currentLevel->RunLevel(&_secReferenceScrollBar);
	}
	_currentSnapTime = _secReferenceScrollBar;//Todo improve
	UpdateScrolling(pStep);
	UpdateWaveSelection();
	UpdateWaypointCreation();
	UpdateStartLevelPreview();
	UpdateGUIData();
}
#pragma region UI Methods
void LevelEditorBehaviour::DrawUI()
{
	//_images.back()->draw();

	DrawGrid();
	//if (_renderReference)
	DrawReferenceGrid();
	//Add Indicator to know which waypoints are from which wave 
	_currentLevel->DrawAllWavesWayPoints();
}
//Draw Blue rectangles at the sides
void LevelEditorBehaviour::DrawGrid()
{
	if (_window != NULL) {
		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(400, -10000)),
			sf::Vertex(sf::Vector2f(400, 1080)),
			sf::Vertex(sf::Vector2f(1520,-10000)),
			sf::Vertex(sf::Vector2f(1520, 1080))
		};
		_window->pushGLStates();
		_window->draw(line, 4, sf::Lines);
		_window->popGLStates();

		sf::RectangleShape rectangle;
		rectangle.setSize(sf::Vector2f(400, 10000));
		rectangle.setFillColor(sf::Color(0, 141, 250, 100));
		rectangle.setOutlineThickness(1);
		rectangle.setPosition(0, 0);
		draw(rectangle);

		sf::RectangleShape rectangle2;
		rectangle2.setSize(sf::Vector2f(400, 10000));
		rectangle2.setFillColor(sf::Color(0, 141, 250, 100));
		rectangle2.setOutlineThickness(1);
		rectangle2.setPosition(1520, 0);
		draw(rectangle2);
	}
}
//Draw the current SnapTime of the level
void LevelEditorBehaviour::DrawReferenceGrid()
{
	//TODO:
	//Optimize drawing
	glActiveTexture(GL_TEXTURE0);
	_window->pushGLStates();
	/////////START DRAWING//////////////
	_currentSnapTimeText.setString("Sec: " + std::to_string(_secReferenceScrollBar));
	_window->draw(_currentSnapTimeText);
	sf::Vector2f position;
	//string time = std::to_string(_time.getElapsedTime().asSeconds());;
	//std::string debug_time = "TIME: " + time;

	for (auto &text : _textReference) {
		position = text.getPosition();
		position.y = (float)std::stoi((std::string) text.getString()); //String to int		
		position.y = 1080 - position.y + _scrollBar*100; //1080 is height of the screen resolution
		text.setPosition(position);	
		//text.setString(text.getString());
		_window->draw(text);
	}
	////////END DRAWING////////////////////
	_window->popGLStates();
}
#pragma endregion
#pragma region Update Specific Methods
//Scrolling method of the preview of the level editor game
glm::vec3 LevelEditorBehaviour::getScreenToWorldPos(sf::Vector2f pScreenPos)
{
	sf::Vector2u windowSize = _window->getSize();
	glm::vec2 mousePosRelativeToScreenCenter = glm::vec2(
		(float)pScreenPos.x - (windowSize.x / 2),
		(float)-pScreenPos.y + (windowSize.y / 2)
	);
	float nearPlane = 0.1f;     //taken from Camera.hpp
	float verticalFOV = 60.0f;  //taken from Camera.hpp
	float nearPlaneHeight = 2 * nearPlane * tan(glm::radians(verticalFOV / 2.0f));
	float ratio = nearPlaneHeight / windowSize.y;

	glm::vec4 rayNearPlane = glm::vec4(
		mousePosRelativeToScreenCenter.x * ratio,
		mousePosRelativeToScreenCenter.y * ratio,
		-nearPlane,
		0
	);

	glm::vec3 rayWorld = glm::normalize(glm::vec3(_world->getMainCamera()->getWorldTransform() * rayNearPlane))*100;
	//cout << rayWorld.x << "x RAYCAST" << endl;
	return rayWorld;
}
void LevelEditorBehaviour::UpdateScrolling(float pstep)
{
	
	if (_autoScroll)
	{
		_scrollBar += _levelEditorTime.restart().asSeconds();

	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		_scrollBar += _levelEditorTime.restart().asSeconds();
		if (!referenceToWorldCheck) {
			setReferenceWorld();
			referenceToWorldCheck = true;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		_scrollBar -= _levelEditorTime.restart().asSeconds();
	}
	else
		_levelEditorTime.restart();

	if (!_autoScroll && sf::Keyboard::isKeyPressed(sf::Keyboard::P) && !_scrollKeyPressed)
	{
		_autoScroll = true;
		_scrollKeyPressed = true;
	}
	else if (_autoScroll && sf::Keyboard::isKeyPressed(sf::Keyboard::P) && !_scrollKeyPressed)
	{
		_autoScroll = false;
		_scrollKeyPressed = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::P) == false) _scrollKeyPressed = false;



}

void LevelEditorBehaviour::UpdateWaypointCreation()
{
	if (sf::Mouse::getPosition().x > 400 && sf::Mouse::getPosition().x < 1520 && _window->isOpen())//check if we are in screen and just if window is open
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::U))//Check if click
		{
			if (!_mousePressed) {//If we weren´t clicking before then add waypoint
				sf::RenderWindow &window = *_window;
				sf::Vector2f pixelPos =(sf::Vector2f) sf::Mouse::getPosition(window);			
				sf::Vector2f screenPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

				glm::vec3 worldPos = getScreenToWorldPos(screenPos);
				_currentLevel->CreateWaypoint(worldPos, screenPos, _currentSnapTime);
				//cout << "Mouse x: " << sf::Mouse::getPosition(window).x << "Mouse y: " << sf::Mouse::getPosition(window).y << endl;
			}
			_mousePressed = true; //save that we just pressed the mouse
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::U)){
			if (!_mousePressed) {//If we weren´t clicking before then add waypoint
				sf::RenderWindow &window = *_window;

				sf::Vector2f pixelPos = (sf::Vector2f) sf::Mouse::getPosition(window);

				sf::Vector2f screenPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

				glm::vec3 worldPos = getScreenToWorldPos(screenPos);
				_currentLevel->CreateMainWaypointMoveDirection(worldPos, screenPos, _currentSnapTime);
				//cout << "Mouse x: " << sf::Mouse::getPosition(window).x << "Mouse y: " << sf::Mouse::getPosition(window).y << endl;
			}
			_mousePressed = true; //save that we just pressed the mouse
		}
		else _mousePressed = false; // reset the mouse pressed
	}
}

void LevelEditorBehaviour::UpdateStartLevelPreview()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && !_gameStarted)
	{
		setReferenceWorld();
		_gameStarted = true;
	}
	
}

void LevelEditorBehaviour::UpdateWaveSelection()
{
	//Cache if press a key Num1
	if (!_numKeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
	{
		NewWave();
		_numKeyPressed = true;
	}
	//Next Num3
	else if (!_numKeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Num3) && !_gameStarted)
	{
		NextWave();
		_numKeyPressed = true;
	}
	//Previous Num2
	else if (!_numKeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) && !_gameStarted)
	{
		PrevWave();
		_numKeyPressed = true;
	}
	else if( !_numKeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::M) && !_gameStarted)
	{
		ClearWave();
	}
	//Restart cache of keypressed
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) &&
		!sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) &&
		!sf::Keyboard::isKeyPressed(sf::Keyboard::Num3) &&
		!sf::Keyboard::isKeyPressed(sf::Keyboard::M))
	{
		_numKeyPressed = false;
	}

}

#pragma endregion
#pragma region Parser Methods
void LevelEditorBehaviour::CheckSaveLevel()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
	{
		SaveLevel();
	}
}
void LevelEditorBehaviour::CheckLoadLevel()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
	{
		LoadLevel();
	}
}
void LevelEditorBehaviour::SaveLevel()
{
	cout << "Creating XML..." << endl;
	LevelParser::SaveLevel(_currentLevel,std::to_string( _indexLevel));
	cout << "Saved XML" << endl;
}
void LevelEditorBehaviour::LoadLevel()
{
	cout << "Reading XML..." << endl;
	Level* level = LevelParser::LoadLevel(std::to_string(_indexLevel), _window);
	_currentLevel = level;
	_currentLevel->ReferenceWorld(_world, _owner);
	_currentWave = _currentLevel->getIndexWave();
	cout << "Done Reading XML..." << endl;

}
//Load all the levels we can find
void LevelEditorBehaviour::LoadLevelInitialize()
{
	for (int i = 0;i < 100;i++)
	{
		
		cout << "Reading XML..." << endl;
		Level* level = LevelParser::LoadLevel(std::to_string(i), _window);
		if (level == NULL)
		{
			cout << "No more levels founded" << endl;
			return;
		}
		_levels.push_back(level);
		_indexLevel = _levels.size() - 1;
		_currentLevel = _levels.at(_indexLevel);
		_currentLevel->ReferenceWorld(_world, _owner);
		_currentWave = _currentLevel->getIndexWave();
		cout << "Done Reading XML..."<<_indexLevel << endl;
		
	}
	UpdateGUIDataAtWaveChange();
}

#pragma endregion
void LevelEditorBehaviour::setActive(bool pActive)
{
	_active = pActive;
	if (!_active)
		_panel->hide();
	else
		_panel->show();
}
bool LevelEditorBehaviour::getActive()
{
	return _active;
}
#pragma region Draw shapes NOT USING
//Draw a circle shape, need optimizing, not using when this comment was created
void LevelEditorBehaviour::draw(sf::CircleShape pCircle)
{
	glActiveTexture(GL_TEXTURE0);
	_window->pushGLStates();
	_window->draw(pCircle);
	_window->popGLStates();

}

void LevelEditorBehaviour::draw(sf::RectangleShape pRectangle)
{
	glActiveTexture(GL_TEXTURE0);
	_window->pushGLStates();
	_window->draw(pRectangle);
	_window->popGLStates();

}
void LevelEditorBehaviour::DotGrid()
{
	for (int i = 0; i < 20;i++)
	{
		for (int j = 0; j < 20;j++)
		{
			sf::CircleShape shape(5);
			shape.setFillColor(sf::Color(250, 250, 50));
			shape.setPosition(sf::Vector2f(i * 50 + 500.0f, j*50.0f));
			draw(shape);
		}
	}

}
#pragma endregion
