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
        if (!OpenDataBase()){
            return false;
        }
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
        return false;
    }
    else {
        return true;
    }
}


int DataBase::SelectCount(const QString &from, const QString &where, const QString &equal) {
    QSqlQuery sel_query;
    sel_query.exec ("SELECT COUNT(*) FROM " + from + " WHERE " + where + " = '" + equal + "'");
    sel_query.next ();
    return sel_query.value (0).toInt ();
}

int DataBase::SelectCount(const QString& from, const QString& where1, const QString& where2, const QString& equal1, const QString& equal2) {
    QSqlQuery sel_query;
    sel_query.exec ("SELECT COUNT(*) FROM " + from + " WHERE " + where1 + " = '" + equal1 + "' AND " + where2 + " = '" + equal2 + "'");
    sel_query.next ();
    return sel_query.value (0).toInt ();
}

bool DataBase::DeleteRow(const QString &from, const QString &where, const QString &equal) {
    QSqlQuery query;
    if (query.exec ("DELETE FROM " + from + " WHERE " + where + " = '" + equal + "'")){
        return true;
    }
    else {
        return false;
    }
}

QByteArray DataBase::SelectPic(const QString &from, const QString &where, const QString &equal) {
    QSqlQuery query;
    query.exec ("SELECT pic FROM " + from + " WHERE " + where + " = '" + equal + "'");
    QSqlRecord rec = query.record ();
    query.next ();
    return query.value (rec.indexOf ("pic")).toByteArray ();
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
