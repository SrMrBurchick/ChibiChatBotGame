#pragma once

#include <QObject>
#include <QSharedPointer>

#include "Core/Modules/ModuleTypes.h"

class Action;
class CBModuleBindResultPool;

class CBModuleOutput: public QObject {
    Q_OBJECT

    Q_PROPERTY(QString name MEMBER Name)
public:
    // ================================ C++ ====================================
    // Modifiers
    explicit CBModuleOutput(QObject* Parent = nullptr);
    virtual ~CBModuleOutput();

    // Getters
    virtual EOutputType GetType() const;
    virtual int GetPoolsCount() const;
    virtual QSharedPointer<CBModuleBindResultPool> CreatePool(int Index) const;

    const QString GetTypeString() const;
    const QString& GetName() const;

private:
    QString Name;

    friend class CBModuleOutputFactory;
};
