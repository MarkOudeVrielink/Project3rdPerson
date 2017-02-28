#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <SFML\Audio.hpp>
#include <SFML\Audio\SoundBuffer.hpp>
#include <glm.hpp>
#include <list> 

#include "mgengine\Resources\ResourceIdentifiers.h"
#include "mgengine\Resources\SFMLResourceHolder.h"

class SoundPlayer : private sf::NonCopyable{
public:
				SoundPlayer();
	void		Play(SoundEffect::ID pEffect);
	void		RemoveStoppedSound();

private:
	SoundBufferHolder		_soundBuffers;
	std::list<sf::Sound>	_sounds;
};
#endif // !SOUNDPLAYER_H
