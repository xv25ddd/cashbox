#include "listmodel.h"
#include "database.h"
#include "cashbox.h"



Cashbox::Cashbox(QObject *parent):
    QSqlQueryModel (parent)
{
    this->updateModel();
}


QVariant Cashbox::data(const QModelIndex &index, int role) const{
    int columnId = role-Qt::UserRole-1;
    QModelIndex modelIndex = this->index(index.row(), columnId);
    return QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
}


QHash<int, QByteArray> Cashbox::roleNames() const{
    QHash<int, QByteArray> roles;
    roles[idRole] = "id";
    roles[BarcodeRole] = "barcode";
    roles[NameRole] = "name";
    roles[TypeRole] = "type";
    roles[CountRole] = "count";
    roles[PriceRole] = "price";
    roles[TotalPriceRole] = "total_price";
    return roles;
}


void Cashbox::updateModel()
{
    this->setQuery("SELECT id, barcode, name, type, count, price, total_price FROM cashbox");
}

int Cashbox::getId(int row)
{
    return this->data(this->index(row, 0), idRole).toInt();
}

