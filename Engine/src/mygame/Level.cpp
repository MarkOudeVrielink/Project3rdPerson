#include "Level.h"

Level::Level()
{
}


Level::~Level()
{
}

void Level::CreateWaypoint(sf::Vector2i pWayPos)
{
	sf::CircleShape shape(5);
	// set the shape color to green
	//shape.setOrigin(5 / 2.0f,0);
	shape.setFillColor(sf::Color(250, 0, 50));
	shape.setPosition((sf::Vector2f)pWayPos);
	_wayPoints.push_back(shape);
}
