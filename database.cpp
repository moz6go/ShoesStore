#include "database.h"

DataBase::DataBase(QObject *parent) : QObject(parent)
{

}

bool DataBase::ConnectToDataBase (){
    if(!QFile(DB_PATH).exists()){
        return false;
    }
    else {
        OpenDataBase();
        return true;
    }
}

bool DataBase::OpenDataBase()
{
    sdb = QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName(DB_PATH);
    if(sdb.open()){
        return true;
    } else {
        return false;
    }
}
