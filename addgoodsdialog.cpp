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

    cb_list << ui_add_goods_dialog->cb_36 <<
               ui_add_goods_dialog->cb_37 <<
               ui_add_goods_dialog->cb_38 <<
               ui_add_goods_dialog->cb_39 <<
               ui_add_goods_dialog->cb_40 <<
               ui_add_goods_dialog->cb_41 <<
               ui_add_goods_dialog->cb_42 <<
               ui_add_goods_dialog->cb_43 <<
               ui_add_goods_dialog->cb_44 <<
               ui_add_goods_dialog->cb_45 <<
               ui_add_goods_dialog->cb_46;

    sb_list << ui_add_goods_dialog->sb_36 <<
               ui_add_goods_dialog->sb_37 <<
               ui_add_goods_dialog->sb_38 <<
               ui_add_goods_dialog->sb_39 <<
               ui_add_goods_dialog->sb_40 <<
               ui_add_goods_dialog->sb_41 <<
               ui_add_goods_dialog->sb_42 <<
               ui_add_goods_dialog->sb_43 <<
               ui_add_goods_dialog->sb_44 <<
               ui_add_goods_dialog->sb_45 <<
               ui_add_goods_dialog->sb_46;
    QObject::connect (ui_add_goods_dialog->model_cb, &QComboBox::currentTextChanged, this, &AddGoodsDialog::ShowPic);
    for (int i = 0; i < cb_list.size (); ++i) {
        QObject::connect (cb_list[i], &QCheckBox::clicked, this, [=](){ InitSpinBox(i); });
        QObject::connect (sb_list[i], static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &AddGoodsDialog::SetRes);
    }
    QObject::connect(ui_add_goods_dialog->add_goods_pb, &QPushButton::clicked, this, &AddGoodsDialog::accept);
    QObject::connect(ui_add_goods_dialog->cancel_pb, &QPushButton::clicked, this, &AddGoodsDialog::reject);
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

void AddGoodsDialog::InitSpinBox(int indx) {
    if (cb_list.at (indx)->isChecked ()){
        sb_list.at (indx)->setValue (1);
    }
    else {
        sb_list.at (indx)->setValue (0);
    }
}

void AddGoodsDialog::SetRes(int i) {
    ui_add_goods_dialog->result_le->clear ();
    for (auto& sb : sb_list) {
        ui_add_goods_dialog->result_le->setText (QString::number (ui_add_goods_dialog->result_le->text ().toInt () + sb->value ()));
    }
}

AddGoodsDialog::~AddGoodsDialog() {
    delete ui_add_goods_dialog;
}
