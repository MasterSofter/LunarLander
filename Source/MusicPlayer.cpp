#include "MusicPlayer.h"


MusicPlayer::MusicPlayer()
: _music()
, _filenames()
, _volume(100.f)
{
	_filenames[Music::MenuTheme]    = "Media/Music/MenuTheme.ogg";
	_filenames[Music::MissionTheme] = "Media/Music/MissionTheme.ogg";
}

void MusicPlayer::play(Music::ID theme)
{
	std::string filename = _filenames[theme];

	if (!_music.openFromFile(filename))
		throw std::runtime_error("Music " + filename + " could not be loaded.");

	_music.setVolume(_volume);
	_music.setLoop(true);
	_music.play();
}

void MusicPlayer::stop()
{
	_music.stop();
}

void MusicPlayer::setVolume(float volume)
{
	_volume = volume;
}

void MusicPlayer::setPaused(bool paused)
{
	if (paused)
		_music.pause();
	else
		_music.play();
}
