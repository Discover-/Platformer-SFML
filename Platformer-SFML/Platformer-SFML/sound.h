#pragma once

#include <vector>
#include <array>
#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <deque>

class Sound
{
    public:
        bool Load(std::string filename);
        void Update();
        void Play(bool loop = false);
        void Stop();

    private:
        sf::SoundBuffer soundBuffer;
        std::deque<sf::Sound> soundInstances;
};
