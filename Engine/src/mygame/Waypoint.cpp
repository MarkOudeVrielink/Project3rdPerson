#include "Waypoint.h"
#include "mge/config.hpp"
#include <iostream>
#include <glm.hpp>
#include <SFML\Graphics.hpp>
#include <GL/glew.h>
#include <cassert>
#include <string> 

using namespace std;
Waypoint::Waypoint(glm::vec3 pWorldWayPos, sf::Vector2f pScreenWayPos, int pOrderInList, int pWaveIndex, sf::RenderWindow* pWindow)
	:_worldWaypointPosition(pWorldWayPos),_screenWaypointPosition(pScreenWayPos),_orderInList(pOrderInList), _waveIndex(pWaveIndex)
{
	_window = pWindow;
	_createDebugInfo();
	cout << pWorldWayPos.x << endl;
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
void Waypoint::MainWaypoint()
{
	if (_orderInList == 0) shape.setFillColor(sf::Color(255, 215, 0));
	else shape.setFillColor(sf::Color(0, 128, 0));
	shape.setRadius(7);

}
void Waypoint::SecondaryWaypoint()
{
	if (_orderInList == 0) shape.setFillColor(sf::Color(205, 92, 92));//Light Red
	else shape.setFillColor(sf::Color(30, 144, 255));//Light Blue
	shape.setRadius(5);
}
sf::RenderWindow * Waypoint::getRenderWindow()
{
	return _window;
}
sf::Vector2f Waypoint::getScreenPosition()
{
	return (sf::Vector2f)_screenWaypointPosition;
}
//Expensive(?)
glm::vec3 Waypoint::getWorldPos()
{
	return _worldWaypointPosition;
}

void Waypoint::_createDebugInfo()
{
	if (!_font.loadFromFile(config::MGE_FONT_PATH + "arial.ttf")) {
		cout << "Could not load font, exiting..." << endl;
		
	}

	shape.setPosition((sf::Vector2f)_screenWaypointPosition);
	_debugText = sf::Text();
	shape.setRadius(5);

	if(_orderInList == 0) shape.setFillColor(sf::Color(205, 92, 92));//Light Red
	else shape.setFillColor(sf::Color(30, 144, 255));//Light Blue

	_debugText.setFont(_font);
	_debugText.setCharacterSize(16);
	_debugText.setFillColor(sf::Color::White);


	_debugInfo = std::to_string(_orderInList )+ "\n W: "+ std::to_string(_waveIndex);
	_debugText.setString(_debugInfo);

	_debugText.setPosition((sf::Vector2f)_screenWaypointPosition);
}
