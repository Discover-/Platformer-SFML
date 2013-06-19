#pragma once

#include <vector>
#include <array>
#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "shareddefines.h"

class Sound
{
    public:
        //Sound::sound();
        //Sound::~Sound();

        bool Load(std::string filename);
        void Update();
        void Play();

    private:
        sf::SoundBuffer soundBuffer;
        std::vector<sf::Sound> soundInstances;
};
