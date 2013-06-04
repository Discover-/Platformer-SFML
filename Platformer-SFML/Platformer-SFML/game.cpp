#include <iostream>
#include <vector>
#include <cstring>
#include <map>
#include <time.h>
#include <unordered_map>
#include <set>
#include <string>
//#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Config.hpp>
#include <SFML/Network.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include "Windows.h"
#include "game.h"
#include "player.h"
#include "shareddefines.h"

Game::Game()
{
    isRunning = true;
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
    sf::Sprite spriteCharacter(imageCharacter);
    player = new Player(this, &window, 400, 70, spriteCharacter);

    sf::Texture imageDirt[2];
    sf::Texture imageGrass[2];
    sf::Texture imageGround[2];
    sf::Texture imageSky[4];
    sf::Sprite spriteDirt[15][20];
    sf::Sprite spriteGrass[12][20];
    sf::Sprite spriteGround[12][20];
    sf::Sprite spriteSky[12][20];

    for (int i = 0; i < 2; ++i)
    {
        std::string numberInStr = std::to_string(long double(i));
        imageDirt[i].loadFromFile("dirt_" + numberInStr + ".png");
        imageGrass[i].loadFromFile("grass_" + numberInStr + ".png");
        imageGround[i].loadFromFile("ground_" + numberInStr + ".png");
    }

    for (int i = 0; i < 4; ++i)
        imageSky[i].loadFromFile("sky_" + std::to_string(long double(i)) + ".png");

    for (int i = 7; i < 9; ++i)
        for (int j = 0; j < 20; ++j)
            spriteGrass[i][j].setTexture(imageGrass[urand(0, 1)]);

    for (int i = 9; i < 12; ++i)
        for (int j = 0; j < 20; ++j)
            spriteGround[i][j].setTexture(imageGround[urand(0, 1)]);

    for (int i = 12; i < 15; ++i)
        for (int j = 0; j < 20; ++j)
            spriteDirt[i][j].setTexture(imageDirt[urand(0, 1)]);

    for (int i = 0; i < 12; ++i)
        for (int j = 0; j < 20; ++j)
            spriteSky[i][j].setTexture(imageSky[(i > 7 || urand(0, 20) < 16) ? 3 : urand(0, 2)]);

    //spriteDirt[0].setPosition(0, 150);
    //spriteDirt[1].setPosition(50, 150);
    //spriteGrass[0].setPosition(100, 150);
    //spriteGrass[1].setPosition(150, 150);
    //spriteGround[0].setPosition(200, 150);
    //spriteGround[1].setPosition(250, 150);

    float boxX = 0.0f, boxY = 0.0f;

    //! Filling up skybox
    for (int i = 0; i < 12; ++i)
    {
        for (int j = 0; j < 20; ++j)
        {
            spriteSky[i][j].setPosition(boxX, boxY);
            boxX += 50.0f;
        }

        boxX = 0.0f;
        boxY += 50.0f;
    }

    //! Filling up grass
    boxX = 0.0f;
    boxY = 250.0f;

    for (int i = 7; i < 9; ++i)
    {
        for (int j = 0; j < 20; ++j)
        {
            spriteGrass[i][j].setPosition(boxX, boxY);
            boxX += 50.0f;
        }

        boxX = 0.0f;
        boxY += 50.0f;
    }

    //! Filling up ground
    boxX = 0.0f;
    boxY = 350.0f;

    for (int i = 9; i < 12; ++i)
    {
        for (int j = 0; j < 20; ++j)
        {
            spriteGround[i][j].setPosition(boxX, boxY);
            boxX += 50.0f;
        }

        boxX = 0.0f;
        boxY += 50.0f;
    }

    //! Filling up dirt
    boxX = 0.0f;
    boxY = 500.0f;

    for (int i = 12; i < 15; ++i)
    {
        for (int j = 0; j < 20; ++j)
        {
            spriteDirt[i][j].setPosition(boxX, boxY);
            boxX += 50.0f;
        }

        boxX = 0.0f;
        boxY += 50.0f;
    }

    window.setFramerateLimit(30);

    //! Sky blocks are not collidable.
    for (int i = 0; i < 12; ++i)
        for (int j = 0; j < 20; ++j)
            gameObjects.push_back(spriteSky[i][j]);

    for (int i = 9; i < 12; ++i)
    {
        for (int j = 0; j < 20; ++j)
        {
            gameObjects.push_back(spriteGround[i][j]);
            gameObjectsCollidable.push_back(spriteGround[i][j]);
        }
    }

    for (int i = 7; i < 9; ++i)
    {
        for (int j = 0; j < 20; ++j)
        {
            gameObjects.push_back(spriteGrass[i][j]);
            gameObjectsCollidable.push_back(spriteGrass[i][j]);
        }
    }

    for (int i = 12; i < 15; ++i)
    {
        for (int j = 0; j < 20; ++j)
        {
            gameObjects.push_back(spriteDirt[i][j]);
            gameObjectsCollidable.push_back(spriteDirt[i][j]);
        }
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

        for (int i = 0; i < 12; ++i)
            for (int j = 0; j < 20; ++j)
                window.draw(spriteSky[i][j]);

        for (int i = 9; i < 12; ++i)
            for (int j = 0; j < 20; ++j)
                window.draw(spriteGround[i][j]);

        for (int i = 7; i < 9; ++i)
            for (int j = 0; j < 20; ++j)
                window.draw(spriteGrass[i][j]);

        for (int i = 12; i < 15; ++i)
            for (int j = 0; j < 20; ++j)
                window.draw(spriteDirt[i][j]);

        sf::Vector2f pos = player->GetPosXY();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            if (!player->CollidesWithGameobjects(player->GetPosX() - 4.0f, player->GetPosY()))
                player->SetPosXY(pos.x -= 4.0f, pos.y);
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            if (!player->CollidesWithGameobjects(player->GetPosX() + 4.0f, player->GetPosY()))
                player->SetPosXY(pos.x += 4.0f, pos.y);
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            if (!player->IsJumping() && !player->IsFalling())
                player->SetIsJumping(true);

        player->Update();
        window.display();
    }

    return 0;
}
