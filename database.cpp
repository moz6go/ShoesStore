#include "database.h"

DataBase::DataBase(QObject *parent) : QObject(parent) {}

DataBase::~DataBase() {
    sdb.close();
}

bool DataBase::ConnectToDataBase (QString db_path) {
    if(!QFile(db_path).exists()){
        return false;
    }
    else {
        if (!OpenDataBase(db_path)){
            return false;
        }
        return true;
    }
}

bool DataBase::OpenDataBase(QString db_path) {
    sdb = QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName(db_path);
    if(sdb.open()){
        return true;
    }
    else {
        return false;
    }
}
bool DataBase::CreateDataBase() {

    return true;
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

int DataBase::SelectCount(const QString &from) {
    QSqlQuery sel_query;
    sel_query.exec ("SELECT COUNT(*) FROM " + from);
    sel_query.next ();
    return sel_query.value (0).toInt ();
}

double DataBase::SelectSum(const QString &query) {
    QSqlQuery sel_query;
    sel_query.exec (query);
    sel_query.next ();
    return sel_query.value (0).toDouble ();
}

QVector<QVariantList> DataBase::SelectTable(const QString &table_name, const QString& where, const QString& date_from, const QString& date_to) {
    QVector<QVariantList> table;
    QLocale loc(QLocale::Ukrainian, QLocale::Ukraine);
    QSqlQuery query;
    query.exec ("SELECT " + table_name + ".*, " + MODELS_TABLE + "." + CATEGORY + ", "
                                                + MODELS_TABLE + "." + SEASON + ", "
                                                + MODELS_TABLE + "." + WHOLESALE_PRICE + ", "
                                                + MODELS_TABLE + "." + RETAIL_PRICE +
                " FROM " + table_name +
                " INNER JOIN " + MODELS_TABLE + " ON " + table_name + "." + MODEL_ID + " = " + MODELS_TABLE + "." + MODEL_ID +
                " WHERE " + where + " BETWEEN '" + date_from + "' AND '" + date_to + "'");
    QSqlRecord rec = query.record ();

    QVariantList header;
    for(int col = 0; col < rec.count(); ++col){
        QSqlField field = rec.field(col);
        header.append (field.name());
    }
    table.append (header);

    while(query.next ()){
        QVariantList row;
        for(int col = 0; col < rec.count(); ++col) {
            if (rec.field(col).type () == QVariant::Double){
                row.append (QVariant(loc.toString (query.value (col).toDouble ())));
            }
            else {
                row.append (query.value (col));
            }
        }
        table.append (row);
    }
    return table;
}

int DataBase::SelectCount(const QString &from, const QString &where, const QString &equal) {
    QSqlQuery query;
    query.exec ("SELECT COUNT(*) FROM " + from + " WHERE " + where + " = '" + equal + "'");
    query.next ();
    return query.value (0).toInt ();
}

int DataBase::SelectCount(const QString& from, const QString& where1, const QString& where2, const QString& equal1, const QString& equal2) {
    QSqlQuery query;
    query.exec ("SELECT COUNT(*) FROM " + from + " WHERE " + where1 + " = '" + equal1 + "' AND " + where2 + " = '" + equal2 + "'");
    query.next ();
    return query.value (0).toInt ();
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

QString DataBase::Select(const QString &select, const QString &from, const QString &where, const QString &equal) {
    QSqlQuery query;
    query.exec ("SELECT " + select + " FROM "+ from +" WHERE "+ where +" = '" + equal + "'");
    QSqlRecord rec = query.record ();
    query.next ();
    return  query.value(rec.indexOf(select)).toString ();
}

QVariantList DataBase::SelectRow(const QString &select,
                                 const QString &from,
                                 const QString &where1,
                                 const QString &where2,
                                 const QString &equal1,
                                 const QString &equal2,
                                 const int& col_count) {
    QSqlQuery query;
    query.exec ("SELECT " + select + " FROM "+ from + " WHERE " + where1 + " = '" + equal1 + "' AND " + where2 + " = '" + equal2 + "'");
    QVariantList data;
    query.next ();
    for (int i = 0; i < col_count; ++ i) {
        data.append (query.value (i));
    }
    return data;
}

QString DataBase::GenerateInsertQuery(QString table, QStringList columns) {
    QString query = "INSERT INTO " + table +" (";
    for (int col = 0; col < columns.size(); ++col) {
        if(col == columns.size () - 1) {
            query += columns[col] + ")";
        }
        else {
            query += columns[col] + ", ";
        }
    }

    query += "VALUES (";

    for (int col = 0; col < columns.size(); ++col) {
        if(col == columns.size () - 1){
            query += ":" + columns[col] + ");";
        }
        else {
            query += ":" + columns[col] + ", ";
        }
    }
    return query;
}

QStringList DataBase::GenerateBindValues(QStringList columns) {
    QStringList bind_val;
    for (int i = 0; i < columns.size (); ++i) {
        bind_val.append (":" + columns[i]);
    }
    return bind_val;
}

