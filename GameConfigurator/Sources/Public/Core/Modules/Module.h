#pragma once

#include <QObject>
#include <QSharedPointer>
#include <QList>
#include <QVector>
#include <QJsonDocument>

class ActionsManager;
class Action;

class CBModuleInputParam;
class CBModuleOutput;
class CBModuleBindConfig;

class QJsonObject;

class CBModule: public QObject {
    Q_OBJECT

    Q_PROPERTY(QString name MEMBER Name)
    Q_PROPERTY(QString version MEMBER Version)
    Q_PROPERTY(bool isSelected MEMBER bIsSelected NOTIFY selectionChanged)
    Q_PROPERTY(int bindsCount READ getBindsCount NOTIFY bindsUpdated)
    Q_PROPERTY(int outputsCount READ getOutputsCount)

public:
    // ================================ C++ ====================================
    // Modifiers
    explicit CBModule(QObject* Parent = nullptr);
    virtual ~CBModule();

    bool ParseConfig(const QJsonDocument& JsonData, const QString& ModuleDirectory);
    bool ParseGameConfig(const QJsonObject& Config);

    // Getters
    bool IsSelected() const;
    const QString& GetName() const;
    const QString& GetVersion() const;
    const QString& GetPath() const;
    bool HasBindedAction(QSharedPointer<Action> TargetAction) const;
    QJsonObject GenerateConfig() const;

    // ================================ QML ====================================
    // Modifiers
    Q_INVOKABLE void setSelected(bool bSelected);
    Q_INVOKABLE CBModuleOutput* getOutput(int Index);
    Q_INVOKABLE void bindNewAction(ActionsManager* Manager, const QString& ActionName);
    Q_INVOKABLE void selectBind(int Index);
    Q_INVOKABLE CBModuleBindConfig* getSelectedBindConfig();
    Q_INVOKABLE void removeBindConfig(int Index);

    // Getters
    Q_INVOKABLE int getBindsCount() const;
    Q_INVOKABLE int getOutputsCount() const;
    Q_INVOKABLE CBModuleBindConfig* getBindConfig(int Index) const;

signals:
    void selectionChanged();
    void bindsUpdated();

protected:
    QSharedPointer<CBModuleBindConfig> GetBindByAction(const QString& TargetAction) const;

private:
    QString Name;
    QString Version;
    QString Path;
    QString ConfigFile;
    QString ModuleDirectory;

    QList<QSharedPointer<CBModuleOutput>> Outputs;
    QList<QSharedPointer<CBModuleInputParam>> Inputs;

    QVector<QSharedPointer<CBModuleBindConfig>> Binds;
    bool bIsSelected = false;
};
