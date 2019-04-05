#include "addgoodsdialog.h"
#include "ui_addgoodsdialog.h"

AddGoodsDialog::AddGoodsDialog(QSqlDatabase* sdb_ptr, QWidget *parent) :
    QDialog(parent),
    ui_add_goods_dialog(new Ui::AddGoodsDialog)
{
    ui_add_goods_dialog->setupUi(this);
    QSqlTableModel* model = new QSqlTableModel(this, *sdb_ptr);
    model->setTable ("model_dir");
    model->select ();
    ui_add_goods_dialog->model_cb->setModel (model);
    ui_add_goods_dialog->model_cb->setModelColumn (1);

}

AddGoodsDialog::~AddGoodsDialog() {
    delete ui_add_goods_dialog;
}
