#pragma once
#include <list>
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics.hpp>
#include "mge\core\Camera.hpp"
class Waypoint
{
public:
	//Waypoint(sf::Vector2f pWayPos, int pOrderInList, int pWaveIndex, sf::RenderWindow *pWindow);
	//Waypoint(sf::Vector2f pWayPos, int pOrderInList, int pWaveIndex, sf::RenderWindow * pWindow, Camera * pCamera);
	Waypoint(glm::vec3 pWorldWayPos, sf::Vector2f pWcreenWayPos, int pOrderInList, int pWaveIndex, sf::RenderWindow * pWindow);
	~Waypoint();
	void CreateWaypoint(sf::Vector2i pWayPos);
	void Draw();
	void MainWaypoint();
	void SecondaryWaypoint();
	sf::RenderWindow * getRenderWindow();
	//std::vector<sf::CircleShape> _wayPoints;
	sf::Vector2f getScreenPosition();
	glm::vec3 getWorldPos();
private:
	sf::Vector2f _screenWaypointPosition;
	glm::vec3 _worldWaypointPosition = glm::vec3(0,0,0);
	int _orderInList;
	std::string _debugInfo;
	sf::Font _font;
	sf::Text _debugText;
	void _createDebugInfo();
	sf::RenderWindow* _window;
	sf::CircleShape shape;
	int _waveIndex;
	Camera* _camera;
	bool _mainWaypoint = true;
};

