#include "bouncetile.h"
#include "shareddefines.h"
#include "game.h"
#include "player.h"
//#include "audio.h"

BounceTile::BounceTile(sf::RenderWindow* _window, sf::Texture _image, sf::Texture _imageUsed, sf::Vector2f _startPosition, std::string color) :
SpecialTile(_window, _image, _startPosition, TYPEID_BOUNCE_TILE)
{
    isUsed = false;
    startPosition = _startPosition;
    SetPosition(startPosition.x, startPosition.y);
    imageUsed = _imageUsed;
}

BounceTile::~BounceTile()
{

}

void BounceTile::Update()
{
    SpecialTile::Update();

    if (GAME_STATE_PAUSED(sGame.GetGameState()))
    {
        Draw(isUsed ? &imageUsed : NULL, true);
        return;
    }

    Draw(isUsed ? &imageUsed : NULL, true);
}

void BounceTile::HandleTimers(sf::Int32 diff_time)
{
    if (IsRemoved())
        return;

    SpecialTile::HandleTimers(diff_time);
}

bool BounceTile::OnCollision(Unit* unit /* = NULL */)
{
    if (!isUsed && unit && unit->GetTypeId() == TYPEID_PLAYER)
    {
        isUsed = true;
        unit->Jump(30);
        //sGame.GetAudio()->Play("Audio/trampoline_shot.wav");
    }

    return false;
}

void BounceTile::OnCollisionOut(Unit* unit)
{

}
