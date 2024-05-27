#include "Core/Action.h"
#include "System/Logger.h"

Action::Action(QObject* Parent)
    : QObject(Parent)
{
}

Action::~Action()
{
    LOG_INFO("Action %s Deleted", getName().toStdString().c_str());
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

void Action::placeSpriteAt(int OriginLocation, int NewLocation)
{
    if (OriginLocation < 0 || OriginLocation >= SpriteSequence.count() || NewLocation < 0 || NewLocation >= SpriteSequence.count())
    {
        emit spriteSequenceUpdated();
        return;
    }

    SpriteSequence.swapItemsAt(OriginLocation, NewLocation);
    emit spriteSequenceUpdated();
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

int Action::getTotalSpritesCounts() const
{
    return SpriteSequence.count();
}

const ActionSequenceSprite Action::GetSpriteById(int Index) const
{
    ActionSequenceSprite Sprite(0, 0);

    if (Index >= 0 && Index < SpriteSequence.count())
    {
        return SpriteSequence[Index];
    }

    return Sprite;
}

void Action::toggleInverted(int Index)
{
    if (Index >= 0 && Index < SpriteSequence.count())
    {
        SpriteSequence[Index].toggleInverted();
        emit spriteSequenceUpdated();
    }
}

QVariantMap Action::getNextSprite()
{
    QVariantMap NextSprite;
    if (SpriteToPlayIndex >= SpriteSequence.count())
    {
        SpriteToPlayIndex = 0;
    }

    ActionSequenceSprite Sprite = SpriteSequence[SpriteToPlayIndex++];
    NextSprite["sprite_column"] = Sprite.Column;
    NextSprite["sprite_row"] = Sprite.Row;
    NextSprite["isInverted"] = Sprite.bInverted;

    return NextSprite;
}

void Action::MarkSelected()
{
    emit spriteSequenceUpdated();
}
