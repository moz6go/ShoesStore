#ifndef ADDGOODSDIALOG_H
#define ADDGOODSDIALOG_H

#include "general.h"
#include "database.h"
#include "sqlqueries.h"

namespace Ui {
class AddGoodsDialog;
}

class AddGoodsDialog : public QDialog
{
    Q_OBJECT
    DataBase* sdb;
    QVariantList row;
    QList<QSpinBox*> sb_list;
    Ui::AddGoodsDialog *ui;
public:
    explicit AddGoodsDialog(DataBase* data_base, const QVariantList& curr_row, QWidget *parent = nullptr);
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
