#ifndef CASHBOX_H
#define CASHBOX_H

#include <QObject>
#include <QSqlQueryModel>


class Cashbox : public QSqlQueryModel
{
    Q_OBJECT

public:
    enum Roles{
        idRole = Qt::UserRole+1,
        BarcodeRole,
        NameRole,
        TypeRole,
        CountRole,
        PriceRole,
        TotalPriceRole
    };
    //
    explicit Cashbox(QObject *parent = 0);
    //
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;


signals:


public slots:

    void updateModel();
    int getId(int row);


private:


};

#endif // CASHBOX_H
