#include "LevelEditor.h"
#include <SFML/Window/Mouse.hpp>
#include <SFML\System\Vector2.hpp>
#include <iostream>
#include <glm.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\System\Vector2.hpp>
using namespace std;
LevelEditorBehaviour::LevelEditorBehaviour(sf::RenderWindow *pWindow, World *pWorld) :AbstractBehaviour()
{
	_window = pWindow;
	_images.push_back(new RawImage(config::MGE_TEXTURE_PATH +
		"bricks.jpg", glm::vec2(50, 50), _window));
	_currentLevel = new Level(_window);
	_world = pWorld;

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
		counter++;
		if (counter == 100)
		{
			sf::Text text = sf::Text();

			text.setFont(_font);
			text.setCharacterSize(16);
			text.setFillColor(sf::Color::White);


			text.setString(std::to_string(i));
			text.setPosition(sf::Vector2f(350, 1080 - i));
			glActiveTexture(GL_TEXTURE0);
			_window->pushGLStates();
			_window->draw(text);
			_window->popGLStates();
			counter = 0;
			_textReference.push_back(text);

		}

	}
	StartGame();

}


LevelEditorBehaviour::~LevelEditorBehaviour()
{
}

void LevelEditorBehaviour::DrawUI()
{
	_images.back()->draw();

	DrawGrid();
	DrawReferenceGrid();
	//Add Indicator to know which waypoints are from which wave 
	_currentLevel->DrawAllWavesWayPoints();


}
void LevelEditorBehaviour::DrawGrid()
{
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
	rectangle.setFillColor(sf::Color(0 , 141 , 250,255));
	rectangle.setOutlineThickness(1);
	rectangle.setPosition(0, 0);
	draw(rectangle);

	sf::RectangleShape rectangle2;
	rectangle2.setSize(sf::Vector2f(400, 10000));
	rectangle2.setFillColor(sf::Color(0, 141, 250, 255));
	rectangle2.setOutlineThickness(1);
	rectangle2.setPosition(1520, 0);
	draw(rectangle2);

}
void LevelEditorBehaviour::DrawReferenceGrid()
{
	//TODO:
	//Optimize drawing
	glActiveTexture(GL_TEXTURE0);
	_window->pushGLStates();
	/////////START DRAWING//////////////
	_currentSnapTimeText.setString("Sec: " + std::to_string(_scrollBar/60.0f));
	_window->draw(_currentSnapTimeText );
	sf::Vector2f position;
	std::string::size_type sz;
	for (auto &text : _textReference) {
		position = text.getPosition();
		position.y = std::stoi((std::string) text.getString()); //String to int		
		position.y = 1080 - position.y + _scrollBar; //1080 is height of the screen resolution
		text.setPosition(position);
		std::string debug_time = "TIME: " + std::to_string(_time.getElapsedTime().asSeconds());
		text.setString(text.getString() );
		_window->draw(text );
	}
	////////END DRAWING////////////////////
	_window->popGLStates();
}

void LevelEditorBehaviour::StartGame()
{
	_currentLevel->StartLevel(_world);
	
}
void LevelEditorBehaviour::update(float pStep)
{
	
	if (_gameStarted)
		_currentLevel->RunLevel(&_time.getElapsedTime());
	else
	{
		_secReferenceScrollBar = _scrollBar / 60.0f;
		_currentLevel->RunLevel(&_secReferenceScrollBar);
	}
	UpdateScrolling();
	UpdateWaveSelection();
	UpdateWaypointCreation();	
	UpdateStartLevelPreview();

}
//Scrolling method of the preview of the level editor game
void LevelEditorBehaviour::UpdateScrolling()
{
	if (_autoScroll)
	{
		_scrollBar += _scrollSpeed;

	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		_scrollBar += _scrollSpeed;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		_scrollBar -= _scrollSpeed;
	}

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
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))//Check if click
	{
		if (!_mousePressed) {//If we weren´t clicking before then add waypoint
			sf::RenderWindow &window = *_window;
			
			sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
			sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
			_currentLevel->CreateWaypoint(worldPos, _currentSnapTime);
			//cout << "Mouse x: " << sf::Mouse::getPosition(window).x << "Mouse y: " << sf::Mouse::getPosition(window).y << endl;
		}
		_mousePressed = true; //save that we just pressed the mouse
	}
	else _mousePressed = false; // reset the mouse pressed
}

void LevelEditorBehaviour::UpdateStartLevelPreview()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && !_gameStarted)
	{
		StartGame();
		_gameStarted = true;
	}
}

void LevelEditorBehaviour::UpdateWaveSelection()
{
	//Cache if press a key Num1
	if (!_numKeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
	{
		_currentLevel->NewWave();
		_numKeyPressed = true;
	}
	//Next Num3
	else if (!_numKeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Num3) && !_gameStarted)
	{
		_currentLevel->NextEnemyWave();
		_numKeyPressed = true;
	}
	//Previous Num2
	else if (!_numKeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) && !_gameStarted)
	{
		_currentLevel->PreviousWave();
		_numKeyPressed = true;
	}
	//Restart cache of keypressed
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) &&
		!sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) &&
		!sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
	{
		_numKeyPressed = false;
	}

}


//TODO: Add parser 
void LevelEditorBehaviour::SaveLevel()
{
}
void LevelEditorBehaviour::LoadLevel()
{
}
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