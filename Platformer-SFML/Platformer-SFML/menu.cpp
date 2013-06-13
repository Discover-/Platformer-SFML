#include "unit.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "game.h"
#include "menu.h"

Menu::Menu(Game* _game)
{
    game = _game;
    selectedOption = 1;
    currentMenu = MENU_MAIN;
    newMenu = MENU_NONE;
    movingCurrMenuOut = false;
    movingNewMenuIn = false;
}

Menu::~Menu()
{

}

void Menu::Load()
{
    std::vector<MenuButtonInfo> menuButtonsInfo;
    MenuButtonInfo menuButtonInfo;

    font.loadFromFile("Fonts/Market_Deco.ttf");

    sf::RectangleShape rectangleBackground(sf::Vector2f(400.0f, 500.0f));
    rectangleBackground.setFillColor(sf::Color(255, 255, 255, 128));
    rectangleBackground.setPosition(rectangleBackground.getLocalBounds().width / 2.0f, rectangleBackground.getLocalBounds().height / 2.0f);
    menuButtonInfo.rectShape = rectangleBackground;
    menuButtonInfo.textShape = sf::Text("", font, 15);
    menuButtonInfo.id = 0;
    menuButtonsInfo.push_back(menuButtonInfo);

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
    menuButtonInfo.id++;
    menuButtonsInfo.push_back(menuButtonInfo);

    rectangleButtons.setPosition(rectangleButtons.getLocalBounds().width / 2.0f, rectangleButtons.getLocalBounds().height / 2.0f + 50.0f);
    textButtons.setPosition(rectangleButtons.getLocalBounds().width / 2.0f - 15.0f, rectangleButtons.getLocalBounds().height / 2.0f + 45.0f);
    textButtons.setString("More Information");
    menuButtonInfo.rectShape = rectangleButtons;
    menuButtonInfo.textShape = textButtons;
    menuButtonInfo.id++;
    menuButtonsInfo.push_back(menuButtonInfo);

    rectangleButtons.setPosition(rectangleButtons.getLocalBounds().width / 2.0f, rectangleButtons.getLocalBounds().height / 2.0f - 50.0f);
    textButtons.setPosition(rectangleButtons.getLocalBounds().width / 2.0f - 92.0f, rectangleButtons.getLocalBounds().height / 2.0f - 55.0f);
    textButtons.setString("How To");
    menuButtonInfo.rectShape = rectangleButtons;
    menuButtonInfo.textShape = textButtons;
    menuButtonInfo.id++;
    menuButtonsInfo.push_back(menuButtonInfo);

    rectangleButtons.setPosition(rectangleButtons.getLocalBounds().width / 2.0f, rectangleButtons.getLocalBounds().height / 2.0f - 150.0f);
    textButtons.setPosition(rectangleButtons.getLocalBounds().width / 2.0f - 80.0f, rectangleButtons.getLocalBounds().height / 2.0f - 155.0f);
    textButtons.setString("Exit Game");
    menuButtonInfo.rectShape = rectangleButtons;
    menuButtonInfo.textShape = textButtons;
    menuButtonInfo.id++;
    menuButtonsInfo.push_back(menuButtonInfo);

    MenuInfo menuInfo;
    menuInfo.id = MENU_MAIN;
    menuInfo.draw = true;
    menuInfo.amountOfButtons = int(menuButtonsInfo.size());
    menus.push_back(std::make_pair(menuInfo, menuButtonsInfo));

    menuButtonsInfo.clear();

    rectangleButtons.setSize(sf::Vector2f(75.0f, 75.0f));

    rectangleButtons.setPosition(rectangleButtons.getLocalBounds().width / 2.0f + 290.0f, rectangleButtons.getLocalBounds().height / 2.0f + 860.0f);
    textButtons.setPosition(rectangleButtons.getLocalBounds().width / 2.0f + 260.0f, rectangleButtons.getLocalBounds().height / 2.0f + 840.0f);
    textButtons.setString("1");
    menuButtonInfo.rectShape = rectangleButtons;
    menuButtonInfo.textShape = textButtons;
    menuButtonInfo.id = 1;
    menuButtonsInfo.push_back(menuButtonInfo);

    rectangleButtons.setPosition(rectangleButtons.getLocalBounds().width / 2.0f + 200.0f, rectangleButtons.getLocalBounds().height / 2.0f + 860.0f);
    textButtons.setPosition(rectangleButtons.getLocalBounds().width / 2.0f + 170.0f, rectangleButtons.getLocalBounds().height / 2.0f + 840.0f);
    textButtons.setString("2");
    menuButtonInfo.rectShape = rectangleButtons;
    menuButtonInfo.textShape = textButtons;
    menuButtonInfo.id++;
    menuButtonsInfo.push_back(menuButtonInfo);

    rectangleButtons.setPosition(rectangleButtons.getLocalBounds().width / 2.0f + 110.0f, rectangleButtons.getLocalBounds().height / 2.0f + 860.0f);
    textButtons.setPosition(rectangleButtons.getLocalBounds().width / 2.0f + 80.0f, rectangleButtons.getLocalBounds().height / 2.0f + 840.0f);
    textButtons.setString("3");
    menuButtonInfo.rectShape = rectangleButtons;
    menuButtonInfo.textShape = textButtons;
    menuButtonInfo.id++;
    menuButtonsInfo.push_back(menuButtonInfo);

    rectangleButtons.setPosition(rectangleButtons.getLocalBounds().width / 2.0f + 20.0f, rectangleButtons.getLocalBounds().height / 2.0f + 860.0f);
    textButtons.setPosition(rectangleButtons.getLocalBounds().width / 2.0f - 10.0f, rectangleButtons.getLocalBounds().height / 2.0f + 840.0f);
    textButtons.setString("4");
    menuButtonInfo.rectShape = rectangleButtons;
    menuButtonInfo.textShape = textButtons;
    menuButtonInfo.id++;
    menuButtonsInfo.push_back(menuButtonInfo);

    rectangleButtons.setPosition(rectangleButtons.getLocalBounds().width / 2.0f - 70.0f, rectangleButtons.getLocalBounds().height / 2.0f + 860.0f);
    textButtons.setPosition(rectangleButtons.getLocalBounds().width / 2.0f - 90.0f, rectangleButtons.getLocalBounds().height / 2.0f + 840.0f);
    textButtons.setString("5");
    menuButtonInfo.rectShape = rectangleButtons;
    menuButtonInfo.textShape = textButtons;
    menuButtonInfo.id++;
    menuButtonsInfo.push_back(menuButtonInfo);

    rectangleButtons.setPosition(rectangleButtons.getLocalBounds().width / 2.0f - 160.0f, rectangleButtons.getLocalBounds().height / 2.0f + 860.0f);
    textButtons.setPosition(rectangleButtons.getLocalBounds().width / 2.0f - 180.0f, rectangleButtons.getLocalBounds().height / 2.0f + 840.0f);
    textButtons.setString("6");
    menuButtonInfo.rectShape = rectangleButtons;
    menuButtonInfo.textShape = textButtons;
    menuButtonInfo.id++;
    menuButtonsInfo.push_back(menuButtonInfo);

    rectangleButtons.setPosition(rectangleButtons.getLocalBounds().width / 2.0f - 250.0f, rectangleButtons.getLocalBounds().height / 2.0f + 860.0f);
    textButtons.setPosition(rectangleButtons.getLocalBounds().width / 2.0f - 270.0f, rectangleButtons.getLocalBounds().height / 2.0f + 840.0f);
    textButtons.setString("7");
    menuButtonInfo.rectShape = rectangleButtons;
    menuButtonInfo.textShape = textButtons;
    menuButtonInfo.id++;
    menuButtonsInfo.push_back(menuButtonInfo);

    menuInfo.id = MENU_LEVELS;
    menuInfo.draw = false;
    menuInfo.amountOfButtons = int(menuButtonsInfo.size());
    menus.push_back(std::make_pair(menuInfo, menuButtonsInfo));
    menuButtonsInfo.clear();
}

