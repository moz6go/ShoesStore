#ifndef DATABASE_H
#define DATABASE_H

#include "general.h"

class DataBase : public QObject
{
    Q_OBJECT
    QSqlDatabase sdb;

    bool OpenDataBase();
    bool RestoreDataBase();
    void CloseDataBase();

public:
    DataBase (QObject *parent = 0);
    bool ConnectToDataBase();
    bool InsertIntoTable(const QString& table,const QVariantList &data);
};

#endif // DATABASE_H
