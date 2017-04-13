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
	:_worldWaypointPosition(pWorldWayPos),_screenWaypointPosition(pScreenWayPos),_positionInWaveList(pOrderInList), _waveIndex(pWaveIndex)
{
	_window = pWindow;
	_createDebugInfo();
	//cout << pWorldWayPos.x << endl;
}

Waypoint::~Waypoint()
{
	
}

void Waypoint::CreateWaypoint(sf::Vector2i pWayPos)
{
	
}

void Waypoint::Draw()
{	
	if (this != nullptr) {
		glActiveTexture(GL_TEXTURE0);
		_window->pushGLStates();
		_window->draw(_shape);
		_window->draw(_debugText);
		_window->popGLStates();
	}
}
void Waypoint::MainWaypoint()
{
	//Check if SpawnPoint
	if (_positionInWaveList == 0 && _color != sf::Vector3i(255, 215, 0))
	{
		_color = sf::Vector3i(255, 215, 0);
		_shape.setFillColor(sf::Color(255, 215, 0));//Light Red
	}
	else if (_positionInWaveList != 0 &&_color != sf::Vector3i(0, 128, 0))
	{
		_color = sf::Vector3i(0, 128, 0);
		_shape.setFillColor(sf::Color(0, 128, 0));//Light Blue
	}	
	if (!_mainWaypoint)
	{
		_mainWaypoint = true;
		_shape.setRadius(7);
	}

}
void Waypoint::SecondaryWaypoint()
{
	//Check if spawn point
	if (_positionInWaveList == 0 && _color != sf::Vector3i(205, 92, 92))
	{
		_color = sf::Vector3i(205, 92, 92);
		_shape.setFillColor(sf::Color(205, 92, 92));//Light Red
	}
	else if (_color != sf::Vector3i(30, 144, 255))
	{
		_color = sf::Vector3i(30, 144, 255);
		_shape.setFillColor(sf::Color(30, 144, 255));//Light Blue
	}
	if (_mainWaypoint)
	{
		_mainWaypoint = false;
		_shape.setRadius(5);
	}
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

void Waypoint::setScreenPosition(sf::Vector2f pScreenPos)
{
	_screenWaypointPosition = pScreenPos;
	_shape.setPosition(pScreenPos);
	_debugText.setPosition(pScreenPos);
}

void Waypoint::setWorldPos(glm::vec3 pWorldPos)
{
	_worldWaypointPosition = pWorldPos;
}

sf::Shape * Waypoint::getShape()
{
	return &_shape;
}

void Waypoint::setIndexInsideWave(int pIndex)
{
	_positionInWaveList = pIndex;
	_debugInfo = std::to_string(_positionInWaveList) + "\n W: " + std::to_string(_waveIndex);
	_debugText.setString(_debugInfo);
}

void Waypoint::setDragging(bool pBool)
{
	_dragging = pBool;
}
bool Waypoint::getDragging()
{
	return _dragging;
}
void Waypoint::_createDebugInfo()
{
	if (!_font.loadFromFile(config::MGE_FONT_PATH + "arial.ttf")) {
		cout <<_font.getInfo().family<< "Could not load font, exiting..." << endl;
		
	}

	_shape.setPosition((sf::Vector2f)_screenWaypointPosition);
	_debugText = sf::Text();
	_shape.setRadius(5);

	if(_positionInWaveList == 0) _shape.setFillColor(sf::Color(205, 92, 92));//Light Red
	else _shape.setFillColor(sf::Color(30, 144, 255));//Light Blue

	_debugText.setFont(_font);
	_debugText.setCharacterSize(16);
	_debugText.setFillColor(sf::Color::White);


	_debugInfo = std::to_string(_positionInWaveList )+ "\n W: "+ std::to_string(_waveIndex);
	_debugText.setString(_debugInfo);

	_debugText.setPosition((sf::Vector2f)_screenWaypointPosition);
}
