#include "returngoodsdialog.h"
#include "ui_returngoodsdialog.h"


ReturnGoodsDialog::ReturnGoodsDialog(DataBase* data_base, QWidget *parent) :
    QDialog(parent),
    ui_return(new Ui::ReturnGoodsDialog)
{
    ui_return->setupUi(this);
    setModal (true);
    sdb = data_base;

    query_model = new QSqlQueryModel();
    query_model->setQuery ("SELECT model_name, brand, goods_size, sale_price, sale_date, goods_id FROM sold_goods WHERE sale_date >= datetime('now', '-1 year')");
    query_model->setHeaderData (0, Qt::Horizontal, SOLD_GOODS_HEADERS_LIST[1]);
    query_model->setHeaderData (1, Qt::Horizontal, SOLD_GOODS_HEADERS_LIST[2]);
    query_model->setHeaderData (2, Qt::Horizontal, SOLD_GOODS_HEADERS_LIST[4]);
    query_model->setHeaderData (3, Qt::Horizontal, SOLD_GOODS_HEADERS_LIST[6]);
    query_model->setHeaderData (4, Qt::Horizontal, SOLD_GOODS_HEADERS_LIST[8]);
    query_model->setHeaderData (5, Qt::Horizontal, SOLD_GOODS_HEADERS_LIST[3]);

    ui_return->sold_goods_table->setModel (query_model);

    SoldGoodsTableInit();

    QObject::connect (ui_return->return_pb, &QPushButton::clicked, this, &ReturnGoodsDialog::accept);
    QObject::connect (ui_return->cancel_pb, &QPushButton::clicked, this, &ReturnGoodsDialog::reject);
}

QString ReturnGoodsDialog::GetGoodsId() {
    return query_model->data (query_model->index (ui_return->sold_goods_table->currentIndex ().row (), 5)).toString ();
}

QString ReturnGoodsDialog::GetModelName() {
    return query_model->data (query_model->index (ui_return->sold_goods_table->currentIndex ().row (), 0)).toString ();
}

QString ReturnGoodsDialog::GetBrand() {
    return query_model->data (query_model->index (ui_return->sold_goods_table->currentIndex ().row (), 1)).toString ();
}

QString ReturnGoodsDialog::GetSize() {
     return query_model->data (query_model->index (ui_return->sold_goods_table->currentIndex ().row (), 2)).toString ();
}

void ReturnGoodsDialog::SoldGoodsTableInit() {
    ui_return->sold_goods_table->verticalHeader ()->setSectionResizeMode (QHeaderView::Fixed);
    ui_return->sold_goods_table->verticalHeader ()->setDefaultSectionSize (18);
    ui_return->sold_goods_table->verticalHeader()->setVisible(false);
    ui_return->sold_goods_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui_return->sold_goods_table->setSelectionMode(QAbstractItemView::SingleSelection);
    ui_return->sold_goods_table->resizeColumnsToContents();
    ui_return->sold_goods_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui_return->sold_goods_table->horizontalHeader()->setStretchLastSection(true);
    ui_return->sold_goods_table->horizontalHeader ()->resizeSections (QHeaderView::ResizeToContents);
    ui_return->sold_goods_table->horizontalHeader ()->setStyleSheet ("QHeaderView { font-size: 9pt; }");
    ui_return->sold_goods_table->setStyleSheet ("QTableView { font-size: 9pt; }");
    ui_return->sold_goods_table->horizontalHeader ()->resizeSections (QHeaderView::ResizeToContents);
}

ReturnGoodsDialog::~ReturnGoodsDialog() {
    delete ui_return;
}
