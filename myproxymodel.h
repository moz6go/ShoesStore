#ifndef EVENTSPROXYMODEL_H
#define EVENTSPROXYMODEL_H

#include "general.h"

class MyProxyModel : public QSortFilterProxyModel
{
public:
    MyProxyModel(QObject* parent = nullptr);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
};

#endif // EVENTSPROXYMODEL_H
