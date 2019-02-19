#include "storemainwindow.h"
#include "ui_storemainwindow.h"

StoreMainWindow::StoreMainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::StoreMainWindow) {
    ui->setupUi(this);
    QWidget* wgt = new QWidget(this);

    t_bar_ = new QToolBar(this);

    t_view_ = new QTableView(this);
    //t_view_->setModel(new QFileSystemModel);
    txt_ = new QPushButton("pic",this);
    t_goods_info_ = new QTableView(this);

    main_l_ = new QGridLayout();

    main_l_->addWidget(t_view_,0,0,2,4);
    main_l_->addWidget(txt_,0,4,1,1);
    main_l_->addWidget(t_goods_info_,1,4,1,1);

//    h_main_layout_ = new QHBoxLayout();
//    rv_layout_ = new QVBoxLayout();
//    lv_layout_ = new QVBoxLayout();

//    lv_layout_->addWidget (t_view_);
//    rv_layout_->addWidget (txt_);
//    rv_layout_->addWidget (t_goods_info_);

    BuildToolBar ();

//    h_main_layout_->addLayout (lv_layout_);
//    h_main_layout_->addLayout (rv_layout_);
//    wgt->setLayout (h_main_layout_);
    wgt->setLayout(main_l_);
    setCentralWidget (wgt);

    setMinimumSize(600, 300);
}

void StoreMainWindow::BuildToolBar() {
    a_add_goods_ = t_bar_->addAction(QPixmap(":/pics/pics/add_goods.png"), "Прийняти товар", this, SLOT(onActionAddGoods()));
    a_sale_goods_ = t_bar_->addAction(QPixmap(":/pics/pics/sale_goods.png"), "Продати товар", this, SLOT(onActionSaleGoods()));
    t_bar_->addSeparator ();
    a_add_new_model_ = t_bar_->addAction(QPixmap(":/pics/pics/add_model.png"), "Додати нову модель", this, SLOT(onActionAddModel()));
    a_del_model_ = t_bar_->addAction(QPixmap(":/pics/pics/delete.png"), "Видалити модель", this, SLOT(onActionDelModel()));
    t_bar_->addSeparator ();
    a_report_ = t_bar_->addAction(QPixmap(":/pics/pics/report.png"), "Додати нову модель", this, SLOT(onActionReport()));

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

void StoreMainWindow::onActionDelModel() {

}

void StoreMainWindow::onActionReport() {

}

StoreMainWindow::~StoreMainWindow() {
    delete ui;
}
