#include "saledialog.h"
#include "ui_saledialog.h"

SaleDialog::SaleDialog(DataBase* data_base, int row, QSortFilterProxyModel* model, QWidget *parent) :
    QDialog(parent),
    ui_sale(new Ui::SaleDialog)
{
    ui_sale->setupUi(this);
    setModal (true);

    sdb = data_base;
    f_model = model;
    current_row = row;
    QSqlQueryModel* brand_model = new QSqlQueryModel(this);
    brand_model->setQuery ("SELECT DISTINCT " + BRAND + " FROM " + AVAILABLE_GOODS_TABLE);
    ui_sale->brand_cb->setModel (brand_model);
    ui_sale->brand_cb->setCurrentText (f_model->data (f_model->index (current_row, 4)).toString ());
    UpdateModelList (ui_sale->brand_cb->currentText ());

    QObject::connect (ui_sale->model_cb, &QComboBox::currentTextChanged, this, &SaleDialog::ShowInfo);
    QObject::connect (ui_sale->brand_cb, &QComboBox::currentTextChanged, this, &SaleDialog::UpdateModelList);
    QObject::connect (ui_sale->size_cb, &QComboBox::currentTextChanged, this, &SaleDialog::UpdateCount);

    QObject::connect (ui_sale->sale_pb, &QPushButton::clicked, this, &SaleDialog::accept);
    QObject::connect (ui_sale->cancel_pb, &QPushButton::clicked, this, &SaleDialog::reject);
    ShowInfo(ui_sale->model_cb->currentText ());
}

QString SaleDialog::GetModel() {
    return ui_sale->model_cb->currentText ();
}

QString SaleDialog::GetSize() {
    return ui_sale->size_cb->currentText ();
}

double SaleDialog::GetPrice() {
    return ui_sale->price_sb->value ();
}

double SaleDialog::GetProfit() {
    return ui_sale->price_sb->value () - ui_sale->wholesale_price_sb->value ();
}

int SaleDialog::GetCount() {
    return ui_sale->count_sb->value ();
}

SaleDialog::~SaleDialog() {
    delete ui_sale;
}

void SaleDialog::ShowInfo(QString text) {
    QPixmap pic;
    pic.loadFromData (sdb->SelectPic (MODELS_TABLE, MODEL_NAME, text));
    ui_sale->pic_lbl->setPixmap (pic.scaledToWidth (300));
    if(pic.width () > pic.height ()) {
        ui_sale->pic_lbl->setAlignment (Qt::AlignTop);
    }
    else {
        ui_sale->pic_lbl->setAlignment (Qt::AlignCenter);
    }
    UpdatePrices ();
    UpdateSizes (ui_sale->model_cb->currentText ());
}

void SaleDialog::UpdateModelList(QString brand) {
    QSqlQueryModel* model = new QSqlQueryModel(this);
    model->setQuery ("SELECT DISTINCT " + MODEL_NAME +
                     " FROM " + AVAILABLE_GOODS_TABLE +
                     " WHERE " + BRAND + " = '" + brand + "'");
    ui_sale->model_cb->setModel (model);
    ui_sale->model_cb->setCurrentText(f_model->data (f_model->index (current_row, 1)).toString ());
}

void SaleDialog::UpdateSizes(QString model) {
    QString model_id = sdb->Select (MODEL_ID, MODELS_TABLE, MODEL_NAME, model);
    QSqlQueryModel* model_sizes = new QSqlQueryModel(this);
    model_sizes->setQuery ("SELECT DISTINCT " + GOODS_SIZE +
                           " FROM " + AVAILABLE_GOODS_TABLE +
                           " WHERE " + MODEL_ID + " ='" + model_id + "'" );
    ui_sale->size_cb->setModel (model_sizes);
}

void SaleDialog::UpdatePrices() {
    ui_sale->wholesale_price_sb->setValue (sdb->Select (WHOLESALE_PRICE, MODELS_TABLE, MODEL_NAME, ui_sale->model_cb->currentText ()).toDouble ());
    ui_sale->retail_price_sb->setValue (sdb->Select (RETAIL_PRICE, MODELS_TABLE, MODEL_NAME, ui_sale->model_cb->currentText ()).toDouble ());
    ui_sale->price_sb->setValue (ui_sale->retail_price_sb->value ());
}

void SaleDialog::UpdateCount() {
    int count = sdb->SelectCount (AVAILABLE_GOODS_TABLE, MODEL_NAME, GOODS_SIZE, "=", "=", ui_sale->model_cb->currentText (), ui_sale->size_cb->currentText ());
    ui_sale->count_sb->setValue (1);
    ui_sale->count_sb->setMaximum (count);
}
