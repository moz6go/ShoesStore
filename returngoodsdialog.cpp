#include "returngoodsdialog.h"
#include "ui_returngoodsdialog.h"


ReturnGoodsDialog::ReturnGoodsDialog(DataBase* data_base, QWidget *parent) :
    QDialog(parent),
    ui_return(new Ui::ReturnGoodsDialog)
{
    ui_return->setupUi(this);
    setModal (true);
    sdb = data_base;

    model = new QSqlTableModel();
    model->setTable (SOLD_GOODS_TABLE);
    model->select ();
    ui_return->sold_goods_table->setModel (model);

    for(int col = 0; col < model->columnCount(); ++col) {
        model->setHeaderData(col, Qt::Horizontal, SOLD_GOODS_TABLE_HEADERS_LIST[col]);
    }

    SoldGoodsTableInit();

    QObject::connect (ui_return->return_pb, &QPushButton::clicked, this, &ReturnGoodsDialog::accept);
    QObject::connect (ui_return->cancel_pb, &QPushButton::clicked, this, &ReturnGoodsDialog::reject);
}

QString ReturnGoodsDialog::GetGoodsId() {
    return model->data (model->index (ui_return->sold_goods_table->currentIndex ().row (), 3)).toString ();
}

QString ReturnGoodsDialog::GetModelName() {
    return model->data (model->index (ui_return->sold_goods_table->currentIndex ().row (), 1)).toString ();
}

QString ReturnGoodsDialog::GetBrand() {
    return model->data (model->index (ui_return->sold_goods_table->currentIndex ().row (), 2)).toString ();
}

QString ReturnGoodsDialog::GetSize() {
     return model->data (model->index (ui_return->sold_goods_table->currentIndex ().row (), 4)).toString ();
}

void ReturnGoodsDialog::SoldGoodsTableInit() {
    ui_return->sold_goods_table->setColumnHidden(0, true);
    ui_return->sold_goods_table->setColumnHidden(3, true);
    ui_return->sold_goods_table->setColumnHidden(5, true);
    ui_return->sold_goods_table->setColumnHidden(7, true);
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
