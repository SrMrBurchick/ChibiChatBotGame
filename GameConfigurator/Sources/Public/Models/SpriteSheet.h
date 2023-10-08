#pragma once

#include <QAbstractListModel>
#include <QObject>
#include <QVector>

class SpriteSheetModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit SpriteSheetModel(QObject* Parent = nullptr);
    virtual ~SpriteSheetModel() = default;

    virtual int rowCount(const QModelIndex& Parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& Index, int Role = Qt::DisplayRole) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    static void registerModel(const QString& ModuleName);

    void updateData();

protected:
};
