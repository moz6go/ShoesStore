#include "addmodeldialog.h"
#include "ui_addmodeldialog.h"

AddModelDialog::AddModelDialog (DataBase* data_base, QWidget *parent) :
    QDialog(parent),
    ui_add_model_dialog(new Ui::AddModelDialog)
{
    ui_add_model_dialog->setupUi(this);
    setModal (true);
    photo_path = "";
    sdb = data_base;
    ui_add_model_dialog->warning_lbl->setVisible (false);

    QSqlTableModel* model_category = new QSqlTableModel(this);
    model_category->setTable (CATEGORIES_TABLE);
    model_category->select ();
    ui_add_model_dialog->category_cb->setModel (model_category);

    QSqlTableModel* model_brand = new QSqlTableModel(this);
    model_brand->setTable (BRANDS_TABLE);
    model_brand->select ();
    ui_add_model_dialog->brand_cb->setModel (model_brand);

    QSqlTableModel* model_season = new QSqlTableModel(this);
    model_season->setTable (SEASONS_TABLE);
    model_season->select ();
    ui_add_model_dialog->season_cb->setModel (model_season);

    QObject::connect(ui_add_model_dialog->add_pb, &QPushButton::clicked, this, &AddModelDialog::accept);
    QObject::connect(ui_add_model_dialog->cancel_pb, &QPushButton::clicked, this, &AddModelDialog::reject);
    QObject::connect(ui_add_model_dialog->load_pic_pb, &QPushButton::clicked, this, &AddModelDialog::LoadPic);
    QObject::connect (ui_add_model_dialog->model_le, &QLineEdit::textChanged, this, &AddModelDialog::EnableAddButton);
    QObject::connect (this, &AddModelDialog::PhotoLoaded, this, &AddModelDialog::EnableAddButton);
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
    photo_path = QFileDialog::getOpenFileName (this, "Виберіть фото моделі", QDir::homePath (), "*.jpg *.png *.bmp");
    if (!photo_path.isEmpty ()){
        ui_add_model_dialog->pic_lbl->setText (photo_path.right (photo_path.size () - photo_path.lastIndexOf ('/') - 1));
    }
    else {
        ui_add_model_dialog->pic_lbl->setText("");
    }
    emit PhotoLoaded ();
}

void AddModelDialog::EnableAddButton() {

    bool photo_ok = !photo_path.isEmpty ();
    bool text_ok = !ui_add_model_dialog->model_le->text ().isEmpty ();
    bool model_ok = !sdb->SelectCount (MODELS_TABLE, MODEL_NAME, "=", ui_add_model_dialog->model_le->text ());

    if(photo_ok && text_ok && model_ok) {
        ui_add_model_dialog->warning_lbl->setVisible (false);
        ui_add_model_dialog->add_pb->setEnabled (true);
    }
    else if (model_ok) {
        ui_add_model_dialog->warning_lbl->setVisible (false);
        ui_add_model_dialog->add_pb->setDisabled (true);
    }
    else {
        ui_add_model_dialog->warning_lbl->setVisible (true);
        ui_add_model_dialog->add_pb->setDisabled (true);
    }
}
