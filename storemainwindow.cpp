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
    model->setSort(0, Qt::AscendingOrder);

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

    t_goods_info_ = new QTableWidget(this);
    GoodsInfoTableInit();
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

    setMinimumSize(1050, 600);

    QObject::connect (t_view_, &QTableView::clicked, this, &StoreMainWindow::ShowPic);
    QObject::connect (t_view_, &QTableView::clicked, this, &StoreMainWindow::ShowGoodsInfo);
}

void StoreMainWindow::resizeEvent(QResizeEvent *event) {
    for(int i = 0; i < model->columnCount(); ++i) {
        t_view_->setColumnWidth(i, this->width() / model->columnCount());
    }
    QMainWindow::resizeEvent(event);
}

void StoreMainWindow::GoodsInfoTableInit() {
    t_goods_info_->clear ();
    t_goods_info_->setRowCount (0);
    t_goods_info_->setColumnCount(2);
    t_goods_info_->setHorizontalHeaderLabels (QStringList() << "Розмір" << "Кількість");
    t_goods_info_->setMaximumSize(300, 250);
    t_goods_info_->setMinimumSize(300, 250);
    t_goods_info_->verticalHeader ()->setSectionResizeMode (QHeaderView::Fixed);
    t_goods_info_->verticalHeader ()->setDefaultSectionSize (20);
    t_goods_info_->verticalHeader ()->setVisible(false);
    t_goods_info_->setSelectionBehavior(QAbstractItemView::SelectRows);
    t_goods_info_->setSelectionMode(QAbstractItemView::SingleSelection);
    t_goods_info_->resizeColumnsToContents();
    t_goods_info_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    t_goods_info_->horizontalHeader()->setStretchLastSection(true);
    t_goods_info_->setColumnWidth (0, 100);
    t_goods_info_->horizontalHeader ()->setStyleSheet ("QHeaderView { font-size: 10pt; }");
    t_goods_info_->setStyleSheet ("QTableWidget { font-size: 10pt; }");
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
        ui->statusBar->showMessage ("Зачекайте, додаю товар...");

        QList<QSpinBox*> sb_list = add_goods->GetSbList ();
        QSqlQuery sel_query;
        sel_query.exec ("SELECT model_id FROM model_dir WHERE model_name = '" + add_goods->GetModelName () + "'");
        QSqlRecord rec = sel_query.record ();
        sel_query.next ();
        int model_id =  sel_query.value(rec.indexOf("model_id")).toInt();
        for (int i = 0; i < sb_list.size (); ++i) {
            if (sb_list[i]->value ()) {
                int size = i + 36;
                for(int count = 0; count < sb_list[i]->value (); ++count) {
                    QSqlQuery query;
                    query.prepare ("INSERT INTO available_goods_dir (model_id, goods_size, goods_date)"
                                   "VALUES (:model_id, :goods_size, :goods_date);");
                    query.bindValue (":model_id", model_id);
                    query.bindValue (":goods_size", size);
                    query.bindValue (":goods_date", QDateTime::currentDateTime ().toString ("dd.MM.yyyy hh:mm:ss"));

                    if(!query.exec ()) {
                        ui->statusBar->showMessage ( query.lastError ().text ());
                    }
                }
            }
        }
        ui->statusBar->showMessage ("Додано " + QString::number(add_goods->GetGoodsCount ()) + " одиниць моделі " + add_goods->GetModelName ());
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
        QSqlQuery query;
        query.prepare ("INSERT INTO model_dir (model_name, category, season, brand, wholesale_price, retail_price, pic, date)"
                          "VALUES (:model_name, :category, :season, :brand, :wholesale_price, :retail_price, :pic, :date);");
        query.bindValue (":model_name", add_model->getModel ());
        query.bindValue (":category", add_model->getCategory ());
        query.bindValue (":season", add_model->getSeason ());
        query.bindValue (":brand", add_model->getBrand ());
        query.bindValue (":wholesale_price", QString::number (add_model->getWholesalepr ()));
        query.bindValue (":retail_price", QString::number (add_model->getRetailpr ()));
        query.bindValue (":pic", bArr);
        query.bindValue (":date", QDateTime::currentDateTime ().toString ("dd.MM.yyyy hh:mm:ss"));

        if(!query.exec ()){
            ui->statusBar->showMessage (query.lastError ().text ());
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

void StoreMainWindow::ShowGoodsInfo() {
    GoodsInfoTableInit();
    int sum_count(0);
    int model_id = model->data (model->index (t_view_->currentIndex ().row (), 0)).toInt ();
    for (int size = 36; size <= 46; ++size) {
        QSqlQuery sel_query;
        sel_query.exec ("SELECT COUNT(*) FROM available_goods_dir WHERE model_id = " + QString::number (model_id) + " AND goods_size = " + QString::number (size));
        QSqlRecord rec = sel_query.record ();
        sel_query.next ();
        int count = sel_query.value (0).toInt ();
        sum_count += count;
        if(count) {
            t_goods_info_->insertRow (t_goods_info_->rowCount ());
            for(int col = 0; col < 2; ++col) {
                QTableWidgetItem* item = new QTableWidgetItem (col == 0 ? QString::number (size) : QString::number (count));
                t_goods_info_->setItem(t_goods_info_->rowCount() - 1, col, item);
            }
        }
    }
    t_goods_info_->insertRow (t_goods_info_->rowCount ());
    for(int col = 0; col < 2; ++col) {
        QTableWidgetItem* item = new QTableWidgetItem (col == 0 ? "ВСЬОГО" : QString::number (sum_count));
        t_goods_info_->setItem(t_goods_info_->rowCount() - 1, col, item);
    }
}

StoreMainWindow::~StoreMainWindow() {
    delete ui;
}
