#include "returngoodsdialog.h"
#include "ui_returngoodsdialog.h"


ReturnGoodsDialog::ReturnGoodsDialog(DataBase* data_base, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReturnGoodsDialog)
{
    ui->setupUi(this);
    setModal (true);
    sdb = data_base;

    query_model = new QSqlQueryModel();
    query_model->setQuery (SqlQueries::SoldGoodsForReturn ());
    query_model->setHeaderData (0, Qt::Horizontal, SOLD_GOODS_HEADERS_LIST[S_MODEL_NAME_COL]);
    query_model->setHeaderData (1, Qt::Horizontal, SOLD_GOODS_HEADERS_LIST[S_BRAND_COL]);
    query_model->setHeaderData (2, Qt::Horizontal, SOLD_GOODS_HEADERS_LIST[S_GOODS_SIZE_COL]);
    query_model->setHeaderData (3, Qt::Horizontal, SOLD_GOODS_HEADERS_LIST[SALE_PRICE_COL]);
    query_model->setHeaderData (4, Qt::Horizontal, SOLD_GOODS_HEADERS_LIST[SALE_DATE_COL]);
    query_model->setHeaderData (5, Qt::Horizontal, SOLD_GOODS_HEADERS_LIST[S_GOODS_ID_COL]);

    ui->sold_goods_table->setModel (query_model);

    SoldGoodsTableInit();

    QObject::connect (ui->sold_goods_table, &QTableView::clicked, this, &ReturnGoodsDialog::EnableReturnButton);
    QObject::connect (ui->return_pb, &QPushButton::clicked, this, &ReturnGoodsDialog::accept);
    QObject::connect (ui->cancel_pb, &QPushButton::clicked, this, &ReturnGoodsDialog::reject);
}

QString ReturnGoodsDialog::GetGoodsId() {
    return query_model->data (query_model->index (ui->sold_goods_table->currentIndex ().row (), 5)).toString ();
}

QString ReturnGoodsDialog::GetModelName() {
    return query_model->data (query_model->index (ui->sold_goods_table->currentIndex ().row (), 0)).toString ();
}

QString ReturnGoodsDialog::GetBrand() {
    return query_model->data (query_model->index (ui->sold_goods_table->currentIndex ().row (), 1)).toString ();
}

QString ReturnGoodsDialog::GetSize() {
     return query_model->data (query_model->index (ui->sold_goods_table->currentIndex ().row (), 2)).toString ();
}

void ReturnGoodsDialog::SoldGoodsTableInit() {
    ui->sold_goods_table->verticalHeader ()->setSectionResizeMode (QHeaderView::Fixed);
    ui->sold_goods_table->verticalHeader ()->setDefaultSectionSize (18);
    ui->sold_goods_table->verticalHeader()->setVisible(false);
    ui->sold_goods_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->sold_goods_table->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->sold_goods_table->resizeColumnsToContents();
    ui->sold_goods_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->sold_goods_table->horizontalHeader()->setStretchLastSection(true);
    ui->sold_goods_table->horizontalHeader ()->resizeSections (QHeaderView::ResizeToContents);
}

void ReturnGoodsDialog::EnableReturnButton() {
    ui->return_pb->setEnabled (true);
}

ReturnGoodsDialog::~ReturnGoodsDialog() {
    delete ui;
}
