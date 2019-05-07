#ifndef RETURNGOODSDIALOG_H
#define RETURNGOODSDIALOG_H

#include <QDialog>
#include "general.h"
#include "database.h"

namespace Ui {
class ReturnGoodsDialog;
}

class ReturnGoodsDialog : public QDialog
{
    Q_OBJECT
    DataBase* sdb;
    QSqlTableModel* model;
    Ui::ReturnGoodsDialog *ui_return;
    QString goods_id;
    void SoldGoodsTableInit();
public:
    explicit ReturnGoodsDialog(DataBase* data_base, QWidget *parent = nullptr);
    QString GetGoodsId();
    QString GetModelName();
    QString GetBrand();
    ~ReturnGoodsDialog();
};

#endif // RETURNGOODSDIALOG_H
