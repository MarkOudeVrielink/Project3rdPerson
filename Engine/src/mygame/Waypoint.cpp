#include "Waypoint.h"
#include "mge/config.hpp"
#include <iostream>
#include <glm.hpp>
#include <SFML\Graphics.hpp>
#include <GL/glew.h>
#include <cassert>
#include <string> 
using namespace std;
Waypoint::Waypoint(sf::Vector2f pWayPos, int pOrderInList, int pWaveIndex, sf::RenderWindow* pWindow):_waypointPosition(pWayPos),_orderInList(pOrderInList), _waveIndex(pWaveIndex)
{
	_window = pWindow;
	_createDebugInfo();
}

Waypoint::~Waypoint()
{
}

void Waypoint::CreateWaypoint(sf::Vector2i pWayPos)
{
}

void Waypoint::Draw()
{	
	glActiveTexture(GL_TEXTURE0);
	_window->pushGLStates();
	_window->draw(shape);
	_window->draw(_debugText);
	_window->popGLStates();
	

	
}

sf::Vector2f Waypoint::getPosition()
{
	return (sf::Vector2f)_waypointPosition;
}

void Waypoint::_createDebugInfo()
{
	if (!_font.loadFromFile(config::MGE_FONT_PATH + "arial.ttf")) {
		cout << "Could not load font, exiting..." << endl;
		
	}

	shape.setPosition((sf::Vector2f)_waypointPosition);
	_debugText = sf::Text();
	shape.setRadius(5);

	if(_orderInList == 0) shape.setFillColor(sf::Color::Red);
	else shape.setFillColor(sf::Color::Blue);

	_debugText.setFont(_font);
	_debugText.setCharacterSize(16);
	_debugText.setFillColor(sf::Color::White);


	_debugInfo = std::to_string(_orderInList )+ "\n W: "+ std::to_string(_waveIndex);
	_debugText.setString(_debugInfo);

	_debugText.setPosition((sf::Vector2f)_waypointPosition);
}