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
    MenuButtonInfo menuButtonInfo;

    font.loadFromFile("Fonts/Market_Deco.ttf");

    sf::RectangleShape rectangleBackground(sf::Vector2f(400.0f, 500.0f));
    rectangleBackground.setFillColor(sf::Color(255, 255, 255, 128));
    rectangleBackground.setPosition(rectangleBackground.getLocalBounds().width / 2.0f, rectangleBackground.getLocalBounds().height / 2.0f);
    menuButtonInfo.rectShape = rectangleBackground;
    menuButtonInfo.textShape = sf::Text("", font, 15);
    menuButtons.push_back(menuButtonInfo);

    sf::RectangleShape rectangleButtons(sf::Vector2f(300.0f, 50.0f));
    sf::Text textButtons("", font, 30);
    textButtons.setColor(sf::Color::Black);
    textButtons.setPosition(textButtons.getLocalBounds().width / 2.0f, textButtons.getLocalBounds().height / 2.0f);
                
    rectangleButtons.setFillColor(sf::Color(255, 255, 255, 100));

    rectangleButtons.setPosition(rectangleButtons.getLocalBounds().width / 2.0f, rectangleButtons.getLocalBounds().height / 2.0f + 150.0f);
    textButtons.setPosition(rectangleButtons.getLocalBounds().width / 2.0f - 80.0f, rectangleButtons.getLocalBounds().height / 2.0f + 145.0f);
    textButtons.setString("Play Game");
    menuButtonInfo.rectShape = rectangleButtons;
    menuButtonInfo.textShape = textButtons;
    menuButtonInfo.id = 1;
    menuButtons.push_back(menuButtonInfo);

    rectangleButtons.setPosition(rectangleButtons.getLocalBounds().width / 2.0f, rectangleButtons.getLocalBounds().height / 2.0f + 50.0f);
    textButtons.setPosition(rectangleButtons.getLocalBounds().width / 2.0f - 15.0f, rectangleButtons.getLocalBounds().height / 2.0f + 45.0f);
    textButtons.setString("More Information");
    menuButtonInfo.rectShape = rectangleButtons;
    menuButtonInfo.textShape = textButtons;
    menuButtonInfo.id++;
    menuButtons.push_back(menuButtonInfo);

    rectangleButtons.setPosition(rectangleButtons.getLocalBounds().width / 2.0f, rectangleButtons.getLocalBounds().height / 2.0f - 50.0f);
    textButtons.setPosition(rectangleButtons.getLocalBounds().width / 2.0f - 92.0f, rectangleButtons.getLocalBounds().height / 2.0f - 55.0f);
    textButtons.setString("How To");
    menuButtonInfo.rectShape = rectangleButtons;
    menuButtonInfo.textShape = textButtons;
    menuButtonInfo.id++;
    menuButtons.push_back(menuButtonInfo);

    rectangleButtons.setPosition(rectangleButtons.getLocalBounds().width / 2.0f, rectangleButtons.getLocalBounds().height / 2.0f - 150.0f);
    textButtons.setPosition(rectangleButtons.getLocalBounds().width / 2.0f - 80.0f, rectangleButtons.getLocalBounds().height / 2.0f - 155.0f);
    textButtons.setString("Exit Game");
    menuButtonInfo.rectShape = rectangleButtons;
    menuButtonInfo.textShape = textButtons;
    menuButtonInfo.id++;
    menuButtons.push_back(menuButtonInfo);
}

void Menu::Draw(sf::RenderWindow &window)
{
    sf::View view = window.getView();

    for (std::vector<MenuButtonInfo>::iterator itr = menuButtons.begin(); itr != menuButtons.end(); ++itr)
    {
        sf::Vector2f itrRectPos = (*itr).rectShape.getPosition();
        sf::RectangleShape itrRectCopy = (*itr).rectShape;
        itrRectCopy.setPosition(view.getCenter().x - itrRectPos.x, view.getCenter().y - itrRectPos.y);
        sf::Text itrTextCopy = (*itr).textShape;
        itrTextCopy.setPosition(view.getCenter().x - itrTextCopy.getPosition().x, view.getCenter().y - itrTextCopy.getPosition().y);\
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        if ((itrRectCopy.getLocalBounds().width <= 300.0f && WillCollision(float(mousePos.x), float(mousePos.y), 16.0f, 16.0f, itrRectCopy.getPosition().x, itrRectCopy.getPosition().y, itrRectCopy.getLocalBounds().height, itrRectCopy.getLocalBounds().width)))
        {
            selectedOption = (*itr).id;
            itrRectCopy.setFillColor(sf::Color::White);
        }
        else if (selectedOption)
            selectedOption = 0;

        window.draw(itrRectCopy);

        if ((*itr).rectShape.getLocalBounds().width <= 300.0f)
            window.draw(itrTextCopy);
    }
}

void Menu::PressedEnterOrMouse(sf::RenderWindow &window)
{
    switch (selectedOption)
    {
        case 0:
            break;
        case 1:
            game->StartActualGame(window);
            break;
        case 2:
            window.close();
            break;
        case 3:
            //! NYI!
            break;
    }
}
