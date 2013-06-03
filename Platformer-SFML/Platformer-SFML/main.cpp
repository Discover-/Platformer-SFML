#include <SFML/Graphics.hpp>

int WinMain()
{
    sf::RenderWindow window(sf::VideoMode(1000, 600), "Platformer C++ SFML");

    sf::Texture image;

    //if (!image.loadFromFile(".\Graphics\Characters\frame_0.png"))
    if (!image.loadFromFile("frame_0.png"))
        return 0;

    sf::Sprite sprite;
    sprite.setTexture(image);

    while (window.isOpen())
    {
        sf::Event _event;

        while (window.pollEvent(_event))
        {
            if (_event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}
