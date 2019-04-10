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
    QList<QCheckBox*> cb_list;
    QList<QSpinBox*> sb_list;
public:
    explicit AddGoodsDialog(QWidget *parent = 0);
    ~AddGoodsDialog();

private slots:
    void ShowPic(QString text);
    void InitSpinBox(int indx);
    void SetRes(int i);
};

#endif // ADDGOODSDIALOG_H