void Menu::Update(sf::RenderWindow &window)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F4) || sf::Keyboard::isKeyPressed(sf::Keyboard::F5))
    {
        for (std::vector<std::pair<MenuInfo, std::vector<MenuButtonInfo>>>::iterator itr3 = menus.begin(); itr3 != menus.end(); ++itr3)
        {
            if ((*itr3).first.id == currentMenu)
            {
                for (std::vector<MenuButtonInfo>::iterator itr = (*itr3).second.begin(); itr != (*itr3).second.end(); ++itr)
                {
                    (*itr).rectShape.setPosition((*itr).rectShape.getPosition().x, sf::Keyboard::isKeyPressed(sf::Keyboard::F4) ? (*itr).rectShape.getPosition().y + 1.0f : (*itr).rectShape.getPosition().y - 1.0f);
                    (*itr).textShape.setPosition((*itr).textShape.getPosition().x, sf::Keyboard::isKeyPressed(sf::Keyboard::F4) ? (*itr).textShape.getPosition().y + 1.0f : (*itr).textShape.getPosition().y - 1.0f);
                }

                break;
            }
        }
    }

    if (movingCurrMenuOut)
    {
        for (std::vector<std::pair<MenuInfo, std::vector<MenuButtonInfo>>>::iterator itr3 = menus.begin(); itr3 != menus.end(); ++itr3)
        {
            if ((*itr3).first.id == currentMenu)
            {
                for (std::vector<MenuButtonInfo>::iterator itr = (*itr3).second.begin(); itr != (*itr3).second.end(); ++itr)
                {
                    (*itr).rectShape.setPosition((*itr).rectShape.getPosition().x, (*itr).rectShape.getPosition().y + 20.0f);
                    (*itr).textShape.setPosition((*itr).textShape.getPosition().x, (*itr).textShape.getPosition().y + 20.0f);

                    if (movingNewMenuIn)
                        continue;

                    if ((*itr).textShape.getPosition().y > 800.0f)
                    {
                        movingCurrMenuOut = false;
                        movingNewMenuIn = true;
                        currentMenu = newMenu;
                        newMenu = MENU_NONE;

                        for (std::vector<std::pair<MenuInfo, std::vector<MenuButtonInfo>>>::iterator itr2 = menus.begin(); itr2 != menus.end(); ++itr2)
                        {
                            if ((*itr2).first.id == currentMenu)
                                (*itr2).first.draw = false;
                            else if ((*itr2).first.id == newMenu)
                                (*itr2).first.draw = true;
                        }
                    }
                }
            }
        }
    }
    else if (movingNewMenuIn)
    {
        for (std::vector<MenuButtonInfo>::iterator itr = menus[currentMenu].second.begin(); itr != menus[currentMenu].second.end(); ++itr)
        {
            (*itr).rectShape.setPosition((*itr).rectShape.getPosition().x, (*itr).rectShape.getPosition().y - 20.0f);
            (*itr).textShape.setPosition((*itr).textShape.getPosition().x, (*itr).textShape.getPosition().y - 20.0f);

            if (!movingNewMenuIn)
                continue;

            if ((*itr).id != 0 && (*itr).rectShape.getPosition().y < -110.0f)
            {
                movingCurrMenuOut = false;
                movingNewMenuIn = false;
            }
        }
    }

    Draw(window);
}

