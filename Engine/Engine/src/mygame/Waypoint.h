#pragma once
#include <list>
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics.hpp>
#include "mge\core\Camera.hpp"
class Waypoint
{
public:
	Waypoint(glm::vec3 pWorldWayPos, sf::Vector2f pWcreenWayPos, int pOrderInList, int pWaveIndex, sf::RenderWindow * pWindow);
	Waypoint(glm::vec3 pWorldWayPos, sf::Vector2f pWcreenWayPos, int pOrderInList, int pWaveIndex, sf::Font pFont, sf::RenderWindow * pWindow);

	~Waypoint();
	void CreateWaypoint(sf::Vector2i pWayPos);
	void Draw();
	void MainWaypoint();
	void SecondaryWaypoint();
	sf::RenderWindow * getRenderWindow();
	sf::Vector2f getScreenPosition();
	glm::vec3 getWorldPos();
	void setScreenPosition(sf::Vector2f pScreenPos);
	void setWorldPos(glm::vec3 pWorldPos);
	sf::Shape * getShape();

	void setIndexInsideWave(int pIndex);

	void setDragging(bool pBool);
	bool getDragging();



	void setPosition(sf::Vector2f pWcreenWayPos);
private:
	bool _dragging = false;
	sf::Vector2f _screenWaypointPosition;
	glm::vec3 _worldWaypointPosition = glm::vec3(0,0,0);
	sf::Vector3i _color = sf::Vector3i(0, 0, 0);
	int _positionInWaveList;
	std::string _debugInfo;
	sf::Font _font;
	sf::Text _debugText;
	void _createDebugInfo();
	sf::RenderWindow* _window;
	sf::CircleShape _shape;
	int _waveIndex;
	Camera* _camera;
	bool _mainWaypoint = true;
};

