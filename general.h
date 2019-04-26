#ifndef GENERAL
#define GENERAL

#include <QApplication>
#include <QMainWindow>
#include <QtDebug>
#include <QDebug>
#include <QObject>
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
#include <QStackedWidget>
#include <QComboBox>
#include <QDateEdit>
#include <QFileDialog>
#include <QDate>
#include <QBuffer>
#include <QMessageBox>
#include <QStyleFactory>
#include <QThread>
#include <QDataWidgetMapper>

#include <QtSql/QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>
#include <QSqlQueryModel>
#include <QSqlRelationalTableModel>
#include <QSqlError>

const int SIZE_WID_1 = 40;
const int SIZE_WID_2 = 24;

#if defined(_WIN32)
    #if defined(QT_DEBUG)
    const QString DB_PATH = "D:\\MyProjects\\Qt\\ShoesStore\\db";
    #else
    const QString DB_PATH = "D:\\ShoesStore\\db";
    #endif
#else
    const QString DB_PATH = "/home/myroslav/Документи/Repos/ShoesStore/db";
#endif

//tables
const QString MODELS_TABLE = "models";
const QString AVAILABLE_GOODS_TABLE = "available_goods";
const QString BRANDS_TABLE = "brands";
const QString CATEGORIES_TABLE = "categories";
const QString SEASONS_TABLE = "seasons";
const QString SOLD_GOODS_TABLE = "sold_goods";

//count of columns
const int MODELS_COL_COUNT = 9;
const int GOODS_COL_COUNT = 6;
const int SOLD_COL_COUNT = 9;

//columns
const QString MODEL_ID = "model_id";
const QString MODEL_NAME = "model_name";
const QString SEASON = "season";
const QString CATEGORY = "category";
const QString BRAND = "brand";
const QString WHOLESALE_PRICE = "wholesale_price";
const QString RETAIL_PRICE = "retail_price";
const QString PIC = "pic";
const QString DATE = "date";

const QString GOODS_ID = "goods_id";
const QString GOODS_SIZE = "goods_size";
const QString GOODS_DATE = "goods_date";

const QString SALE_PRICE = "sale_price";
const QString SALE_DATE = "sale_date";
const QString PROFIT = "profit";

const QString AVAILABLE_GOODS_WPRICE_SUM_QUERY = "SELECT SUM(" + WHOLESALE_PRICE + ")"
                                          " FROM ("
                                            " SELECT " + AVAILABLE_GOODS_TABLE + "." + MODEL_ID + ", " + MODELS_TABLE + "." + WHOLESALE_PRICE + ""
                                            " FROM " + AVAILABLE_GOODS_TABLE +
                                            " INNER JOIN " + MODELS_TABLE + " ON " + AVAILABLE_GOODS_TABLE + "." + MODEL_ID + " = " + MODELS_TABLE + "." + MODEL_ID + ")";

const QString SOLD_GOODS_SUM_QUERY = "SELECT SUM("+ SALE_PRICE +")"
                                    " FROM " + SOLD_GOODS_TABLE ;


const QString SOLD_GOODS_SUM_BY_MONTH_QUERY = "SELECT SUM("+ SALE_PRICE +")"
                                             " FROM " + SOLD_GOODS_TABLE +
                                             " WHERE " + SALE_DATE + ">= datetime('now', 'start of month')";

const QString SOLD_GOODS_SUM_BY_YEAR_QUERY = "SELECT SUM("+ SALE_PRICE +")"
                                             " FROM " + SOLD_GOODS_TABLE +
                                             " WHERE " + SALE_DATE + ">= datetime('now', 'start of year')";

const QString PROFIT_QUERY = "SELECT SUM("+ PROFIT +")"
                            " FROM " + SOLD_GOODS_TABLE ;


const QString PROFIT_BY_MONTH_QUERY = "SELECT SUM("+ PROFIT +")"
                                    " FROM " + SOLD_GOODS_TABLE +
                                    " WHERE " + SALE_DATE + ">= datetime('now', 'start of month')";

const QString PROFIT_BY_YEAR_QUERY = "SELECT SUM("+ PROFIT +")"
                                    " FROM " + SOLD_GOODS_TABLE +
                                    " WHERE " + SALE_DATE + ">= datetime('now', 'start of year')";



const QStringList SUMMARY_ROWS = {
    "Всього товару:",
    " - шт.",
    " - грн",
    "Оборот, грн",
    " - в поточному році",
    " - в поточному місяці",
    "Прибуток, грн",
    " - в поточному році",
    " - в поточному місяці",
};

const QStringList REPORT_TYPES = {
    "Продані товари в поточному місяці",
    "Отримані товари в поточному місяці",
    "Залишок товарів"
};

enum SearchType {
    BY_MODEL_NAME = 1,
    BY_SEASON,
    BY_CATEGORY,
    BY_BRAND,
    BY_WPRICE,
    BY_RPRICE
};

enum CompareType {
    EQUAL = 1,
    NOT_EQUAL,
    CONTAINS,
    LESS,
    GREATER,
    LESS_EQUAL,
    GREATER_EQUAL
};

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
enum State {
    ENABLED_ALL = 1,
    DISABLED_ALL,
    DATA_BASE_ISNT_INIT,
    MODEL_TABLE_EMPTY,
    GOODS_TABLE_EMPTY
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
