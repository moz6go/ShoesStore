#ifndef GENERAL
#define GENERAL

#include <QApplication>
#include <QMainWindow>
#include <QDialog>
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
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QTextEdit>
#include <QLineEdit>
#include <QComboBox>
#include <QFileDialog>
#include <QDate>
#include <QBuffer>
#include <QMessageBox>
#include <QStyleFactory>
#include <QStandardPaths>

#include <QtSql/QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QSqlField>

const int SIZE_WID_1 = 40;
const int SIZE_WID_2 = 24;

#if defined(_WIN32)
    #if defined(QT_DEBUG)
        const QString DB_PATH = "shoes_strore_db.sqlite3";
    #else
        const QString DB_PATH = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/ShoesStore/shoes_strore_db.sqlite3";
    #endif
#else
    const QString DB_PATH = QDir::currentPath () + "/shoes_strore_db.sqlite3";
#endif
const QString DATE_FORMAT = "dd.MM.yyyy";
const QString TIME_FORMAT = "hh:mm";
const QString SQL_DATE_FORMAT = "yyyy-MM-dd";
const QString SQL_DATE_TIME_FORMAT = "yyyy-MM-dd hh:mm:ss";
const QString FS_DATE_TIME_FORMAT = "yyyy-MM-dd hh-mm-ss";

//tables
const QString MODELS_TABLE = "models";
const QString AVAILABLE_GOODS_TABLE = "available_goods";
const QString BRANDS_TABLE = "brands";
const QString CATEGORIES_TABLE = "categories";
const QString SEASONS_TABLE = "seasons";
const QString SOLD_GOODS_TABLE = "sold_goods";

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



//count of columns
const int MODELS_COL_COUNT = 9;
const int GOODS_COL_COUNT = 6;
const int SOLD_COL_COUNT = 9;


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

const QStringList SOLD_GOODS_HEADERS_LIST = {
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

enum ModelsCol{
    MODEL_ID_COL,
    MODEL_NAME_COL,
    SEASON_COL,
    CATEGORY_COL,
    BRAND_COL,
    WHOLESALE_PRICE_COL,
    RETAIL_PRICE_COL,
    PIC_COL,
    DATE_COL
};

enum AvGoodsCol {
    AV_MODEL_ID_COL,
    AV_MODEL_NAME_COL,
    AV_BRAND_COL,
    GOODS_ID_COL,
    GOODS_SIZE_COL,
    GOODS_DATE_COL
};

enum SoldGoodCol {
    S_MODEL_ID_COL,
    S_MODEL_NAME_COL,
    S_BRAND_COL,
    S_GOODS_ID_COL,
    S_GOODS_SIZE_COL,
    S_GOODS_DATE_COL,
    SALE_PRICE_COL,
    PROFIT_COL,
    SALE_DATE_COL
};

enum Sizes {
    S36,
    S37,
    S38,
    S39,
    S40,
    S41,
    S42,
    S43,
    S44,
    S45,
    S46,
    WITHOUT_SIZE
};

enum Role {
    ADD,
    EDIT
};


#endif // GENERAL
