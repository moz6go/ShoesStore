#include "addgoodsdialog.h"
#include "ui_addgoodsdialog.h"

AddGoodsDialog::AddGoodsDialog(DataBase* data_base, QWidget *parent) :
    QDialog(parent),
    ui_add_goods_dialog(new Ui::AddGoodsDialog)
{
    ui_add_goods_dialog->setupUi(this);
    sdb = data_base;
    model = new QSqlTableModel(this);
    model->setTable (MODELS_TABLE);
    model->select ();

//    QDataWidgetMapper* mapper = new QDataWidgetMapper;
//    mapper->setModel (model);
//    mapper->addMapping (ui_add_goods_dialog->model_cb, 0);
//    mapper->toFirst ();
    ui_add_goods_dialog->model_cb->setModel (model);
    ui_add_goods_dialog->model_cb->setModelColumn (1);

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
    for (int i = 0; i < sb_list.size (); ++i) {
        QObject::connect (sb_list[i], static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &AddGoodsDialog::SetRes);
    }
    QObject::connect (ui_add_goods_dialog->result_le, &QLineEdit::textChanged, this, &AddGoodsDialog::EnableAddButton);
    QObject::connect (ui_add_goods_dialog->add_goods_pb, &QPushButton::clicked, this, &AddGoodsDialog::accept);
    QObject::connect (ui_add_goods_dialog->cancel_pb, &QPushButton::clicked, this, &AddGoodsDialog::reject);
    ShowPic(ui_add_goods_dialog->model_cb->currentText ());
}

QList<QSpinBox*> &AddGoodsDialog::GetSbList() {
    return sb_list;
}

int AddGoodsDialog::GetGoodsCount() {
    return ui_add_goods_dialog->result_le->text ().toInt ();
}

QString AddGoodsDialog::GetModelName() {
    return ui_add_goods_dialog->model_cb->currentText ();
}

void AddGoodsDialog::ShowPic(QString text) {
    QPixmap pic;
    pic.loadFromData (sdb->SelectPic (MODELS_TABLE, MODEL_NAME, text));
    ui_add_goods_dialog->pic_lbl->setPixmap (pic.scaledToWidth (300));
    if(pic.width () > pic.height ()) {
        ui_add_goods_dialog->pic_lbl->setAlignment (Qt::AlignTop);
    }
    else {
        ui_add_goods_dialog->pic_lbl->setAlignment (Qt::AlignCenter);
    }
}

void AddGoodsDialog::SetRes(int i) {
    ui_add_goods_dialog->result_le->clear ();
    for (auto& sb : sb_list) {
        ui_add_goods_dialog->result_le->setText (QString::number (ui_add_goods_dialog->result_le->text ().toInt () + sb->value ()));
    }
}

void AddGoodsDialog::EnableAddButton() {
    if (ui_add_goods_dialog->result_le->text ().toInt ()) {
        ui_add_goods_dialog->add_goods_pb->setEnabled (true);
    }
    else {
        ui_add_goods_dialog->add_goods_pb->setDisabled (true);
    }
}

AddGoodsDialog::~AddGoodsDialog() {
    delete ui_add_goods_dialog;
}
