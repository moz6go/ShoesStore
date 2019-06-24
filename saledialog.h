#ifndef SALEDIALOG_H
#define SALEDIALOG_H

#include "general.h"
#include "database.h"
#include "sqlqueries.h"

namespace Ui {
class SaleDialog;
}

class SaleDialog : public QDialog
{
    Q_OBJECT
    DataBase* sdb;
    QVariantList row;
    Ui::SaleDialog *ui;
public:
    explicit SaleDialog(DataBase* data_base, const QVariantList& curr_row, QWidget *parent = nullptr);
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
