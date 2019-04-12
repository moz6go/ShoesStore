#include "database.h"

DataBase::DataBase(QObject *parent) : QObject(parent) {}

DataBase::~DataBase() {
    sdb.close();
}

bool DataBase::ConnectToDataBase () {
    if(!QFile(DB_PATH).exists()){
        return false;
    }
    else {
        OpenDataBase();
        return true;
    }
}

bool DataBase::InsertDataIntoTable (const QString& query_str, const QStringList& bind_values_list, const QVariantList& data) {
    QSqlQuery query;
    query.prepare (query_str);
    for (int i = 0; i < bind_values_list.size (); ++i) {
        query.bindValue (bind_values_list[i], data[i]);
    }

    if(!query.exec ()) {
        qDebug() << query.lastError ().text ();
        return false;
    }
    else {
        return true;
    }
}

int DataBase::GetGoodsCount(const int &model_id, const int &size) {
    QSqlQuery sel_query;
    sel_query.exec ("SELECT COUNT(*) FROM available_goods_dir WHERE model_id = " + QString::number (model_id) + " AND goods_size = " + QString::number (size));
    sel_query.next ();
    return sel_query.value (0).toInt ();
}

int DataBase::GetModelId(const QString &model_name) {
    QSqlQuery sel_query;
    sel_query.exec ("SELECT model_id FROM model_dir WHERE model_name = '" + model_name + "'");
    QSqlRecord rec = sel_query.record ();
    sel_query.next ();
    return  sel_query.value(rec.indexOf("model_id")).toInt();
}

bool DataBase::OpenDataBase() {
    sdb = QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName(DB_PATH);
    if(sdb.open()){
        return true;
    } else {
        return false;
    }
}
