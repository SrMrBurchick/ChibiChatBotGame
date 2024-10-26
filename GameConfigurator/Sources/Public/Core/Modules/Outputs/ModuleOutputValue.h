#pragma once

#include <QObject>
#include <QSharedPointer>

#include "Core/Modules/ModuleTypes.h"

class Action;

class CBModuleOutputValue: public QObject {
    Q_OBJECT

    Q_PROPERTY(QString name MEMBER Name)
public:
    // ================================ C++ ====================================
    // Modifiers
    // explicit CBModuleOutputValue(QObject* Parent = nullptr);
    // virtual ~CBModuleOutputValue();

private:
    QString Name;
};
