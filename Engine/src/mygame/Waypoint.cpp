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

sf::Vector2f Waypoint::getPosition()
{
	return (sf::Vector2f)_waypointPosition;
}
sf::Vector2f Waypoint::getWorldPos()
{

	return sf::Vector2f (192.0f / 1920 * _waypointPosition.x - 96,  108.0f / 1080 * _waypointPosition.y - 54);
}

void Waypoint::_createDebugInfo()
{
	if (!_font.loadFromFile(config::MGE_FONT_PATH + "arial.ttf")) {
		cout << "Could not load font, exiting..." << endl;
		
	}

	shape.setPosition((sf::Vector2f)_waypointPosition);
	_debugText = sf::Text();
	shape.setRadius(5);

	if(_orderInList == 0) shape.setFillColor(sf::Color(205, 92, 92));//Light Red
	else shape.setFillColor(sf::Color(30, 144, 255));//Light Blue

	_debugText.setFont(_font);
	_debugText.setCharacterSize(16);
	_debugText.setFillColor(sf::Color::White);


	_debugInfo = std::to_string(_orderInList )+ "\n W: "+ std::to_string(_waveIndex);
	_debugText.setString(_debugInfo);

	_debugText.setPosition((sf::Vector2f)_waypointPosition);
}
