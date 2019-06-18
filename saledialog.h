#ifndef SALEDIALOG_H
#define SALEDIALOG_H

#include "general.h"
#include "database.h"

namespace Ui {
class SaleDialog;
}

class SaleDialog : public QDialog
{
    Q_OBJECT
    DataBase* sdb;
    QSortFilterProxyModel* f_model;
    int current_row;
    Ui::SaleDialog *ui_sale;
public:
    explicit SaleDialog(DataBase* data_base, int row, QSortFilterProxyModel* model, QWidget *parent = nullptr);
    QString GetModel();
    QString GetSize();
    double GetPrice();
    double GetProfit();
    int GetCount();
    ~SaleDialog();

private slots:
    void ShowInfo(QString text);
    void UpdateModelList(QString brand);
    void UpdateSizes(QString model);
    void UpdatePrices();
    void UpdateCount();
};

#endif // SALEDIALOG_H
