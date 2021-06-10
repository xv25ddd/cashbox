#include "listmodel.h"
#include "database.h"
#include "cashbox.h"



ListModel::ListModel(QObject *parent):
    QSqlQueryModel (parent)
{
    this->updateModel();
}


QVariant ListModel::data(const QModelIndex &index, int role) const{
    int columnId = role-Qt::UserRole-1;
    QModelIndex modelIndex = this->index(index.row(), columnId);
    return QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
}


QHash<int, QByteArray> ListModel::roleNames() const{
    QHash<int, QByteArray> roles;
    roles[idRole] = "id";
    roles[BarcodeRole] = "barcode";
    roles[NameRole] = "name";
    roles[TypeRole] = "type";
    roles[CountRole] = "count";
    roles[PriceRole] = "price";
    return roles;
}


void ListModel::updateModel()
{
    this->setQuery("SELECT id, barcode, name, type, count, price FROM main");
}

int ListModel::getId(int row)
{
    return this->data(this->index(row, 0), idRole).toInt();
}

