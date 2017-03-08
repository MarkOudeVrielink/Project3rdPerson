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
	/*
	sf::Vector2u windowSize = _window->getSize();
	glm::vec2 mousePosRelativeToScreenCenter = glm::vec2(
		(float)_screenWaypointPosition.x - (windowSize.x / 2),
		(float)-_screenWaypointPosition.y + (windowSize.y / 2)
	);
	float nearPlane = 0.1f;     //taken from Camera.hpp
	float verticalFOV = 45.0f;  //taken from Camera.hpp
	float nearPlaneHeight = 2 * nearPlane * tan(glm::radians(verticalFOV / 2.0f));
	float ratio = nearPlaneHeight / windowSize.y;

	glm::vec4 rayNearPlane = glm::vec4(
		mousePosRelativeToScreenCenter.x * ratio,
		mousePosRelativeToScreenCenter.y * ratio,
		-nearPlane,
		0
	);
	glm::vec3 rayWorld = glm::normalize(glm::vec3(_camera->getWorldTransform() * rayNearPlane));
	*/
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
