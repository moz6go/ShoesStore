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
#include <QSettings>

#include <QtSql/QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>
#include <QSqlQueryModel>
#include <QSqlRelationalTableModel>
#include <QSqlError>
#include <QSqlField>

const int SIZE_WID_1 = 40;
const int SIZE_WID_2 = 24;

#if defined(_WIN32)
    #if defined(QT_DEBUG)
        const QString DB_PATH  = "D:\\MyProjects\\Qt\\ShoesStore\\shoes_strore_db.sqlite3";
    #else
        const QString DB_PATH = QDir::currentPath () + "/shoes_strore_db.sqlite3";
    #endif
#else
    const QString DB_PATH = "/home/myroslav/Документи/Repos/ShoesStore/shoes_strore_db.sqlite3";
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

const QString COSTS_BY_MONTH_QUERY = "SELECT SUM(" + WHOLESALE_PRICE + ")"
                                          " FROM ("
                                            " SELECT " + AVAILABLE_GOODS_TABLE + "." + MODEL_ID + ", " + MODELS_TABLE + "." + WHOLESALE_PRICE + ""
                                            " FROM " + AVAILABLE_GOODS_TABLE +
                                            " INNER JOIN " + MODELS_TABLE + " ON " + AVAILABLE_GOODS_TABLE + "." + MODEL_ID + " = " + MODELS_TABLE + "." + MODEL_ID +
                                            " WHERE " + GOODS_DATE + " >= datetime('now', 'start of month'))";

const QString COSTS_BY_YEAR_QUERY = "SELECT SUM(" + WHOLESALE_PRICE + ")"
                                          " FROM ("
                                            " SELECT " + AVAILABLE_GOODS_TABLE + "." + MODEL_ID + ", " + MODELS_TABLE + "." + WHOLESALE_PRICE + ""
                                            " FROM " + AVAILABLE_GOODS_TABLE +
                                            " INNER JOIN " + MODELS_TABLE + " ON " + AVAILABLE_GOODS_TABLE + "." + MODEL_ID + " = " + MODELS_TABLE + "." + MODEL_ID +
                                            " WHERE " + GOODS_DATE + " >= datetime('now', 'start of year'))";

const QString INCOME_BY_MONTH_QUERY = "SELECT SUM("+ SALE_PRICE +")"
                                             " FROM " + SOLD_GOODS_TABLE +
                                             " WHERE " + SALE_DATE + ">= datetime('now', 'start of month')";

const QString INCOME_BY_YEAR_QUERY = "SELECT SUM("+ SALE_PRICE +")"
                                             " FROM " + SOLD_GOODS_TABLE +
                                             " WHERE " + SALE_DATE + ">= datetime('now', 'start of year')";

const QString AVAILABLE_GOODS_QUERY = "SELECT * FROM " + AVAILABLE_GOODS_TABLE;
const QString SOLD_GOODS_QUERY = "SELECT * FROM " + SOLD_GOODS_TABLE;

const QStringList SUMMARY_ROWS = {
    /*0*/ "Всього товару:",
    /*1*/ " - шт.",
    /*2*/ " - грн",
    /*3*/ "Поточний місяць, грн:",
    /*4*/ " - Дохід",
    /*5*/ " - Витрати",
    /*6*/ " - Чистий прибуток",
    /*7*/ "Поточний рік, грн:",
    /*8*/ " - Дохід",
    /*9*/ " - Витрати",
    /*10*/" - Чистий прибуток"
};

const QStringList DICTIONARIES = {
    "Бренди",
    "Категорії",
    "Сезони"
};

enum DictTypes {
    BRANDS,
    CATEGORIES,
    SEASONS
};

const QStringList REPORTS_TYPES = {
    "Продані товари за період",
    "Отримані товари за період"
};

enum ReportsTypes {
    SOLD_GOODS_REPORT,
    AVAILABLE_GOODS_REPORT
};

enum SearchType {
    BY_MODEL_NAME = 1,
    BY_SEASON,
    BY_CATEGORY,
    BY_BRAND,
    BY_WPRICE,
    BY_RPRICE
};

const QStringList MODELS_TABLE_HEADERS_LIST = {
    /*0*/"!id",
    /*1*/"Назва моделі",
    /*2*/"Сезон",
    /*3*/"Категорія",
    /*4*/"Виробник",
    /*5*/"Оптова ціна, грн",
    /*6*/"Роздрібна ціна, грн",
    /*7*/"!pic",
    /*8*/"!date"
};

const QStringList SOLD_GOODS_TABLE_HEADERS_LIST = {
    /*0*/"Код моделі",
    /*1*/"Назва моделі",
    /*2*/"Виробник",
    /*3*/"Код одиниці товару",
    /*4*/"Розмір",
    /*5*/"Дата прийому товару",
    /*6*/"Відпускна ціна, грн",
    /*7*/"Прибуток, грн",
    /*8*/"Дата продажу"
};

enum State {
    ENABLED_ALL,
    DISABLED_ALL
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
