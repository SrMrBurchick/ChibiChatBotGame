#pragma once

#include <QObject>
#include <QVector>
#include <QSharedPointer>

class CBModuleBindResult;
class ActionsManager;
class CBModulesManager;
class Action;

class CBModuleBindResultPool: public QObject {
    Q_OBJECT

    Q_PROPERTY(QString postfix READ getPoolPostfix)
    Q_PROPERTY(int resultsCount READ getResultsCount NOTIFY bindsUpdated)

public:
    // ================================ C++ ====================================
    // Modifiers
    explicit CBModuleBindResultPool(QObject* Parent = nullptr);
    virtual ~CBModuleBindResultPool();

    static QSharedPointer<CBModuleBindResultPool> CreatePool(const QString& Postfix);

    // Getters
    bool CanBindAction(const QSharedPointer<Action>& TargetAction) const;

    // ================================ QML ====================================
    // Modifiers
    Q_INVOKABLE void bindNewAction(ActionsManager* Manager, CBModulesManager* ModulesManager, const QString& ActionName);
    Q_INVOKABLE void removeResult(int Index);
    Q_INVOKABLE void setNewPosition(int Position);
    Q_INVOKABLE void commitSwap(int OldPosition);

    // Getters
    Q_INVOKABLE int getResultsCount() const;
    Q_INVOKABLE CBModuleBindResult* getResult(int Index) const;
    Q_INVOKABLE const QString& getPoolPostfix() const;

signals:
    void selectionChanged();
    void bindsUpdated();

protected:
    void ChangeResultPosition(int OldPosition);

private:
    QVector<QSharedPointer<CBModuleBindResult>> Results;
    QString PoolPostfix;
    int NewPosition = -1;
};
