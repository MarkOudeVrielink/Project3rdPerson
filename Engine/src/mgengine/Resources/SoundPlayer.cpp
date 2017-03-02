#include "mgengine\Resources\SoundPlayer.h"
#include "mge\config.hpp"

SoundPlayer::SoundPlayer() : _soundBuffers(), _sounds()
{
	//TODO: Load the sounds.
	_soundBuffers.load(SoundEffect::Player_Shoot,	config::MGE_SOUND_PATH + "Player_Shoot.wav");
	_soundBuffers.load(SoundEffect::Player_Hit,		config::MGE_SOUND_PATH + "Player_Hit.wav");
	_soundBuffers.load(SoundEffect::Drop_Pick,		config::MGE_SOUND_PATH + "Drop_Pick.wav");
	_soundBuffers.load(SoundEffect::Enemy_Death_1,	config::MGE_SOUND_PATH + "Enemy_Death_1_sfx.wav");
	_soundBuffers.load(SoundEffect::Enemy_Death_2,	config::MGE_SOUND_PATH + "Enemy_Death_2_sfx.wav");
	_soundBuffers.load(SoundEffect::Enemy_Hit,		config::MGE_SOUND_PATH + "Enemy_Hit_sfx.wav");
}

void SoundPlayer::Play(SoundEffect::ID pEffect)
{
	_sounds.push_back(sf::Sound(_soundBuffers.get(pEffect)));
	_sounds.back().play();
}

void SoundPlayer::RemoveStoppedSound()
{
	_sounds.remove_if([](const sf::Sound& s)
	{
		return s.getStatus() == sf::Sound::Stopped;
	});
}