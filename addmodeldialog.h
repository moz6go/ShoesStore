#ifndef ADDMODELDIALOG_H
#define ADDMODELDIALOG_H

#include "general.h"
#include "database.h"

namespace Ui {
class AddModelDialog;
}

class AddModelDialog : public QDialog
{
    Q_OBJECT
    Ui::AddModelDialog *ui;

    DataBase* sdb;
    QString photo_path;
    QVariantList row;
    bool isPicDel;
public:
    explicit AddModelDialog(DataBase* data_base, const QVariantList& curr_row, QWidget *parent = nullptr);
    QString getModel();
    QString getCategory ();
    QString getSeason();
    QString getBrand();
    QString getWholesalepr();
    QString getRetailpr();
    QString getPhotoPath();
    bool isPicDeleted();
    ~AddModelDialog();
private slots:
    void LoadPic();
    void EnableAddButton();
    void onClosePbClick();
};

#endif // ADDMODELDIALOG_H
