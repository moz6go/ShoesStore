#include "addgoodsdialog.h"
#include "ui_addgoodsdialog.h"

AddGoodsDialog::AddGoodsDialog(DataBase* data_base, int row, QSortFilterProxyModel* model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddGoodsDialog)
{
    ui->setupUi(this);
    setModal (true);
    sdb = data_base;
    f_model = model;
    current_row = row;

    QSqlQueryModel* brand_model = new QSqlQueryModel(this);
    brand_model->setQuery ("SELECT DISTINCT " + BRAND + " FROM " + MODELS_TABLE);
    ui->brand_cb->setModel (brand_model);
    ui->brand_cb->setCurrentText (f_model->data (f_model->index (current_row, 4)).toString ());

    UpdateModelList (ui->brand_cb->currentText ());

    sb_list << ui->sb_36 <<
               ui->sb_37 <<
               ui->sb_38 <<
               ui->sb_39 <<
               ui->sb_40 <<
               ui->sb_41 <<
               ui->sb_42 <<
               ui->sb_43 <<
               ui->sb_44 <<
               ui->sb_45 <<
               ui->sb_46 <<
               ui->sb_ws;


    QObject::connect (ui->model_cb, &QComboBox::currentTextChanged, this, &AddGoodsDialog::ShowPic);
    QObject::connect (ui->brand_cb, &QComboBox::currentTextChanged, this, &AddGoodsDialog::UpdateModelList);
    for (int i = 0; i < sb_list.size (); ++i) {
        QObject::connect (sb_list[i], static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &AddGoodsDialog::SetRes);
    }
    QObject::connect (ui->result_le, &QLineEdit::textChanged, this, &AddGoodsDialog::EnableAddButton);
    QObject::connect (ui->add_goods_pb, &QPushButton::clicked, this, &AddGoodsDialog::accept);
    QObject::connect (ui->cancel_pb, &QPushButton::clicked, this, &AddGoodsDialog::reject);
    ShowPic(ui->model_cb->currentText ());
}

QList<QSpinBox*> &AddGoodsDialog::GetSbList() {
    return sb_list;
}

int AddGoodsDialog::GetGoodsCount() {
    return ui->result_le->text ().toInt ();
}

QString AddGoodsDialog::GetModelName() {
    return ui->model_cb->currentText ();
}

QString AddGoodsDialog::GetBrand() {
    return ui->brand_cb->currentText ();
}

void AddGoodsDialog::ShowPic(QString text) {
    QPixmap photo;
    photo.loadFromData (sdb->SelectPic (MODELS_TABLE, MODEL_NAME, text));
    if (!photo.isNull ()){
        ui->pic_lbl->setPixmap(photo.scaledToHeight (ui->pic_lbl->height ()));
    }
    else {
        ui->pic_lbl->setPixmap(QPixmap(":/pics/default_pic.png"));
    }
}

void AddGoodsDialog::UpdateModelList(QString brand) {
    QSqlQueryModel* model = new QSqlQueryModel(this);
    model->setQuery ("SELECT " + MODEL_NAME + " FROM " + MODELS_TABLE + " WHERE " + BRAND + " ='" + brand + "'");
    ui->model_cb->setModel (model);
    ui->model_cb->setCurrentText (f_model->data(f_model->index (current_row, 1)).toString ());
}

void AddGoodsDialog::SetRes(int) {
    ui->result_le->clear ();
    for (auto& sb : sb_list) {
        ui->result_le->setText (QString::number (ui->result_le->text ().toInt () + sb->value ()));
    }
}

void AddGoodsDialog::EnableAddButton() {
    if (ui->result_le->text ().toInt ()) {
        ui->add_goods_pb->setEnabled (true);
    }
    else {
        ui->add_goods_pb->setDisabled (true);
    }
}

AddGoodsDialog::~AddGoodsDialog() {
    delete ui;
}
