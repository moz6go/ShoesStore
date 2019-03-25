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
    QLabel* model_lb;
    QLineEdit* model_le;
    QHBoxLayout* model_layout;

    QLabel* season_lb;
    QLineEdit* season_le;
    QHBoxLayout* season_layout;

    QLabel* brand_lb;
    QLineEdit* brand_le;
    QHBoxLayout* brand_layout;

    QLabel* wholesaleprice_lb;
    QLineEdit* wholesaleprice_le;
    QHBoxLayout* wholesaleprice_layout;

    QLabel* retailprice_lb;
    QLineEdit* retailprice_le;
    QHBoxLayout* retailprice_layout;


    QVBoxLayout* main_layout;

public:
    explicit AddModelDialog(QWidget *parent = 0);
    QString getModel();
    QString getSeason();
    QString getBrand();
    double getWholesalepr();
    double getRetailpr();
    ~AddModelDialog();

private:
    Ui::AddModelDialog *ui_add_model_dialog;
};

#endif // ADDMODELDIALOG_H
