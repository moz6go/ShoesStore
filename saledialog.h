#ifndef SALEDIALOG_H
#define SALEDIALOG_H

#include <QDialog>
#include "general.h"
#include "database.h"

namespace Ui {
class SaleDialog;
}

class SaleDialog : public QDialog
{
    Q_OBJECT
    DataBase* sdb;
    Ui::SaleDialog *ui_sale;
public:
    explicit SaleDialog(DataBase* data_base, QWidget *parent = nullptr);
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
