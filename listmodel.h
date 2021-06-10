#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QObject>
#include <QSqlQueryModel>


class ListModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    enum Roles{
        idRole = Qt::UserRole+1,
        BarcodeRole,
        NameRole,
        TypeRole,
        CountRole,
        PriceRole
    };
    //
    explicit ListModel(QObject *parent = 0);
    //
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;


signals:


public slots:

    void updateModel();
    int getId(int row);

};

#endif // LISTMODEL_H
