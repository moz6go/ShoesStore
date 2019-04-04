#include "storemainwindow.h"
#include "ui_storemainwindow.h"

StoreMainWindow::StoreMainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::StoreMainWindow) {
    ui->setupUi(this);
    QWidget* wgt = new QWidget(this);

    t_bar_ = new QToolBar(this);

    t_view_ = new QTableView(this);
    l_pic_ = new QLabel("pic",this);
    t_goods_info_ = new QTableView(this);

    h_main_layout_ = new QHBoxLayout();
    rv_layout_ = new QVBoxLayout();
    lv_layout_ = new QVBoxLayout();

    lv_layout_->addWidget (t_view_);
    rv_layout_->addWidget (l_pic_);
    rv_layout_->addWidget (t_goods_info_);

    BuildToolBar ();

    h_main_layout_->addLayout (lv_layout_);
    h_main_layout_->addLayout (rv_layout_);
    wgt->setLayout (h_main_layout_);
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
    a_report_ = t_bar_->addAction(QPixmap(":/pics/pics/report.png"), "Згенерувати звіт", this, SLOT(onActionReport()));

    t_bar_->setMovable (false);
    t_bar_->setIconSize (QSize(SIZE_WID, SIZE_WID));
    addToolBar(Qt::TopToolBarArea, t_bar_);
}

void StoreMainWindow::onActionAddGoods() {
    AddGoodsDialog* add_goods = new AddGoodsDialog(this);
    if(add_goods->exec () == QDialog::Accepted) {

    }
}

void StoreMainWindow::onActionSaleGoods() {

}

void StoreMainWindow::onActionAddModel() {
    AddModelDialog* add_model = new AddModelDialog(this);
    if(add_model->exec () == QDialog::Accepted) {
        qDebug() << add_model->getModel ();
        qDebug() << add_model->getBrand ();
        qDebug() << add_model->getSeason ();
        qDebug() << add_model->getRetailpr ();
        qDebug() << add_model->getWholesalepr ();
        qDebug() << add_model->getPhotoPath ();
    }
}

void StoreMainWindow::onActionDelModel() {

}

void StoreMainWindow::onActionReport() {

}

StoreMainWindow::~StoreMainWindow() {
    delete ui;
}
