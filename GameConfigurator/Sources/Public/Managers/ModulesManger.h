#pragma once
#include <QObject>
#include <QVector>
#include <QStringList>
#include <QFileInfo>
#include <QSharedPointer>

class CBModule;

class CBModulesManager: public QObject {
    Q_OBJECT

    Q_PROPERTY(bool isBusy MEMBER bIsBusy NOTIFY busyUpdated)
    Q_PROPERTY(int modulesCount READ getModulesCount NOTIFY modulesSynced)
public:
    explicit CBModulesManager(QObject* Parent = nullptr);
    virtual ~CBModulesManager();

    // ================================ C++ ====================================
    // Modifiers
    bool IsModuleExists(const QSharedPointer<CBModule>& NewModule) const;

    // Getters


    // ================================ QML ====================================
    // Modifiers
    Q_INVOKABLE void initModules();
    Q_INVOKABLE void selectModule(int Index);

    // Getters
    Q_INVOKABLE int getModulesCount() const;
    Q_INVOKABLE CBModule* getSelectedModule() const;
    Q_INVOKABLE CBModule* getModule(int Index) const;

signals:
    void busyUpdated();
    void moduleSelected();
    void modulesSynced();

protected:
    // ================================ C++ ====================================
    // Modifiers
    void SetBusy(bool isBusy);
    void SearchModules();

    // Getters
    QList<QFileInfo> GetModules();
    static QFileInfo SearchModuleConfig(const QDir& Directory);

private:
    bool bIsBusy = false;
    QVector<QSharedPointer<CBModule>> Modules;
};
