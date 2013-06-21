#pragma once

#include <vector>
#include <array>
#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <deque>

class Game;

class Sound
{
    public:
        Sound::Sound(Game* _game);
        Sound::~Sound();

        bool Load(std::string filename);
        void Update();
        void Play(bool loop = false);
        void Stop();
        void SetVolume(float volume);
        float GetVolume();
        void SetLoop(bool val);
        bool IsLooping();
        sf::SoundSource::Status GetStatus();

        sf::Sound* GetPlayingSound();

    private:
        Game* game;
        sf::SoundBuffer soundBuffer;
        std::deque<sf::Sound> soundInstances;
};
