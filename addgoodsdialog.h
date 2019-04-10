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
    QList<QSpinBox*> sb_list;
public:
    explicit AddGoodsDialog(QWidget *parent = nullptr);
    QList<QSpinBox*>& GetSbList();
    ~AddGoodsDialog();

private slots:
    void ShowPic(QString text);
    void SetRes(int i);
    void EnableAddButton();
};

#endif // ADDGOODSDIALOG_H
