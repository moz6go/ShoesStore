#include "saledialog.h"
#include "ui_saledialog.h"

SaleDialog::SaleDialog(DataBase* data_base, const QVariantList &curr_row, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaleDialog)
{
    ui->setupUi(this);
    setModal (true);

    sdb = data_base;
    row = curr_row;
    QSqlQueryModel* brand_model = new QSqlQueryModel(this);
    brand_model->setQuery (SqlQueries::AvailableBrands());
    ui->brand_cb->setModel (brand_model);
    ui->brand_cb->setCurrentText (row.at (BRAND_COL).toString ());
    UpdateModelList (ui->brand_cb->currentText ());

    QObject::connect (ui->model_cb, &QComboBox::currentTextChanged, this, &SaleDialog::ShowInfo);
    QObject::connect (ui->brand_cb, &QComboBox::currentTextChanged, this, &SaleDialog::UpdateModelList);
    QObject::connect (ui->size_cb, &QComboBox::currentTextChanged, this, &SaleDialog::UpdateCount);

    QObject::connect (ui->ok_pb, &QPushButton::clicked, this, &SaleDialog::accept);
    QObject::connect (ui->cancel_pb, &QPushButton::clicked, this, &SaleDialog::reject);
    ShowInfo(ui->model_cb->currentText ());
}

QString SaleDialog::GetModel() {
    return ui->model_cb->currentText ();
}

QString SaleDialog::GetSize() {
    return ui->size_cb->currentText ();
}

double SaleDialog::GetPrice() {
    return ui->price_sb->value ();
}

double SaleDialog::GetProfit() {
    return ui->price_sb->value () - ui->wholesale_price_sb->value ();
}

int SaleDialog::GetCount() {
    return ui->count_sb->value ();
}

SaleDialog::~SaleDialog() {
    delete ui;
}

void SaleDialog::ShowInfo(QString text) {
    QPixmap pic;
    pic.loadFromData (sdb->SelectPic (MODELS_TABLE, MODEL_NAME, text));
    if (!pic.isNull ()){
        ui->pic_lbl->setPixmap(pic.scaledToHeight (ui->pic_lbl->height ()));
    }
    else {
        ui->pic_lbl->setPixmap(QPixmap(":/pics/default_pic.png"));
    }
    UpdatePrices ();
    UpdateSizes (ui->model_cb->currentText ());
}

void SaleDialog::UpdateModelList(QString brand) {
    QSqlQueryModel* model = new QSqlQueryModel(this);
    model->setQuery (SqlQueries::ModelsListByBrand(brand));
    ui->model_cb->setModel (model);
    ui->model_cb->setCurrentText(row.at(MODEL_NAME_COL).toString ());
}

void SaleDialog::UpdateSizes(QString model) {
    QString model_id = sdb->Select (MODEL_ID, MODELS_TABLE, MODEL_NAME, model);
    QSqlQueryModel* model_sizes = new QSqlQueryModel(this);
    model_sizes->setQuery (SqlQueries::AvailableSizesByModelId(model_id));
    ui->size_cb->setModel (model_sizes);
}

void SaleDialog::UpdatePrices() {
    ui->wholesale_price_sb->setValue (sdb->Select (WHOLESALE_PRICE, MODELS_TABLE, MODEL_NAME, ui->model_cb->currentText ()).toDouble ());
    ui->retail_price_sb->setValue (sdb->Select (RETAIL_PRICE, MODELS_TABLE, MODEL_NAME, ui->model_cb->currentText ()).toDouble ());
    ui->price_sb->setValue (ui->retail_price_sb->value ());
}

void SaleDialog::UpdateCount() {
    int count = sdb->SelectCount (AVAILABLE_GOODS_TABLE, MODEL_NAME, GOODS_SIZE, "=", "=", ui->model_cb->currentText (), ui->size_cb->currentText ());
    ui->count_sb->setValue (1);
    ui->count_sb->setMaximum (count);
}
