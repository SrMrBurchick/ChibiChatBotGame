#pragma once
#include <QString>

struct ActionSequenceSprite {
    int Column;
    int Row;
    bool bInverted;

    ActionSequenceSprite(int Column, int Row);
    void toggleInverted();
};

struct ActionGridSprite {
    int Column;
    int Row;
    QString Action;
};
