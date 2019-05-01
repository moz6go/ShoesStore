#ifndef ADDMODELDIALOG_H
#define ADDMODELDIALOG_H

#include <QDialog>
#include "general.h"
#include "database.h"

namespace Ui {
class AddModelDialog;
}

class AddModelDialog : public QDialog
{
    Q_OBJECT
    DataBase* sdb;
    QString photo_path;
    Ui::AddModelDialog *ui_add_model_dialog;
public:
    explicit AddModelDialog(DataBase* data_base, QWidget *parent = nullptr);
    QString getModel();
    QString getCategory ();
    QString getSeason();
    QString getBrand();
    double getWholesalepr();
    double getRetailpr();
    QString getPhotoPath();
    ~AddModelDialog();
private slots:
    void LoadPic();
    void EnableAddButton();
signals:
    void PhotoLoaded();
};

#endif // ADDMODELDIALOG_H
