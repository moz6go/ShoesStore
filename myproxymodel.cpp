#include "myproxymodel.h"

MyProxyModel::MyProxyModel(QObject *parent) : QSortFilterProxyModel (parent)
{
}

QVariant MyProxyModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()) {
        return QVariant();
    }
    if (role == Qt::TextColorRole) {
        QColor color;
        if(this->data(this->index(index.row(), GOODS_COUNT_COL)).toInt () == 0) {
            color = Qt::darkGray;
        }
        return QBrush(color);
    }
    if (role == Qt::DisplayRole) {
        if (index.column() == WHOLESALE_PRICE_COL || index.column() == RETAIL_PRICE_COL ){
            QLocale loc(QLocale::Ukrainian, QLocale::Ukraine);
            return loc.toString(QSortFilterProxyModel::data(index,role).toDouble (), 'f', 2);
        }
    }
    return QSortFilterProxyModel::data(index, role);
}
