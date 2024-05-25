#include "Core/Action.h"

Action::Action(QObject* Parent)
    : QObject(Parent)
{

}

void Action::MarkConfigUpdated()
{
    QVariantMap VariantConfig;
    VariantConfig["text"] = Config.Text;
    VariantConfig["executionTime"] = Config.ExecutionTime;
    VariantConfig["isCanInterrupt"] = Config.bCanInterrupt;
    VariantConfig["fontSize"] = Config.FontSize;
    VariantConfig["textColor"] = Config.TextColor;

    emit configUpdated(VariantConfig);
}

void Action::SetName(const QString& NewName)
{
    Name = NewName;
    emit nameChanged(Name);
}

void Action::PlaceSpriteAt(int OriginLocation, int NewLocation)
{
    if (OriginLocation < 0 || OriginLocation >= SpriteSequence.count() || NewLocation < 0 || NewLocation >= SpriteSequence.count())
    {
        return;
    }

    SpriteSequence.swapItemsAt(OriginLocation, NewLocation);
}

void Action::addNewSprite(int Column, int Row)
{
    ActionSequenceSprite NewSprite(Column, Row);
    SpriteSequence.push_back(NewSprite);

    emit spriteSequenceUpdated();
}

void Action::removeSprite(int Index)
{
    if (Index < 0 || Index >= SpriteSequence.count())
    {
        return;
    }

    SpriteSequence.remove(Index);

    emit spriteSequenceUpdated();
}

bool Action::hasSprite(int Column, int Row) const
{
    for (const ActionSequenceSprite& Sprite : SpriteSequence)
    {
        if (Sprite.Column == Column && Sprite.Row == Row)
        {
            return true;
        }
    }

    return false;
}

int Action::getSpriteCounts(int Column, int Row) const
{
    if (!hasSprite(Column, Row))
    {
        return 0;
    }

    int Count = 0;

    for (const ActionSequenceSprite& Sprite : SpriteSequence)
    {
        if (Sprite.Column == Column && Sprite.Row == Row)
        {
            ++Count;
        }
    }

    return Count;
}

QString Action::getName() const
{
    return Name;
}
