#include "Components/ActionComponent.h"

ActionSequenceSprite::ActionSequenceSprite(int Column, int Row)
    : Column(Column), Row(Row), bInverted(false)
{
}

void ActionSequenceSprite::toggleInverted()
{
    bInverted = !bInverted;
}
