#ifndef DATABASE_H
#define DATABASE_H

#include "general.h"

class DataBase : public QObject
{
    Q_OBJECT
    QSqlDatabase sdb;
    QString last_error;

    bool OpenDataBase();
    bool RestoreDataBase();
    void CloseDataBase();

public:
    DataBase (QObject *parent = 0);
    ~DataBase ();
    bool ConnectToDataBase();
    bool InsertDataIntoTable (const QString& query_str,
                              const QStringList& bind_values_list,
                              const QVariantList& data);

    int SelectCount(const QString& from,
                    const QString& where,
                    const QString& equal);

    int SelectCount(const QString& from,
                    const QString& where1,
                    const QString& where2,
                    const QString& equal1,
                    const QString& equal2);

    bool DeleteRow(const QString& from,
                   const QString& where,
                   const QString& equal);

    QByteArray SelectPic(const QString& from,
                         const QString& where,
                         const QString& equal);

    QString Select(const QString &select,
                   const QString& from,
                   const QString& where,
                   const QString& equal);

    QVariantList SelectRow(const QString &select,
                           const QString& from,
                           const QString& where1,
                           const QString& where2,
                           const QString& equal1,
                           const QString& equal2,
                           const int& col_count);

};

#endif // DATABASE_H
