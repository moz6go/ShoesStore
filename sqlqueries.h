#ifndef SQLQUERIES_H
#define SQLQUERIES_H
#include "general.h"

class SqlQueries
{
public:
    SqlQueries();
    static QString CreateModelsTable();
    static QString CreateAvailablesGoodsTable();
    static QString CreateBrandsTable();
    static QString CreateCategoriesTable();
    static QString CreateSeasonsTable();
    static QString CreateSoldGoodsTable();

    static QString CountOfSoldGoodsByLastYear();
    static QString AvailableGoodsWpriceSum();
    static QString CostsByMonth();
    static QString CostsByYear();
    static QString IncomeByMonth();
    static QString IncomeByYear();
    static QString AvailableGoods();
    static QString SoldGoods();
    static QString SelectTableForReport(const QString &table_name, const QString& where, const QString& date_from, const QString& date_to);
    static QString BrandsFrom(const QString& table);
    static QString ModelsListByBrand(const QString& table, const QString& brand);
    static QString AvailableSizesByModelId(const QString& model_id);
    static QString SoldGoodsByLastYear(const QString& model_id);
    static QString SoldGoodsForReturn();
};

#endif // SQLQUERIES_H
