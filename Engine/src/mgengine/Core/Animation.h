#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>

class Animation {
public:							
							Animation(sf::RenderWindow* pWindow, const sf::Texture& pTexture, sf::Vector2i pFramesSize, std::size_t pNumFrames, float pDuration);
	
	const sf::Texture* 		getTexture() const;

	void 					setFrameSize(sf::Vector2i pFrameSize);
	sf::Vector2i		 	getFrameSize() const;

	void 					setNumFrames(std::size_t pNumFrames);
	std::size_t 			getNumFrames() const;

	void 					setDuration(float pDuration);
	float 					getDuration() const;

	void 					setRepeating(bool pFlag);
	bool 					isRepeating() const;

	void					setPosition(sf::Vector2f pPosition);
	void					setScale(sf::Vector2f pScale);

	void 					restart();
	bool 					isFinished() const;

	sf::FloatRect 			getLocalBounds() const;	
		
	void					update(float dt);	
private:

	void			_draw();
	void			_updateFrames(float dt);

	sf::Sprite		_sprite;

	sf::Vector2i 	_frameSize;
	std::size_t 	_numFrames;
	std::size_t 	_currentFrame;

	float 			_duration;
	float 			_elapsedTime;
	bool			_repeat;

	sf::RenderWindow* _window;
};

#endif // !ANIMATION_H
