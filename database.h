#ifndef DATABASE_H
#define DATABASE_H

#include "general.h"

class DataBase : public QObject
{
    Q_OBJECT
    QSqlDatabase sdb;
    QString last_error;
    QStringList create_queries;
    bool OpenDataBase(QString db_path);
    bool RestoreDataBase();
public:
    DataBase (const QStringList& create_tables_queries, QObject *parent = nullptr);
    ~DataBase ();
    QString LastError();
    bool ConnectToDataBase(const QString& db_path);
    bool CreateDataBase();
    void CloseDataBase();
    bool UpdateInsertData (const QString& query_str,  const QStringList& bind_values_list, const QVariantList& data);
    QString GenerateInsertQuery(const QString& table, const QStringList& columns);
    QString GenerateUpdateQuery(const QString& table, const QStringList& columns, const QString& where, const QString& equal);
    QStringList GenerateBindValues(QStringList columns);
    QString Select(const QString &query);
    double SelectSum(const QString& qeury);

    QVector<QVariantList> SelectTable(const QString &str_query);

    int SelectCount(const QString& from);

    int SelectCount(const QString& from,
                    const QString& where,
                    const QString& equal_sign,
                    const QString& equal);

    int SelectCount(const QString& from,
                    const QString& where1, const QString& where2,
                    const QString& equal_sign1, const QString& equal_sign2,
                    const QString& equal1, const QString& equal2);

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

    QVariantList SelectRow(const QString& select,
                           const QString& from,
                           const QString& where,
                           const QString& equal,
                           const int& col_count);

    QVariantList SelectRow(const QString& select,
                           const QString& from,
                           const QString& where1,
                           const QString& where2,
                           const QString& equal1,
                           const QString& equal2,
                           const int& col_count);
};

#endif // DATABASE_H
