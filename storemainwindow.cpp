#include "storemainwindow.h"
#include "ui_storemainwindow.h"

StoreMainWindow::StoreMainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::StoreMainWindow) {
    ui->setupUi(this);

    QWidget* wgt = new QWidget(this);
    setCentralWidget (wgt);

    t_bar_ = new QToolBar(this);
    t_view_ = new QTableView(this);
    txt_ = new QTextEdit(this);
    t_goods_info_ = new QTableView(this);
    h_main_layout_ = new QHBoxLayout(this);
    rv_layout_ = new QVBoxLayout(this);
    lv_layout_ = new QVBoxLayout(this);

    lv_layout_->addWidget (t_view_);
    rv_layout_->addWidget (txt_);
    rv_layout_->addWidget (t_goods_info_);

    BuildToolBar ();

    h_main_layout_->addLayout (lv_layout_);
    h_main_layout_->addLayout (rv_layout_);

    wgt->setLayout (h_main_layout_);
    setMinimumSize(600, 300);
}

void StoreMainWindow::BuildToolBar(){
    a_add_goods_ = t_bar_->addAction(QPixmap(":/pics/pics/add_goods.png"), "Прийняти товар", this, SLOT(onActionAddGoods()));
    a_sale_goods_ = t_bar_->addAction(QPixmap(":/pics/pics/sale_goods.png"), "Продати товар", this, SLOT(onActionSaleGoods()));
    t_bar_->addSeparator ();
    a_add_new_model_ = t_bar_->addAction(QPixmap(":/pics/pics/add_model.png"), "Додати нову модель", this, SLOT(onActionAddModel()));
    t_bar_->setMovable (false);
    t_bar_->setIconSize (QSize(SIZE_WID, SIZE_WID));
    addToolBar(Qt::TopToolBarArea, t_bar_);
}

void StoreMainWindow::onActionAddGoods() {

}
void StoreMainWindow::onActionSaleGoods() {

}
void StoreMainWindow::onActionAddModel() {

}

StoreMainWindow::~StoreMainWindow()
{
    delete ui;
}
