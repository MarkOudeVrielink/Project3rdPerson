#include "LevelEditor.h"
#include <SFML/Window/Mouse.hpp>
#include <SFML\System\Vector2.hpp>
#include <iostream>
#include <glm.hpp>
#include <SFML\Graphics.hpp>
using namespace std;
LevelEditorBehaviour::LevelEditorBehaviour(sf::RenderWindow *pWindow):AbstractBehaviour()
{
	_window = pWindow;
	_images.push_back(new RawImage(config::MGE_TEXTURE_PATH +
		"bricks.jpg", glm::vec2(50, 50), _window));
	_currentLevel = new Level();
}


LevelEditorBehaviour::~LevelEditorBehaviour()
{
}
void LevelEditorBehaviour::draw( sf::CircleShape pCircle)
{
	glActiveTexture(GL_TEXTURE0);
	_window->pushGLStates();
	_window->draw(pCircle);
	_window->popGLStates();

}

void LevelEditorBehaviour::DrawUI()
{
	_images.back()->draw();
	DrawGrid();
		for (auto &waypoint : _currentLevel->_wayPoints) 
		{
			draw(waypoint);
		}
		
}
void LevelEditorBehaviour::DrawGrid()
{
	for (int i = 0; i < 20;i++)
	{
		for (int j = 0; j < 20;j++)
		{
			sf::CircleShape shape(5);
			shape.setFillColor(sf::Color(250, 250, 50));
			shape.setPosition(sf::Vector2f(i*50+500.0f,j*50.0f));
			draw(shape);
		}
	}

}
void LevelEditorBehaviour::SaveLevel()
{
}
void LevelEditorBehaviour::LoadLevel()
{
}

void LevelEditorBehaviour::update(float pStep)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		sf::RenderWindow &window = *_window;
		_currentLevel->CreateWaypoint(sf::Mouse::getPosition(window));
		cout << "Mouse x: "<<sf::Mouse::getPosition(window).x << "Mouse y: " << sf::Mouse::getPosition(window).y << endl;
	}
}
