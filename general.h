#ifndef GENERAL
#define GENERAL

#include <QApplication>
#include <QMainWindow>
#include <QtDebug>
#include <QDebug>
#include <QObject>
#include <QThread>
#include <QMutex>
#include <QMainWindow>
#include <QToolBar>
#include <QLabel>
#include <QLineEdit>
#include <QHeaderView>
#include <QTableView>
#include <QTableWidget>
#include <QSplitter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QSpinBox>
#include <QTextEdit>
#include <QLineEdit>
#include <QFileSystemModel>
#include <QPushButton>
#include <QFileDialog>
#include <QDate>
#include <QBuffer>

#include <QtSql/QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QSqlRelationalTableModel>
#include <QSqlError>

#include <thread>
#include <mutex>

const int SIZE_WID = 32;

#if defined(_WIN32)
const QString DB_PATH = "D:\\MyProjects\\Qt\\ShoesStore\\db";
#else
const QString DB_PATH = "/home/myroslav/Документи/Repos/ShoesStore/db";
#endif

const QString ADD_MODEL_QUERY = "INSERT INTO model_dir (model_name, category, season, brand, wholesale_price, retail_price, pic, date)"
                                "VALUES (:model_name, :category, :season, :brand, :wholesale_price, :retail_price, :pic, :date);";
const QStringList ADD_MODEL_BIND_VALUES = {
    ":model_name",
    ":category",
    ":season",
    ":brand",
    ":wholesale_price",
    ":retail_price",
    ":pic",
    ":date"
};
const QString ADD_GOODS_QUERY = "INSERT INTO available_goods_dir (model_id, goods_size, goods_date)"
                                "VALUES (:model_id, :goods_size, :goods_date);";
const QStringList ADD_GOODS_BIND_VALUES = {
    ":model_id",
    ":goods_size",
    ":goods_date"
};
const QString MODEL_DIR = "model_dir";
const QString AVAILABLE_GOODS_DIR = "available_goods_dir";

const QStringList MAIN_TABLE_HEADERS_LIST = {
    "!id",
    "Назва моделі",
    "Сезон",
    "Категорія",
    "Виробник",
    "Оптова ціна, грн",
    "Роздрібна ціна, грн",
    "!pic",
    "!date"
};

const QStringList MODEL_DIR_COLUMNS = {
    "model_id",
    "model_name",
    "season",
    "category",
    "brand",
    "wholesale_price",
    "retail_price",
    "pic",
    "date"
};

const QStringList AVAILABLE_GOODS_DIR_COLUMNS = {
    "model_id",
    "model_name",
    "season",
    "category",
    "brand",
    "wholesale_price",
    "retail_price",
    "pic",
    "date",
    "godds_id",
    "goods_size",
    "goods_date"
};

enum SIZES {

};

struct Model {
    QString model_name;
    QString season;
    QString brand;
    double wholesale_price;
    double retail_price;
    QString photo_path;
};

struct Product : public Model {
    unsigned size;
};

#endif // GENERAL
