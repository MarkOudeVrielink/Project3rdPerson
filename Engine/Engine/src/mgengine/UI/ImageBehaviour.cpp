#include "ImageBehaviour.h"
#include "mge/config.hpp"
#include "mge/core/Texture.hpp"
#include <SFML/Graphics.hpp>
using namespace std;


RawImage::RawImage(std::string pPath, glm::vec2 pPosition, sf::RenderWindow *pWindow)
{
	_path = pPath;
	_pos = pPosition;
	_window = pWindow;

	cout << "Image Loaded"<< endl;
	_texture.loadFromFile(_path);	
	_sprite.setTexture(_texture, true);
}

RawImage::~RawImage()
{
}
void RawImage::draw()
{
	_sprite.setPosition((sf::Vector2f(_pos.x,_pos.y)));
	glActiveTexture(GL_TEXTURE0);
	_window->pushGLStates();
	_window->draw(_sprite);
	_window->popGLStates();
	
}
