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
    QTextEdit* model_te;
    QHBoxLayout* model_layout;

    QLabel* season_lb;
    QTextEdit* season_te;
    QHBoxLayout* season_layout;

    QVBoxLayout* main_layout;

public:
    explicit AddModelDialog(QWidget *parent = 0);
    ~AddModelDialog();

private:
    Ui::AddModelDialog *ui_add_model_dialog;
};

#endif // ADDMODELDIALOG_H
