#ifndef ADDMODELDIALOG_H
#define ADDMODELDIALOG_H

#include <QDialog>
#include "general.h"

namespace Ui {
class AddModelDialog;
}

class AddModelDialog : public QDialog
{
    Q_OBJECT
    QString photo_path;
public:
    explicit AddModelDialog(QSqlDatabase* sdb_ptr, QWidget *parent = 0);
    QString getModel();
    QString getCategory ();
    QString getSeason();
    QString getBrand();
    double getWholesalepr();
    double getRetailpr();
    QString getPhotoPath();
    ~AddModelDialog();
private:
    Ui::AddModelDialog *ui_add_model_dialog;
private slots:
    void LoadPic();
};

#endif // ADDMODELDIALOG_H