void Menu::Draw(sf::RenderWindow &window)
{
    sf::Vector2i currMousePos = sf::Mouse::getPosition(window);
    sf::View view = window.getView();

    for (std::vector<std::pair<MenuInfo, std::vector<MenuButtonInfo>>>::iterator itr = menus.begin(); itr != menus.end(); ++itr)
    {
        if ((*itr).first.draw || (*itr).first.id == currentMenu)
        {
            for (std::vector<MenuButtonInfo>::iterator itr2 = (*itr).second.begin(); itr2 != (*itr).second.end(); ++itr2)
            {
                sf::Vector2f itrRectPos = (*itr2).rectShape.getPosition();
                sf::RectangleShape itrRectCopy = (*itr2).rectShape;
                itrRectCopy.setPosition(view.getCenter().x - itrRectPos.x, view.getCenter().y - itrRectPos.y);
                sf::Text itrTextCopy = (*itr2).textShape;
                itrTextCopy.setPosition(view.getCenter().x - itrTextCopy.getPosition().x, view.getCenter().y - itrTextCopy.getPosition().y);

                if ((*itr2).id && (selectedOption == (*itr2).id || !(prevMousePos.x == currMousePos.x && prevMousePos.y == currMousePos.y) && !(currMousePos.x > 1000.0f || currMousePos.y > 600.0f || currMousePos.x < 0.0f || currMousePos.y < 0.0f)))
                {
                    if (selectedOption == (*itr2).id || WillCollision(float(currMousePos.x), float(currMousePos.y), 16.0f, 16.0f, itrRectCopy.getPosition().x, itrRectCopy.getPosition().y, itrRectCopy.getLocalBounds().height, itrRectCopy.getLocalBounds().width))
                    {
                        selectedOption = (*itr2).id;
                        itrRectCopy.setFillColor(sf::Color::White);
                    }
                    else if (selectedOption)
                        selectedOption = 0;
                }

                window.draw(itrRectCopy);

                if ((*itr2).id)
                    window.draw(itrTextCopy);
            }
        }
    }

    prevMousePos = currMousePos;
}

