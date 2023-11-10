#include "Components/ActionComponent.h"

ActionSequenceSprite::ActionSequenceSprite(int Column, int Row)
    : Column(Column), Row(Row), bInverted(false)
{
}

void ActionSequenceSprite::toggleInverted()
{
    bInverted = !bInverted;
}


ActionGridSprite::ActionGridSprite(int Column, int Row)
    :Column(Column), Row(Row)
{
}

bool ActionGridSprite::operator==(const ActionGridSprite& GridSprite) const
{
    return Column == GridSprite.Column && Row == GridSprite.Row;
}

bool ActionGridSprite::operator!=(const ActionGridSprite& GridSprite) const
{
    return !(*this == GridSprite);
}

bool operator==(const ActionGridSprite& GridSprite, const ActionSequenceSprite& SequenceSprite)
{
    return GridSprite.Column == SequenceSprite.Column && GridSprite.Row == SequenceSprite.Row;
}

bool operator!=(const ActionGridSprite& GridSprite, const ActionSequenceSprite& SequenceSprite)
{
    return !(GridSprite == SequenceSprite);
}

bool operator==(const ActionSequenceSprite& SequenceSprite, const ActionGridSprite& GridSprite)
{
    return GridSprite == SequenceSprite;
}

bool operator!=(const ActionSequenceSprite& SequenceSprite, const ActionGridSprite& GridSprite)
{
    return GridSprite != SequenceSprite;
}

