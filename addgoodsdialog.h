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
    QList<QSpinBox*> sb_list;
    QDataWidgetMapper* mapper;
    Ui::AddGoodsDialog *ui_add_goods_dialog;
public:
    explicit AddGoodsDialog(DataBase* data_base, int row, QSortFilterProxyModel* filter_model, QWidget *parent = nullptr);
    QList<QSpinBox*>& GetSbList();
    int GetGoodsCount();
    QString GetModelName();
    QString GetBrand();
    ~AddGoodsDialog();

private slots:
    void ShowPic(QString text);
    void SetRes(int i);
    void EnableAddButton();
};

#endif // ADDGOODSDIALOG_H
