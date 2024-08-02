#pragma once

#include <QObject>
#include <QColor>
#include <QVariantMap>

#include "Components/ActionComponent.h"

class ActionConfig : public QObject{
    Q_OBJECT

    Q_PROPERTY(QString text MEMBER Text)
    Q_PROPERTY(int displayTime MEMBER DisplayTime)
    Q_PROPERTY(bool canInterrupt MEMBER bCanInterrupt)
    Q_PROPERTY(int fontSize MEMBER FontSize)
    Q_PROPERTY(QColor textColor MEMBER TextColor)
    Q_PROPERTY(QString rewardID MEMBER ChannelPointsRewardID NOTIFY rewardUpdated)
public:
    explicit ActionConfig(QObject* Parent = nullptr);
    virtual ~ActionConfig();

    QString Text;
    int DisplayTime;
    bool bCanInterrupt;

    // Text Config
    int FontSize;
    QColor TextColor;

    // Twitch Config
    QString ChannelPointsRewardID;

signals:
    void rewardUpdated(const QString& newRewardID);
};

class Action : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString name MEMBER Name NOTIFY nameChanged)
    Q_PROPERTY(int newIndex MEMBER NewSpriteIndex)

public:
    explicit Action(QObject* Parent = nullptr);
    virtual ~Action();

public:
    //============================ QML ========================================
    // Modifiers
    Q_INVOKABLE void addNewSprite(int Column, int Row);
    Q_INVOKABLE void removeSprite(int Index);
    Q_INVOKABLE void toggleInverted(int Index);
    Q_INVOKABLE void placeSpriteAt(int OriginLocation, int NewLocation);

    // Getters
    Q_INVOKABLE bool hasSprite(int Column, int Row) const;
    Q_INVOKABLE int getSpriteCounts(int Column, int Row) const;
    Q_INVOKABLE QString getName() const;
    Q_INVOKABLE int getTotalSpritesCounts() const;
    Q_INVOKABLE ActionConfig* getConfig();

    Q_INVOKABLE QVariantMap getNextSprite();

    //============================ C++ ========================================
    // Modifiers
    void SetName(const QString& NewName);

    // Getters
    const ActionSequenceSprite GetSpriteById(int Index) const;

    // Event
    void MarkSelected();

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
    int NewSpriteIndex = -1;
    int SpriteToPlayIndex = 0;
};