void Menu::PressedEnterOrMouse(sf::RenderWindow &window)
{
    if (movingCurrMenuOut || movingNewMenuIn)
        return;

    switch (currentMenu)
    {
        case MENU_NONE:
            break;
        case MENU_MAIN:
        {
            switch (selectedOption)
            {
                case 0:
                    break;
                case 1: //! Play Game
                    newMenu = MENU_LEVELS;
                    movingCurrMenuOut = true;
                    break;
                case 2: //! More Information
                    //! NYI!
                    break;
                case 3: //! How To
                    //! NYI!
                    break;
                case 4: //! Exit Game
                    window.close();
                    break;
            }
            break;
        }
        case MENU_LEVELS:
        {
            switch (selectedOption)
            {
                case 0:
                    break;
                default:
                {
                    for (std::vector<std::pair<MenuInfo, std::vector<MenuButtonInfo>>>::iterator itr = menus.begin(); itr != menus.end(); ++itr)
                    {
                        if ((*itr).first.id == currentMenu)
                        {
                            for (std::vector<MenuButtonInfo>::iterator itr2 = (*itr).second.begin(); itr2 != (*itr).second.end(); ++itr2)
                            {
                                if ((*itr2).id == selectedOption)
                                {
                                    game->StartActualGame(window, (*itr2).textShape.getString());
                                    break;
                                }
                            }

                            break;
                        }
                    }
                    break;
                }
            }
            break;
        }
    }
}

void Menu::UpdateSelection(bool moveNegative)
{
    if (moveNegative)//! arrow up
    {
        if ((currentMenu == MENU_LEVELS && selectedOption <= 1) || (currentMenu != MENU_LEVELS && selectedOption < 2))
            selectedOption = currentMenu == MENU_MAIN ? menus[currentMenu].first.amountOfButtons - 1 : menus[currentMenu].first.amountOfButtons;
        else
            selectedOption -= 1;
    }
    else
    {
        if ((currentMenu == MENU_LEVELS && selectedOption >= menus[currentMenu].first.amountOfButtons) || (currentMenu != MENU_LEVELS && selectedOption >= menus[currentMenu].first.amountOfButtons - 1))
        //if (selectedOption >= menus[currentMenu].first.amountOfButtons)
            selectedOption = 1;
        else
            selectedOption += 1;
    }
}
