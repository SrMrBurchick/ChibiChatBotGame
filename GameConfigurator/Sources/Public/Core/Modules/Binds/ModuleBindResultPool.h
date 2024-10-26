#pragma once

#include <QObject>
#include <QVector>
#include <QSharedPointer>

class CBModuleBindResult;

class CBModuleBindResultPool: public QObject {
    Q_OBJECT

public:
    // ================================ C++ ====================================
    // Modifiers
    explicit CBModuleBindResultPool(QObject* Parent = nullptr);
    virtual ~CBModuleBindResultPool();

    static QSharedPointer<CBModuleBindResultPool> CreatePool(const QString& Postfix);

private:
    QVector<QSharedPointer<CBModuleBindResult>> Results;
    QString PoolPostfix;
};
