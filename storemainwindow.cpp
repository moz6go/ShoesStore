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
    model->select ();

    t_view_->setColumnHidden(0, true);
    t_view_->setColumnHidden(7, true);
    t_view_->setColumnHidden(8, true);

    t_view_->verticalHeader ()->setSectionResizeMode (QHeaderView::Fixed);
    t_view_->verticalHeader ()->setDefaultSectionSize (20);
    t_view_->verticalHeader()->setVisible(false);
    t_view_->setSelectionBehavior(QAbstractItemView::SelectRows);
    t_view_->setSelectionMode(QAbstractItemView::SingleSelection);
    t_view_->resizeColumnsToContents();
    t_view_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    t_view_->horizontalHeader()->setStretchLastSection(true);
    t_view_->horizontalHeader ()->resizeSections (QHeaderView::ResizeToContents);
    t_view_->horizontalHeader ()->setStyleSheet ("QHeaderView { font-size: 10pt; }");
    t_view_->setStyleSheet ("QTableView { font-size: 10pt; }");
    t_view_->setMinimumWidth (700);
    t_view_->horizontalHeader ()->resizeSections (QHeaderView::ResizeToContents);

    t_bar_ = new QToolBar(this);
    BuildToolBar ();

    l_pic_ = new QLabel(this);

    t_goods_info_ = new QTableView(this);
    t_goods_info_->setMaximumSize(300, 250);
    t_goods_info_->setMinimumSize(300, 250);

    h_main_layout_ = new QHBoxLayout();
    rv_layout_ = new QVBoxLayout();
    lv_layout_ = new QVBoxLayout();

    lv_layout_->addWidget (t_view_);
    rv_layout_->addWidget (l_pic_);
    rv_layout_->addWidget (t_goods_info_);

    h_main_layout_->addLayout (lv_layout_);
    h_main_layout_->addLayout (rv_layout_);
    wgt->setLayout (h_main_layout_);
    setCentralWidget (wgt);

    setMinimumSize(1050, 500);

    QObject::connect (t_view_, &QTableView::clicked, this, &StoreMainWindow::ShowPic);
}

void StoreMainWindow::resizeEvent(QResizeEvent *event) {
    for(int i = 0; i < model->columnCount(); ++i) {
        t_view_->setColumnWidth(i, this->width() / model->columnCount());
    }
    QMainWindow::resizeEvent(event);
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
        QList<QSpinBox*> sb_list = add_goods->GetSbList ();
        for (auto& sb : sb_list) {
            //qDebug() << sb->value ();
        }
    }
}

void StoreMainWindow::onActionSaleGoods() {

}

void StoreMainWindow::onActionAddModel() {
    AddModelDialog* add_model = new AddModelDialog (this);
    if(add_model->exec () == QDialog::Accepted) {
        QPixmap pic(add_model->getPhotoPath ());
        QByteArray bArr;
        QBuffer buff(&bArr);
        buff.open (QIODevice::WriteOnly);
        pic.save (&buff,"JPG");
        QSqlQuery my_query;
        my_query.prepare ("INSERT INTO model_dir (model_name, category, season, brand, wholesale_price, retail_price, pic, date)"
                          "VALUES (:model_name, :category, :season, :brand, :wholesale_price, :retail_price, :pic, :date);");
        my_query.bindValue (":model_name", add_model->getModel ());
        my_query.bindValue (":category", add_model->getCategory ());
        my_query.bindValue (":season", add_model->getSeason ());
        my_query.bindValue (":brand", add_model->getBrand ());
        my_query.bindValue (":wholesale_price", QString::number (add_model->getWholesalepr ()));
        my_query.bindValue (":retail_price", QString::number (add_model->getRetailpr ()));
        my_query.bindValue (":pic", bArr);
        my_query.bindValue (":date", QDateTime::currentDateTime ().toString ("dd.MM.yyyy hh:mm:ss"));

        if(!my_query.exec ()){
            qDebug() << my_query.lastError ().text ();
        }
        model->select ();
        ui->statusBar->showMessage ("Додано модель " + add_model->getModel () + ", виробник " + add_model->getBrand ());
    }
}

void StoreMainWindow::onActionDelModel() {

}

void StoreMainWindow::onActionReport() {

}

void StoreMainWindow::ShowPic() {
    QPixmap pic;
    pic.loadFromData (model->data(model->index (t_view_->currentIndex ().row (), 7)).toByteArray ());
    l_pic_->setPixmap (pic.scaledToWidth (300));
    if(pic.width () > pic.height ()){
        l_pic_->setAlignment (Qt::AlignTop);
    }
    else {
        l_pic_->setAlignment (Qt::AlignCenter);
    }
}

StoreMainWindow::~StoreMainWindow() {
    delete ui;
}
