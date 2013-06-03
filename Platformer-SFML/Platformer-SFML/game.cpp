#include "shareddefines.h"
#include <iostream>
#include <vector>
//#include <cmath>
#include <cstring>
#include <map>
#include <time.h>
#include <unordered_map>
#include <set>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Config.hpp>
#include <SFML/Network.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include "Windows.h"
#include "game.h"

Game::Game()
{
    isRunning = true;
    isJumping = false;
    isFalling = false;
    fallSpeed = 0;
    jumpSpeed = 150;
}

Game::~Game()
{

}



int Game::Update()
{
    isRunning = true;
    sf::RenderWindow window(sf::VideoMode(1000, 600), "Platformer C++ SFML");
    sf::Texture imageCharacter;
    imageCharacter.loadFromFile("tux_frame_0.png");
    sf::Sprite spriteCharacter;
    spriteCharacter.setTexture(imageCharacter);
    spriteCharacter.setPosition(400, 500);

    sf::Texture imageDirt[2];
    sf::Texture imageGrass[2];
    sf::Texture imageGround[2];
    sf::Texture imageSky[4];
    sf::Sprite spriteDirt[2];
    sf::Sprite spriteGrass[2];
    sf::Sprite spriteGround[2];
    sf::Sprite spriteSky[4][20]; //! Layer and amount

    for (int i = 0; i < 2; ++i)
    {
        std::string numberInStr = std::to_string(long double(i));
        imageDirt[i].loadFromFile("dirt_" + numberInStr + ".png");
        imageGrass[i].loadFromFile("grass_" + numberInStr + ".png");
        imageGround[i].loadFromFile("ground_" + numberInStr + ".png");
        spriteDirt[i].setTexture(imageDirt[i]);
        spriteGrass[i].setTexture(imageGrass[i]);
        spriteGround[i].setTexture(imageGround[i]);
    }

    for (int i = 0; i < 4; ++i)
    {
        std::string numberInStr = std::to_string(long double(i));
        imageSky[i].loadFromFile("sky_" + numberInStr + ".png");
    }

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 20; ++j)
            spriteSky[i][j].setTexture(imageSky[urand(0, 3)]);

    spriteDirt[0].setPosition(0, 150);
    spriteDirt[1].setPosition(50, 150);
    spriteGrass[0].setPosition(100, 150);
    spriteGrass[1].setPosition(150, 150);
    spriteGround[0].setPosition(200, 150);
    spriteGround[1].setPosition(250, 150);

    //! Filling up skybox
    float skyX = 0.0f, skyY = 0.0f;

    for (int i = 0; i < 3; ++i) //! Three layers
    {
        for (int j = 0; j < 20; ++j) //! 12 blocks per layer
        {
            spriteSky[i][j].setPosition(skyX, skyY);
            skyX += 50.0f;
        }

        skyX = 0.0f;
        skyY += 50.0f;
    }

    while (window.isOpen())
    {
        sf::Event _event;

        while (window.pollEvent(_event))
        {
            if (_event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        for (int i = 0; i < 2; ++i)
        {
            window.draw(spriteDirt[i]);
            window.draw(spriteGrass[i]);
            window.draw(spriteGround[i]);
        }

        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 20; ++j)
                window.draw(spriteSky[i][j]);

        sf::Vector2f pos = spriteCharacter.getPosition();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            spriteCharacter.setPosition(pos.x -= 0.04, pos.y);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            isJumping = true;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            spriteCharacter.setPosition(pos.x += 0.04, pos.y);

        if (isJumping)
        {
            if (jumpSpeed)
            {
                spriteCharacter.setPosition(pos.x, pos.y - 0.5f);
                jumpSpeed--;
            }
            else
            {
                isJumping = false;
                //isFalling = true;
                jumpSpeed = 15;
            }
        }

        if (isFalling)
        {
            spriteCharacter.setPosition(pos.x, pos.y + fallSpeed);
            fallSpeed++;

            if (!fallSpeed)
            {
                isFalling = false;
                fallSpeed = 0;
            }
        }

        window.draw(spriteCharacter);
        window.display();
    }

    return 0;
}
