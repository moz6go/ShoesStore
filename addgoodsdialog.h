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

public:
    explicit AddGoodsDialog(QWidget *parent = 0);
    ~AddGoodsDialog();

private:
    Ui::AddGoodsDialog *ui_add_goods_dialog;
};

#endif // ADDGOODSDIALOG_H
