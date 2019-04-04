#include "addmodeldialog.h"
#include "ui_addmodeldialog.h"

AddModelDialog::AddModelDialog(QSqlDatabase* sdb_ptr, QWidget *parent) :
    QDialog(parent),
    ui_add_model_dialog(new Ui::AddModelDialog)
{
    ui_add_model_dialog->setupUi(this);
    setModal (true);
    photo_path = "";

    QSqlTableModel* model_category = new QSqlTableModel(this, *sdb_ptr);
    model_category->setTable ("category_dir");
    model_category->select ();
    ui_add_model_dialog->category_cb->setModel (model_category);

    QSqlTableModel* model_brand = new QSqlTableModel(this, *sdb_ptr);
    model_brand->setTable ("brand_dir");
    model_brand->select ();
    ui_add_model_dialog->brand_cb->setModel (model_brand);

    QSqlTableModel* model_season = new QSqlTableModel(this, *sdb_ptr);
    model_season->setTable ("season_dir");
    model_season->select ();
    ui_add_model_dialog->season_cb->setModel (model_season);

    connect(ui_add_model_dialog->add_pb, &QPushButton::clicked, this, &AddModelDialog::accept);
    connect(ui_add_model_dialog->cancel_pb, &QPushButton::clicked, this, &AddModelDialog::reject);
    connect(ui_add_model_dialog->load_pic_pb, &QPushButton::clicked, this, &AddModelDialog::LoadPic);

}
QString AddModelDialog::getModel() {
    return ui_add_model_dialog->model_le->text ();
}

QString AddModelDialog::getCategory () {
    return ui_add_model_dialog->category_cb->currentText ();
}

QString AddModelDialog::getSeason () {
    return ui_add_model_dialog->season_cb->currentText ();
}

QString AddModelDialog::getBrand () {
    return ui_add_model_dialog->brand_cb->currentText ();
}

double AddModelDialog::getWholesalepr () {
    return ui_add_model_dialog->wholesalepr_sb->value ();
}

double AddModelDialog::getRetailpr () {
    return ui_add_model_dialog->retailpr_sb ->value ();
}

QString AddModelDialog::getPhotoPath(){
    return photo_path;
}

AddModelDialog::~AddModelDialog() {
    delete ui_add_model_dialog;
}

void AddModelDialog::LoadPic(){
    photo_path = QFileDialog::getOpenFileName (this, "Виберіть фото моделі", "", "*.jpg *.png *.bmp");
}
