#pragma once
#include <list>
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics.hpp>

class Waypoint
{
public:
	Waypoint(sf::Vector2f pWayPos, int pOrderInList, int pWaveIndex, sf::RenderWindow *pWindow);
	~Waypoint();
	void CreateWaypoint(sf::Vector2i pWayPos);
	void Draw();
	//std::vector<sf::CircleShape> _wayPoints;
	sf::Vector2f getPosition();
private:
	sf::Vector2f _waypointPosition;
	int _orderInList;
	std::string _debugInfo;
	sf::Font _font;
	sf::Text _debugText;
	void _createDebugInfo();
	sf::RenderWindow* _window;
	sf::CircleShape shape;
	int _waveIndex;
};

