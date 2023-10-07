#pragma once
#include <QString>

struct ActionSequenceSprite {
    int Column;
    int Row;
    bool bInverted;
};

struct ActionGridSprite {
    int Column;
    int Row;
    QString Action;
};
