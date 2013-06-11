#pragma once

struct Position
{
    public:
        float GetPositionX() const { return positionX; }
        float GetPositionY() const { return positionY; }
        void GetPosition(float &x, float &y) const { x = positionX; y = positionY; }
        sf::Vector2f GetPosition() const { return sf::Vector2f(positionX, positionY); }
        void SetPositionX(float val) { positionX = val; }
        void SetPositionY(float val) { positionY = val; }
        void SetPosition(float valX, float valY) { positionX = valX; positionY = valY; }

    private:
        float positionX;
        float positionY;
};
