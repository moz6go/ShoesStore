#include "addmodeldialog.h"
#include "ui_addmodeldialog.h"

AddModelDialog::AddModelDialog (DataBase* data_base, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddModelDialog)
{
    ui->setupUi(this);
    setModal (true);
    photo_path = "";
    sdb = data_base;
    ui->warning_lbl->setVisible (false);
    ui->pic_lbl->setPixmap (QPixmap(":/pics/default_pic.png"));

    QSqlTableModel* model_category = new QSqlTableModel(this);
    model_category->setTable (CATEGORIES_TABLE);
    model_category->select ();
    ui->category_cb->setModel (model_category);

    QSqlTableModel* model_brand = new QSqlTableModel(this);
    model_brand->setTable (BRANDS_TABLE);
    model_brand->select ();
    ui->brand_cb->setModel (model_brand);

    QSqlTableModel* model_season = new QSqlTableModel(this);
    model_season->setTable (SEASONS_TABLE);
    model_season->select ();
    ui->season_cb->setModel (model_season);

    QObject::connect(ui->add_pb, &QPushButton::clicked, this, &AddModelDialog::accept);
    QObject::connect(ui->cancel_pb, &QPushButton::clicked, this, &AddModelDialog::reject);
    QObject::connect(ui->load_pic_pb, &QPushButton::clicked, this, &AddModelDialog::LoadPic);
    QObject::connect (ui->model_le, &QLineEdit::textChanged, this, &AddModelDialog::EnableAddButton);
    QObject::connect (this, &AddModelDialog::PhotoLoaded, this, &AddModelDialog::EnableAddButton);
}

QString AddModelDialog::getModel() {
    return ui->model_le->text ();
}

QString AddModelDialog::getCategory () {
    return ui->category_cb->currentText ();
}

QString AddModelDialog::getSeason () {
    return ui->season_cb->currentText ();
}

QString AddModelDialog::getBrand () {
    return ui->brand_cb->currentText ();
}

double AddModelDialog::getWholesalepr () {
    return ui->wholesalepr_sb->value ();
}

double AddModelDialog::getRetailpr () {
    return ui->retailpr_sb ->value ();
}

QString AddModelDialog::getPhotoPath(){
    return photo_path;
}

AddModelDialog::~AddModelDialog() {
    delete ui;
}

void AddModelDialog::LoadPic(){
    photo_path = QFileDialog::getOpenFileName (this, "Виберіть фото моделі", QDir::homePath (), "*.jpg *.png *.bmp");
    if (!photo_path.isEmpty ()){
        ui->pic_name_lbl->setText (photo_path.right (photo_path.size () - photo_path.lastIndexOf ('/') - 1));
        ui->pic_lbl->setPixmap (QPixmap(photo_path).scaledToHeight (ui->pic_lbl->height ()));
    }
    else {
        ui->pic_name_lbl->setText("");
        ui->pic_lbl->setPixmap (QPixmap(":/pics/default_pic.png"));
    }
    emit PhotoLoaded ();
}

void AddModelDialog::EnableAddButton() {

    bool photo_ok = !photo_path.isEmpty ();
    bool text_ok = !ui->model_le->text ().isEmpty ();
    bool model_ok = !sdb->SelectCount (MODELS_TABLE, MODEL_NAME, "=", ui->model_le->text ());

    if(photo_ok && text_ok && model_ok) {
        ui->warning_lbl->setVisible (false);
        ui->add_pb->setEnabled (true);
    }
    else if (model_ok) {
        ui->warning_lbl->setVisible (false);
        ui->add_pb->setDisabled (true);
    }
    else {
        ui->warning_lbl->setVisible (true);
        ui->add_pb->setDisabled (true);
    }
}
