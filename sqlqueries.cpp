#include "sqlqueries.h"

SqlQueries::SqlQueries() {}

QString SqlQueries::CreateModelsTable()
{
    return "CREATE TABLE " + MODELS_TABLE + " ("
            + MODEL_ID + " INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, "
            + MODEL_NAME + " TEXT NOT NULL, "
            + SEASON + " TEXT NOT NULL, "
            + CATEGORY + " TEXT NOT NULL, "
            + BRAND + " TEXT NOT NULL, "
            + WHOLESALE_PRICE + " REAL NOT NULL, "
            + RETAIL_PRICE + " REAL NOT NULL, "
            + PIC + " BLOB, "
            + DATE + " TEXT NOT NULL)";
}

QString SqlQueries::CreateAvailablesGoodsTable() {
    return "CREATE TABLE " + AVAILABLE_GOODS_TABLE + " ("
            + MODEL_ID + "	INTEGER NOT NULL,"
            + MODEL_NAME + " TEXT NOT NULL,"
            + BRAND + " TEXT NOT NULL,"
            + GOODS_ID + "	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,"
            + GOODS_SIZE + " TEXT NOT NULL,"
            + GOODS_DATE + " TEXT NOT NULL)";
}

QString SqlQueries::CreateBrandsTable() {
    return "CREATE TABLE " + BRANDS_TABLE + " ("+ BRAND +" TEXT)";
}

QString SqlQueries::CreateCategoriesTable() {
    return "CREATE TABLE " + CATEGORIES_TABLE+ " ("+ CATEGORY + " TEXT)";
}

QString SqlQueries::CreateSeasonsTable() {
    return "CREATE TABLE " + SEASONS_TABLE + " ("+ SEASON + " TEXT)";
}

QString SqlQueries::CreateSoldGoodsTable() {
    return "CREATE TABLE " + SOLD_GOODS_TABLE + " ("
            + MODEL_ID + " INTEGER NOT NULL, "
            + MODEL_NAME + " TEXT NOT NULL, "
            + BRAND + " TEXT NOT NULL, "
            + GOODS_ID + " INTEGER NOT NULL, "
            + GOODS_SIZE + " TEXT NOT NULL, "
            + GOODS_DATE + " TEXT NOT NULL, "
            + SALE_PRICE + " REAL NOT NULL, "
            + PROFIT + " REAL NOT NULL, "
            + SALE_DATE + " TEXT NOT NULL)";
}

QString SqlQueries::CountOfSoldGoodsByLastYear() {
    return "SELECT COUNT(*)"
           " FROM (SELECT * FROM "+ SOLD_GOODS_TABLE +" WHERE " + MODEL_ID + " = 2)"
           " WHERE " + SALE_DATE + " >= datetime('now', '-1 year')";
}

QString SqlQueries::AvailableGoodsWpriceSum() {
    return "SELECT SUM(" + WHOLESALE_PRICE + ")"
           " FROM ("
           " SELECT " + AVAILABLE_GOODS_TABLE + "." + MODEL_ID + ", " + MODELS_TABLE + "." + WHOLESALE_PRICE + ""
           " FROM " + AVAILABLE_GOODS_TABLE +
           " INNER JOIN " + MODELS_TABLE + " ON " + AVAILABLE_GOODS_TABLE + "." + MODEL_ID + " = " + MODELS_TABLE + "." + MODEL_ID + ")";
}

QString SqlQueries::CostsByMonth() {
    return "SELECT SUM(" + WHOLESALE_PRICE + ")"
           " FROM ("
           " SELECT " + AVAILABLE_GOODS_TABLE + "." + MODEL_ID + ", " + MODELS_TABLE + "." + WHOLESALE_PRICE + ""
           " FROM " + AVAILABLE_GOODS_TABLE +
           " INNER JOIN " + MODELS_TABLE + " ON " + AVAILABLE_GOODS_TABLE + "." + MODEL_ID + " = " + MODELS_TABLE + "." + MODEL_ID +
           " WHERE " + GOODS_DATE + " >= datetime('now', 'start of month'))";
}

QString SqlQueries::CostsByYear() {
    return "SELECT SUM(" + WHOLESALE_PRICE + ")"
           " FROM ("
           " SELECT " + AVAILABLE_GOODS_TABLE + "." + MODEL_ID + ", " + MODELS_TABLE + "." + WHOLESALE_PRICE + ""
           " FROM " + AVAILABLE_GOODS_TABLE +
           " INNER JOIN " + MODELS_TABLE + " ON " + AVAILABLE_GOODS_TABLE + "." + MODEL_ID + " = " + MODELS_TABLE + "." + MODEL_ID +
           " WHERE " + GOODS_DATE + " >= datetime('now', 'start of year'))";
}

QString SqlQueries::IncomeByMonth() {
    return "SELECT SUM("+ SALE_PRICE +")"
           " FROM " + SOLD_GOODS_TABLE +
           " WHERE " + SALE_DATE + ">= datetime('now', 'start of month')";
}

QString SqlQueries::IncomeByYear() {
    return "SELECT SUM("+ SALE_PRICE +")"
           " FROM " + SOLD_GOODS_TABLE +
           " WHERE " + SALE_DATE + ">= datetime('now', 'start of year')";
}

QString SqlQueries::AvailableGoods() {
    return "SELECT * FROM " + AVAILABLE_GOODS_TABLE;
}

QString SqlQueries::SoldGoods() {
    return "SELECT * FROM " + SOLD_GOODS_TABLE;
}

QString SqlQueries::SelectTableForReport(const QString &table_name, const QString &where, const QString &date_from, const QString &date_to)
{
    return "SELECT " + table_name + ".*, " + MODELS_TABLE + "." + CATEGORY + ", "
                                           + MODELS_TABLE + "." + SEASON + ", "
                                           + MODELS_TABLE + "." + WHOLESALE_PRICE + ", "
                                           + MODELS_TABLE + "." + RETAIL_PRICE +
           " FROM " + table_name +
           " INNER JOIN " + MODELS_TABLE + " ON " + table_name + "." + MODEL_ID + " = " + MODELS_TABLE + "." + MODEL_ID +
           " WHERE " + where + " BETWEEN '" + date_from + "' AND '" + date_to + "'";
}
