#include "unit.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "collision.h"
#include "game.h"
#include "menu.h"

Menu::Menu(Game* _game)
{
    game = _game;
    selectedOption = 0;
}

Menu::~Menu()
{

}

void Menu::Load()
{
    sf::Texture image;
    image.loadFromFile("Graphics/Other/menu_button_play.png");
    MenuSpriteInfo tileInfo;
    tileInfo.image = image;
    tileInfo.posX = 0.0f;
    tileInfo.posY = 70.0f;
    tileInfo.id = 1;
    sprites.push_back(tileInfo);

    image.loadFromFile("Graphics/Other/menu_button_exit.png");
    tileInfo.image = image;
    tileInfo.posX = 0.0f;
    tileInfo.posY = 220.0f;
    tileInfo.id = 2;
    sprites.push_back(tileInfo);

    image.loadFromFile("Graphics/Other/menu_button_info.png");
    tileInfo.image = image;
    tileInfo.posX = 0.0f;
    tileInfo.posY = 370.0f;
    tileInfo.id = 3;
    sprites.push_back(tileInfo);

    image.loadFromFile("Graphics/Other/menu_button_play_lightup.png");
    tileInfo.image = image;
    tileInfo.posX = 0.0f;
    tileInfo.posY = 70.0f;
    tileInfo.id = 1;
    spritesLightup.push_back(tileInfo);

    image.loadFromFile("Graphics/Other/menu_button_exit_lightup.png");
    tileInfo.image = image;
    tileInfo.posX = 0.0f;
    tileInfo.posY = 220.0f;
    tileInfo.id = 2;
    spritesLightup.push_back(tileInfo);

    image.loadFromFile("Graphics/Other/menu_button_info_lightup.png");
    tileInfo.image = image;
    tileInfo.posX = 0.0f;
    tileInfo.posY = 370.0f;
    tileInfo.id = 3;
    spritesLightup.push_back(tileInfo);
}

void Menu::Update(sf::RenderWindow &window)
{
    float mouseX = float(sf::Mouse::getPosition(window).x);
    float mouseY = float(sf::Mouse::getPosition(window).y);

    //selectedOption = 0;

    for (std::vector<MenuSpriteInfo>::iterator itr = sprites.begin(); itr != sprites.end(); ++itr)
    {
        sf::Sprite sprite((*itr).image);
        sprite.setPosition((*itr).posX, (*itr).posY);

        if (WillCollision(mouseX, mouseY, 10.0f, 10.0f, sprite.getPosition().x, sprite.getPosition().y, sprite.getLocalBounds().height, sprite.getLocalBounds().width))
        {
            selectedOption = (*itr).id;
            break;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        selectedOption--;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        selectedOption++;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) || sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        switch (selectedOption)
        {
            case 1:
                game->StartActualGame(window);
                break;
            case 2:
                window.close();
                break;
            case 3:
                // NYI
                break;
            default:
                break;
        }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        window.close();

    Draw(window);
}

void Menu::Draw(sf::RenderWindow &window)
{
    for (std::vector<MenuSpriteInfo>::iterator itr = sprites.begin(); itr != sprites.end(); ++itr)
    {
        if (selectedOption != 0 && selectedOption == (*itr).id)
        {
            for (std::vector<MenuSpriteInfo>::iterator itr2 = spritesLightup.begin(); itr2 != spritesLightup.end(); ++itr2)
            {
                if ((*itr2).id == (*itr).id)
                {
                    sf::Sprite sprite((*itr2).image);
                    sprite.setPosition((*itr2).posX, (*itr2).posY);
                    window.draw(sprite);
                    break;
                }
            }
        }
        else
        {
            sf::Sprite sprite((*itr).image);
            sprite.setPosition((*itr).posX, (*itr).posY);
            window.draw(sprite);
        }
    }
}
