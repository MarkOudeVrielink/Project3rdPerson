#include "mgengine/Resources/MusicPlayer.h"
#include "mge/config.hpp"

MusicPlayer::MusicPlayer() : _music(), _fileNames(), _volume(100.0f)
{
	_fileNames[Music::MenuTheme]		= config::MGE_MUSIC_PATH + "Menu_music.wav";
	_fileNames[Music::MissionTheme_1]	= config::MGE_MUSIC_PATH + "Level_1_music.wav";
	_fileNames[Music::MissionTheme_2]	= config::MGE_MUSIC_PATH + "Level_2_music.wav";
	_fileNames[Music::GameOverTheme]	= config::MGE_MUSIC_PATH + "GameOver_music.wav";
	_fileNames[Music::BossTheme]		= config::MGE_MUSIC_PATH + "Boss_music.wav";
	//TODO:: add the audio files.
}

void MusicPlayer::Play(Music::ID pTheme, bool pRepeat)
{
	std::string filename = _fileNames[pTheme];
	if (!_music.openFromFile(filename))
		throw std::runtime_error("Music " + filename + " could not be loaded.");

	_music.setVolume(_volume);
	_music.setLoop(pRepeat);
	_music.play();
}

void MusicPlayer::Stop()
{
	_music.stop();
}

void MusicPlayer::SetPaused(bool pPaused)
{
	if (pPaused) {
		_music.pause();
	} else {
		_music.play();
	}
}

void MusicPlayer::SetVolume(float pVolume)
{
	_music.setVolume(pVolume);
}
