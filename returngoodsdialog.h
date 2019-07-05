#ifndef RETURNGOODSDIALOG_H
#define RETURNGOODSDIALOG_H

#include "general.h"
#include "database.h"
#include "sqlqueries.h"

namespace Ui {
class ReturnGoodsDialog;
}

class ReturnGoodsDialog : public QDialog
{
    Q_OBJECT
    DataBase* sdb;
    QSqlQueryModel* query_model;
    Ui::ReturnGoodsDialog *ui;
    QString goods_id;
    void SoldGoodsTableInit();
private slots:
    void EnableReturnButton();
public:
    explicit ReturnGoodsDialog(DataBase* data_base, QWidget *parent = nullptr);
    QString GetGoodsId();
    QString GetModelName();
    QString GetBrand();
    QString GetSize();
    ~ReturnGoodsDialog();
};

#endif // RETURNGOODSDIALOG_H
