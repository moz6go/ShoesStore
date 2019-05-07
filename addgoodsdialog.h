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
    DataBase* sdb;
    QSortFilterProxyModel* f_model;
    int current_row;
    QList<QSpinBox*> sb_list;
    Ui::AddGoodsDialog *ui_add_goods_dialog;
public:
    explicit AddGoodsDialog(DataBase* data_base, int row, QSortFilterProxyModel* model, QWidget *parent = nullptr);
    QList<QSpinBox*>& GetSbList();
    int GetGoodsCount();
    QString GetModelName();
    QString GetBrand();
    ~AddGoodsDialog();

private slots:
    void ShowPic(QString text);
    void UpdateModelList(QString brand);
    void SetRes(int i);
    void EnableAddButton();
};

#endif // ADDGOODSDIALOG_H
