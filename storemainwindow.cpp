#include "storemainwindow.h"
#include "ui_storemainwindow.h"

StoreMainWindow::StoreMainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::StoreMainWindow) {
    ui->setupUi(this);
    QWidget* wgt = new QWidget(this);

    sdb = new DataBase(this);
    if (sdb->ConnectToDataBase ()) {
        ui->statusBar->showMessage ("З'єднано з базою даних успішно!");
    }
    else {
        ui->statusBar->showMessage ("Неможливо з'єднатись з базою даних!");
    }

    model = new QSqlTableModel(this);
    model->setTable ("model_dir");

    for(int i = 0; i < model->columnCount(); ++i) {
        model->setHeaderData(i, Qt::Horizontal, HEADERS_LIST[i]);
    }
    model->setSort(0,Qt::AscendingOrder);

    t_view_ = new QTableView(this);
    t_view_->setModel (model);
    t_view_->setColumnHidden(0, true);
    t_view_->setColumnHidden(7, true);
    t_view_->verticalHeader ()->setSectionResizeMode (QHeaderView::Fixed);
    t_view_->verticalHeader ()->setDefaultSectionSize (20);
    t_view_->verticalHeader()->setVisible(false);
    //t_view_->setShowGrid(false);
    t_view_->setSelectionBehavior(QAbstractItemView::SelectRows);
    t_view_->setSelectionMode(QAbstractItemView::SingleSelection);
    t_view_->resizeColumnsToContents();
    t_view_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    t_view_->horizontalHeader()->setStretchLastSection(true);
    t_view_->horizontalHeader ()->setStyleSheet ("QHeaderView { font-size: 10pt; }");
    t_view_->setStyleSheet ("QTableView { font-size: 10pt; }");
    model->select ();

    t_bar_ = new QToolBar(this);

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
    AddModelDialog* add_model = new AddModelDialog (this);
    if(add_model->exec () == QDialog::Accepted) {
        QSqlQuery my_query;
        QString ins = QString("INSERT INTO model_dir (model_name, category, season, brand, pic, wholesale_price, retail_price)"
                              "VALUES ('%1', '%2', '%3', '%4', '%5', '%6', '%7');")
                .arg(add_model->getModel ())
                .arg(add_model->getCategory ())
                .arg(add_model->getSeason ())
                .arg(add_model->getBrand ())
                .arg(add_model->getPhotoPath ())
                .arg(add_model->getWholesalepr ())
                .arg(add_model->getRetailpr ());

        if(!my_query.exec (ins)){
            qDebug() << my_query.lastError ().text ();
        }
        model->select ();
        ui->statusBar->showMessage ("Додано модель " + add_model->getModel ());
    }
}

void StoreMainWindow::onActionDelModel() {

}

void StoreMainWindow::onActionReport() {

}

StoreMainWindow::~StoreMainWindow() {
    delete ui;
}
