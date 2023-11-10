#pragma once

#include <QAbstractListModel>
#include <QObject>
#include <QVector>

enum ePredefinedActionsListRole {
    ActionNameRole = Qt::UserRole,
    ChanceRole
};

struct PredefinedAction {
    QString ActionName;
    int Chance;
};

class PredefinedActionsListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit PredefinedActionsListModel(QObject* Parent = nullptr);
    virtual ~PredefinedActionsListModel() = default;

    virtual int rowCount(const QModelIndex& Parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& Index, int Role = Qt::DisplayRole) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    static void registerModel(const QString& ModelName);

    void initModel(QVector<PredefinedAction>& ActionsList);

    Q_INVOKABLE void removeElement(const int Index);
    Q_INVOKABLE void addNewAction(const QString& NewAction, const int Chance);

    Q_INVOKABLE const QVector<PredefinedAction>& getList() const;
    Q_INVOKABLE QVector<QString> getActions() const;

signals:
    void actionRemoved(QString Action);
    void actionAdded(QString Action);

protected:
    QVector<PredefinedAction> PredefinedActionsList;
};
