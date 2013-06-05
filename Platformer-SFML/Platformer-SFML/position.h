struct Position
{
    public:
        float GetPositionX() const { return positionX; }
        float GetPositionY() const { return positionY; }
        void GetPosition(float &x, float &y) const { x = positionX; y = positionY; }
        void SetPositionX(float val) { positionX = val; }
        void SetPositionY(float val) { positionY = val; }
        void SetPositionXY(float valX, float valY) { positionX = valX; positionY = valY; }

    private:
        float positionX;
        float positionY;
};
