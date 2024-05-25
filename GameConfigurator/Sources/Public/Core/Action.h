#pragma once

#include <QObject>
#include <QColor>
#include <QVariantMap>

#include "Components/ActionComponent.h"

struct ActionConfig {
    QString Text;
    int ExecutionTime;
    bool bCanInterrupt;

    // Text Config
    int FontSize;
    QColor TextColor;
};

class Action : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString name MEMBER Name NOTIFY nameChanged)

public:
    explicit Action(QObject* Parent = nullptr);
    virtual ~Action() = default;

    void SetName(const QString& NewName);
    void PlaceSpriteAt(int OriginLocation, int NewLocation);

public:
    // Modifiers
    Q_INVOKABLE void addNewSprite(int Column, int Row);
    Q_INVOKABLE void removeSprite(int Index);

    // Getters
    Q_INVOKABLE bool hasSprite(int Column, int Row) const;
    Q_INVOKABLE int getSpriteCounts(int Column, int Row) const;
    Q_INVOKABLE QString getName() const;

signals:
    void newSpriteAdded(int Column, int Row);
    void spriteRemoved(int Column, int Row);

    void configUpdated(QVariantMap NewConfig);

    void spriteSequenceUpdated();
    void nameChanged(const QString& NewName);

protected:
    void MarkConfigUpdated();

private:
    QString Name;
    ActionConfig Config;
    QVector<ActionSequenceSprite> SpriteSequence;
};
