#pragma once
#include "mge/core/Texture.hpp"
#include "mge/behaviours/AbstractBehaviour.hpp"
#include <iostream>
#include <glm.hpp>
#include <SFML/Graphics.hpp>

class RawImage 
{
public:
	RawImage(std::string pPath, glm::vec2 pPosition, sf::RenderWindow *pWindow);
	~RawImage();
	void draw();
private:
	std::string _path;
	glm::vec2 _pos;
	sf::Texture _texture;
	sf::RenderWindow *_window;
	sf::Sprite _sprite;
	sf::Text _debugText;
};

