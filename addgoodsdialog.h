#ifndef ADDGOODSDIALOG_H
#define ADDGOODSDIALOG_H

#include <QDialog>
#include "general.h"
#include "database.h"

namespace Ui {
class AddGoodsDialog;
}

class AddGoodsDialog : public QDialog
{
    Q_OBJECT
    Ui::AddGoodsDialog *ui_add_goods_dialog;
    QSqlTableModel* model;
public:
    explicit AddGoodsDialog(QWidget *parent = 0);
    ~AddGoodsDialog();

private slots:
    void ShowPic(QString text);
};

#endif // ADDGOODSDIALOG_H
