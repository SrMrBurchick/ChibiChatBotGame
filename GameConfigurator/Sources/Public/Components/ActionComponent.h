#pragma once
#include <QString>
#include <QVector>

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
    QVector<QString> Actions;
    ActionGridSprite(int Column, int Row);

    bool operator==(const ActionGridSprite& GridSprite) const;
    bool operator!=(const ActionGridSprite& GridSprite) const;
};

bool operator==(const ActionGridSprite& GridSprite, const ActionSequenceSprite& SequenceSprite);
bool operator!=(const ActionGridSprite& GridSprite, const ActionSequenceSprite& SequenceSprite);

bool operator==(const ActionSequenceSprite& SequenceSprite, const ActionGridSprite& GridSprite);
bool operator!=(const ActionSequenceSprite& SequenceSprite, const ActionGridSprite& GridSprite);

using ActionsMap = QHash<QString, QList<ActionSequenceSprite>>;
