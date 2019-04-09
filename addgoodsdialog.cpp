#include "addgoodsdialog.h"
#include "ui_addgoodsdialog.h"

AddGoodsDialog::AddGoodsDialog(QWidget *parent) :
    QDialog(parent),
    ui_add_goods_dialog(new Ui::AddGoodsDialog)
{
    ui_add_goods_dialog->setupUi(this);
    model = new QSqlTableModel(this);
    model->setTable ("model_dir");
    model->select ();
    ui_add_goods_dialog->model_cb->setModel (model);
    ui_add_goods_dialog->model_cb->setModelColumn (1);
    ui_add_goods_dialog->pic_lbl->clear ();
    QObject::connect (ui_add_goods_dialog->model_cb, &QComboBox::currentTextChanged, this, &AddGoodsDialog::ShowPic);
}


void AddGoodsDialog::ShowPic(QString text) {
    QPixmap pic;
    QSqlQuery query;
    query.exec ("SELECT pic FROM model_dir WHERE model_name = '" + text + "'");
    QSqlRecord rec = query.record ();
    query.next ();
    pic.loadFromData (query.value (rec.indexOf ("pic")).toByteArray ());
    ui_add_goods_dialog->pic_lbl->setPixmap (pic.scaledToWidth (300));
    if(pic.width () > pic.height ()) {
        ui_add_goods_dialog->pic_lbl->setAlignment (Qt::AlignTop);
    }
    else {
        ui_add_goods_dialog->pic_lbl->setAlignment (Qt::AlignCenter);
    }
}

AddGoodsDialog::~AddGoodsDialog() {
    delete ui_add_goods_dialog;
}
