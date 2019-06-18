#include "myproxymodel.h"

MyProxyModel::MyProxyModel(QObject *parent) : QSortFilterProxyModel (parent)
{
}

QVariant MyProxyModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()){
        return QVariant();
    }
    if (role == Qt::TextColorRole) {
//        QColor color;
//        if(this->data(this->index(index.row(), EVENT_STATUS_COL)).toString() == STATUS_LIST[CANCELED]) {
//            color = Qt::darkGray;
//        }
//        else if (this->data(this->index(index.row(), EVENT_STATUS_COL)).toString() == STATUS_LIST[EXECUTED]) {
//            color = Qt::darkGreen;
//        }
//        else if (this->data(this->index(index.row(), EVENT_PATIENT_ID_COL)).toString().isEmpty ()) {
//            color = Qt::red;
//        }
//        return QBrush(color);
    }
    if (role == Qt::DisplayRole) {
        if (index.column() == WHOLESALE_PRICE_COL || index.column() == RETAIL_PRICE_COL ){
            QLocale loc(QLocale::Ukrainian, QLocale::Ukraine);
            return loc.toString(QSortFilterProxyModel::data(index,role).toDouble (), 'f', 2);
        }
    }
    return QSortFilterProxyModel::data(index, role);
}
