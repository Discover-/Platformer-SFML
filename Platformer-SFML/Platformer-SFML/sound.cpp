#include "sound.h"
#include "game.h"

Sound::Sound(Game* _game)
{
    game = _game;
}

Sound::~Sound()
{

}

bool Sound::Load(std::string filename)
{
    return soundBuffer.loadFromFile(filename);
}

void Sound::Update()
{
    for (int i = 0; i < soundInstances.size(); ++i)
    {
        if (soundInstances[i].getStatus() == sf::Sound::Stopped)
        {
            soundInstances.erase(soundInstances.begin() + i);
            --i;
        }
    }
}

void Sound::Play(bool loop /* = false */)
{
    soundInstances.push_back(sf::Sound(soundBuffer));
    soundInstances.back().play();

    if (loop)
        soundInstances.back().setLoop(true);

    if (game->IsMusicMuted())
        soundInstances.back().setVolume(0.0f);
}

void Sound::Stop()
{
    if (sf::Sound* playingSnd = GetPlayingSound())
        playingSnd->stop();
}

void Sound::SetVolume(float volume)
{
    if (sf::Sound* playingSnd = GetPlayingSound())
        playingSnd->setVolume(volume);
}

float Sound::GetVolume()
{
    if (sf::Sound* playingSnd = GetPlayingSound())
        return playingSnd->getVolume();

    return 0.0f;
}

void Sound::SetLoop(bool val)
{
    if (sf::Sound* playingSnd = GetPlayingSound())
        playingSnd->setLoop(val);
}

bool Sound::IsLooping()
{
    if (sf::Sound* playingSnd = GetPlayingSound())
        return playingSnd->getLoop();

    return false;
}

sf::SoundSource::Status Sound::GetStatus()
{
    if (sf::Sound* playingSnd = GetPlayingSound())
        return playingSnd->getStatus();

    return sf::SoundSource::Stopped;
}

sf::Sound* Sound::GetPlayingSound()
{
    for (int i = 0; i < soundInstances.size(); ++i)
        if (soundInstances[i].getStatus() == sf::Sound::Playing)
            return &soundInstances[i];

    return NULL;
}
