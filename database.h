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
    ~DataBase ();
    bool ConnectToDataBase();
    bool InsertDataIntoTable (const QString& query_str, const QStringList& bind_values_list, const QVariantList& data);
    int GetGoodsCount(const int& model_id, const int& size);
    int GetModelId(const QString& model_name);
};

#endif // DATABASE_H
