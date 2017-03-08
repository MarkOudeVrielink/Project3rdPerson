#include "mgengine\Core\Animation.h"

#include <SFML/Graphics/Texture.hpp>
#include <iostream>
Animation::Animation(sf::RenderWindow* pWindow, const sf::Texture & pTexture, sf::Vector2i pFramesSize, std::size_t pNumFrames, float pDuration) :
	_window(pWindow), _sprite(pTexture), _frameSize(pFramesSize), _numFrames(pNumFrames), _duration(pDuration), _currentFrame(0), _elapsedTime(0), _repeat(true)
{
	_sprite.setOrigin(_frameSize.x/2, _frameSize.y/2);
}

void Animation::update(float dt)
{
	_updateFrames(dt);
	_draw();
}

void Animation::_draw()
{
	_window->pushGLStates();
	_window->draw(_sprite);
	_window->popGLStates();
}

void Animation::_updateFrames(float dt)
{

	float timePerFrame = _duration / static_cast<float>(_numFrames);
	_elapsedTime += dt;

	sf::Vector2i textureBounds(_sprite.getTexture()->getSize());
	sf::IntRect textureRect = _sprite.getTextureRect();

	if (_currentFrame == 0)
		textureRect = sf::IntRect(0, 0, _frameSize.x, _frameSize.y);

	// While we have a frame to process
	while (_elapsedTime >= timePerFrame && (_currentFrame <= _numFrames || _repeat))
	{
		// Move the texture rect left
		textureRect.left += textureRect.width;

		// If we reach the end of the texture
		if (textureRect.left + textureRect.width > textureBounds.x)
		{
			// Move it down one line
			textureRect.left = 0;
			textureRect.top += textureRect.height;
		}

		// And progress to next frame
		_elapsedTime -= timePerFrame;
		if (_repeat)
		{
			_currentFrame = (_currentFrame + 1) % _numFrames;

			if (_currentFrame == 0)
				textureRect = sf::IntRect(0, 0, _frameSize.x, _frameSize.y);
		}
		else
		{
			_currentFrame++;
		}
	}

	_sprite.setTextureRect(textureRect);
}

const sf::Texture * Animation::getTexture() const
{
	return _sprite.getTexture();
}

void Animation::setFrameSize(sf::Vector2i pFrameSize)
{
	_frameSize = pFrameSize;
}

sf::Vector2i Animation::getFrameSize() const
{
	return _frameSize;
}

void Animation::setNumFrames(std::size_t pNumFrames)
{
	_numFrames = pNumFrames;
}

std::size_t Animation::getNumFrames() const
{
	return _numFrames;
}

void Animation::setDuration(float pDuration)
{
	_duration = pDuration;
}

float Animation::getDuration() const
{
	return _duration;
}

void Animation::setRepeating(bool pFlag)
{
	_repeat = pFlag;
}

bool Animation::isRepeating() const
{
	return _repeat;
}

void Animation::setPosition(sf::Vector2f pPosition)
{
	sf::Vector2u screen = _window->getSize();

	_sprite.setPosition(pPosition.x + screen.x/2, pPosition.y + screen.y/2);
}

void Animation::setScale(sf::Vector2f pScale)
{
	_sprite.setScale(pScale);
}

void Animation::restart()
{
	_currentFrame = 0;
}

bool Animation::isFinished() const
{
	return _currentFrame >= _numFrames;
}

sf::FloatRect Animation::getLocalBounds() const
{
	return sf::FloatRect(_sprite.getOrigin(), static_cast<sf::Vector2f>(getFrameSize()));
}

