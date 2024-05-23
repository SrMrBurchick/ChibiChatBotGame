#pragma once

#include <QObject>

class ActionsManager : public QObject
{
    Q_OBJECT

public:
    explicit ActionsManager(QObject* Parent = nullptr);
};
