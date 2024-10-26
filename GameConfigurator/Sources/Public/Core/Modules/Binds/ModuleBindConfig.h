#pragma once

#include <QObject>
#include <QSharedPointer>
#include <QVector>

#include "Core/Action.h"

class CBModuleBindResultConfig;
class CBModuleOutput;

class CBModuleBindConfig: public QObject {
    Q_OBJECT

    Q_PROPERTY(Action* targetAction READ getTargetAction)
    Q_PROPERTY(bool isSelected MEMBER bIsSelected NOTIFY selectionChanged)
    Q_PROPERTY(int resultsConfigCount READ getResultsConfigCount)

public:
    // ================================ C++ ====================================
    // Modifiers
    explicit CBModuleBindConfig(QObject* Parent = nullptr);
    virtual ~CBModuleBindConfig();

    bool IsBindedToAction(QSharedPointer<Action> NewAction) const;
    static QSharedPointer<CBModuleBindConfig> CreateBindConfig(const QSharedPointer<Action>& NewAction, const QVector<QSharedPointer<CBModuleOutput>>& Outputs);

    // Getters
    bool IsSelected() const;

    // ================================ QML ====================================
    // Modifiers
    Q_INVOKABLE void setSelected(bool bSelected);

    // Getters
    Q_INVOKABLE Action* getTargetAction() const;
    Q_INVOKABLE CBModuleBindResultConfig* getResultConfig(int Index) const;
    Q_INVOKABLE int getResultsConfigCount() const;

signals:
    void selectionChanged();

private:
    QSharedPointer<Action> TargetAction;
    QVector<QSharedPointer<CBModuleBindResultConfig>> Configs;
    bool bIsSelected = false;
};
