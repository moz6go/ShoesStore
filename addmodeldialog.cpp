#include "addmodeldialog.h"
#include "ui_addmodeldialog.h"

AddModelDialog::AddModelDialog (DataBase* data_base, const QVariantList &curr_row, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddModelDialog)
{
    ui->setupUi(this);
    setModal (true);
    photo_path = "";
    sdb = data_base;
    row = curr_row;
    ui->warning_lbl->setVisible (false);

    ui->pic_lbl->setPixmap (QPixmap(":/pics/default_pic.png"));
    ui->close_pb->setVisible (false);

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

    if (row.size ()) {
        isPicDel = false;

        ui->model_le->setText (row.at (MODEL_NAME_COL).toString ());
        ui->category_cb->setCurrentText (row.at (CATEGORY_COL).toString ());
        ui->brand_cb->setCurrentText (row.at (BRAND_COL).toString ());
        ui->season_cb->setCurrentText (row.at (SEASON_COL).toString ());
        ui->wholesalepr_sb->setValue (row.at(WHOLESALE_PRICE_COL).toDouble ());
        ui->retailpr_sb->setValue (row.at(RETAIL_PRICE_COL).toDouble ());

        QPixmap photo;
        photo.loadFromData (row.at (PIC_COL).toByteArray ());
        if (!photo.isNull ()){
            ui->pic_lbl->setPixmap(photo.scaledToHeight (ui->pic_lbl->height ()));
            ui->close_pb->setVisible (true);
        }
        else {
            ui->pic_lbl->setPixmap(QPixmap(":/pics/default_pic.png"));
            ui->close_pb->setVisible (false);
        }

        if (sdb->SelectCount (AVAILABLE_GOODS_TABLE, MODEL_ID, "=", row.at(MODEL_ID_COL).toString()) ||
            sdb->SelectCount (SOLD_GOODS_TABLE, MODEL_ID, "=", row.at(MODEL_ID_COL).toString())) {
            ui->model_le->setDisabled(true);
            ui->category_cb->setDisabled(true);
            ui->brand_cb->setDisabled(true);
            ui->season_cb->setDisabled(true);
            ui->wholesalepr_sb->setDisabled(true);
            ui->retailpr_sb->setDisabled(true);
        }
        setWindowTitle ("Редагувати модель");
        EnableAddButton();
    }
    QObject::connect (ui->close_pb, &QPushButton::clicked, this, &AddModelDialog::onClosePbClick);
    QObject::connect(ui->add_pb, &QPushButton::clicked, this, &AddModelDialog::accept);
    QObject::connect(ui->cancel_pb, &QPushButton::clicked, this, &AddModelDialog::reject);
    QObject::connect(ui->load_pic_pb, &QPushButton::clicked, this, &AddModelDialog::LoadPic);
    QObject::connect (ui->model_le, &QLineEdit::textChanged, this, &AddModelDialog::EnableAddButton);
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

QString AddModelDialog::getWholesalepr () {
    return QString::number (ui->wholesalepr_sb->value ());
}

QString AddModelDialog::getRetailpr() {
    return QString::number (ui->retailpr_sb->value ());
}

QString AddModelDialog::getPhotoPath(){
    return photo_path;
}

bool AddModelDialog::isPicDeleted() {
    return isPicDel;
}

AddModelDialog::~AddModelDialog() {
    delete ui;
}

void AddModelDialog::LoadPic(){
    photo_path = QFileDialog::getOpenFileName (this, "Виберіть фото моделі", QDir::homePath (), "*.jpg *.png *.bmp");
    if (!photo_path.isEmpty ()){
        ui->pic_name_lbl->setText (photo_path.right (photo_path.size () - photo_path.lastIndexOf ('/') - 1));
        ui->pic_lbl->setPixmap (QPixmap(photo_path).scaledToHeight (ui->pic_lbl->height ()));
        ui->close_pb->setVisible (true);
    }
}

void AddModelDialog::EnableAddButton() {

    bool text_ok = !ui->model_le->text ().isEmpty ();
    bool model_ok = !sdb->SelectCount (MODELS_TABLE, MODEL_NAME, "=", ui->model_le->text ()) || row.at (MODEL_NAME_COL).toString () == ui->model_le->text ();

    if(text_ok && model_ok) {
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

void AddModelDialog::onClosePbClick() {
    photo_path = "";
    ui->pic_name_lbl->setText ("");
    ui->pic_lbl->setPixmap(QPixmap(":/pics/default_pic.png"));
    ui->close_pb->setVisible (false);
    isPicDel = true;
}
