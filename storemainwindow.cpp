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
        QMessageBox::critical (this, "Error", "Неможливо з'єднатись з базою даних!", QMessageBox::Ok);
    }

    model = new QSqlTableModel(this);
    model->setTable (MODELS_TABLE);

    for(int col = 0; col < model->columnCount(); ++col) {
        model->setHeaderData(col, Qt::Horizontal, MAIN_TABLE_HEADERS_LIST[col]);
    }
    model->setSort (0, Qt::AscendingOrder);

    main_table_view = new QTableView(this);
    MainTableInit ();

    toolbar = new QToolBar(this);
    BuildToolBar ();

    pic_label = new QLabel(this);

    goods_info_table = new QTableWidget(this);
    GoodsInfoTableInit();

    h_main_layout = new QHBoxLayout();
    rv_layout = new QVBoxLayout();
    lv_layout = new QVBoxLayout();

    lv_layout->addWidget (main_table_view);
    rv_layout->addWidget (pic_label);
    rv_layout->addWidget (goods_info_table);

    h_main_layout->addLayout (lv_layout);
    h_main_layout->addLayout (rv_layout);
    wgt->setLayout (h_main_layout);
    setCentralWidget (wgt);
    SetState(DEFAULT);
    setMinimumSize(1050, 600);

    QObject::connect (main_table_view, &QTableView::clicked, this, &StoreMainWindow::ShowPic);
    QObject::connect (main_table_view, &QTableView::clicked, this, &StoreMainWindow::ShowGoodsInfo);
    Update(0);
}

void StoreMainWindow::resizeEvent(QResizeEvent *event) {
    for(int i = 0; i < model->columnCount(); ++i) {
        main_table_view->setColumnWidth(i, this->width() / model->columnCount());
    }
    QMainWindow::resizeEvent(event);
}

void StoreMainWindow::MainTableInit() {
    main_table_view->setModel (model);
    main_table_view->setColumnHidden(0, true);
    main_table_view->setColumnHidden(7, true);
    main_table_view->setColumnHidden(8, true);
    main_table_view->verticalHeader ()->setSectionResizeMode (QHeaderView::Fixed);
    main_table_view->verticalHeader ()->setDefaultSectionSize (20);
    main_table_view->verticalHeader()->setVisible(false);
    main_table_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    main_table_view->setSelectionMode(QAbstractItemView::SingleSelection);
    main_table_view->resizeColumnsToContents();
    main_table_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    main_table_view->horizontalHeader()->setStretchLastSection(true);
    main_table_view->horizontalHeader ()->resizeSections (QHeaderView::ResizeToContents);
    main_table_view->horizontalHeader ()->setStyleSheet ("QHeaderView { font-size: 10pt; }");
    main_table_view->setStyleSheet ("QTableView { font-size: 10pt; }");
    main_table_view->setMinimumWidth (700);
    main_table_view->horizontalHeader ()->resizeSections (QHeaderView::ResizeToContents);
}

void StoreMainWindow::GoodsInfoTableInit() {
    goods_info_table->clear ();
    goods_info_table->setRowCount (0);
    goods_info_table->setColumnCount(2);
    goods_info_table->setHorizontalHeaderLabels (QStringList() << "Розмір" << "Кількість");
    goods_info_table->setMaximumSize(300, 250);
    goods_info_table->setMinimumSize(300, 250);
    goods_info_table->verticalHeader ()->setSectionResizeMode (QHeaderView::Fixed);
    goods_info_table->verticalHeader ()->setDefaultSectionSize (20);
    goods_info_table->verticalHeader ()->setVisible(false);
    goods_info_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    goods_info_table->setSelectionMode(QAbstractItemView::SingleSelection);
    goods_info_table->resizeColumnsToContents();
    goods_info_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    goods_info_table->horizontalHeader()->setStretchLastSection(true);
    goods_info_table->setColumnWidth (0, 100);
    goods_info_table->horizontalHeader ()->setStyleSheet ("QHeaderView { font-size: 10pt; }");
    goods_info_table->setStyleSheet ("QTableWidget { font-size: 10pt; }");
}

void StoreMainWindow::BuildToolBar() {
    action_add_goods = toolbar->addAction(QPixmap(":/pics/pics/add_goods.png"), "Прийняти товар", this, SLOT(onActionAddGoods()));
    action_sale_goods = toolbar->addAction(QPixmap(":/pics/pics/sale_goods.png"), "Продати товар", this, SLOT(onActionSaleGoods()));
    toolbar->addSeparator ();
    action_add_new_model = toolbar->addAction(QPixmap(":/pics/pics/add_model.png"), "Додати нову модель", this, SLOT(onActionAddModel()));
    action_del_model = toolbar->addAction(QPixmap(":/pics/pics/delete.png"), "Видалити модель", this, SLOT(onActionDelModel()));
    toolbar->addSeparator ();
    action_report = toolbar->addAction(QPixmap(":/pics/pics/report.png"), "Згенерувати звіт", this, SLOT(onActionReport()));

    toolbar->setMovable (false);
    toolbar->setIconSize (QSize(SIZE_WID, SIZE_WID));
    addToolBar(Qt::TopToolBarArea, toolbar);
}

void StoreMainWindow::AddGoodsThread() {
}

void StoreMainWindow::SetState(State state) {
    switch (state) {
    case DEFAULT:
        action_sale_goods->setEnabled (true);
        action_add_goods->setEnabled (true);
        action_del_model->setEnabled (true);
        action_add_new_model->setEnabled (true);
        action_report->setEnabled (true);
        break;
    case ADD_GOODS:
        action_sale_goods->setDisabled (true);
        action_add_goods->setDisabled (true);
        action_del_model->setDisabled (true);
        action_add_new_model->setDisabled (true);
        action_report->setDisabled (true);
        break;
    case SALE_GOODS:
        action_sale_goods->setEnabled (true);
        action_add_goods->setEnabled (true);
        action_del_model->setEnabled (true);
        action_add_new_model->setEnabled (true);
        action_report->setEnabled (true);
        break;
    case ADD_MODEL:
        action_sale_goods->setEnabled (true);
        action_add_goods->setEnabled (true);
        action_del_model->setEnabled (true);
        action_add_new_model->setEnabled (true);
        action_report->setEnabled (true);
        break;
    case DELETE_MODEL:
        action_sale_goods->setEnabled (true);
        action_add_goods->setEnabled (true);
        action_del_model->setEnabled (true);
        action_add_new_model->setEnabled (true);
        action_report->setEnabled (true);
        break;
    case REPORT:
        action_sale_goods->setEnabled (true);
        action_add_goods->setEnabled (true);
        action_del_model->setEnabled (true);
        action_add_new_model->setEnabled (true);
        action_report->setEnabled (true);
    }
}


void StoreMainWindow::onActionAddGoods() {
    add_goods = new AddGoodsDialog(sdb, this);
    if (add_goods->exec () == QDialog::Accepted) {
        SetState(ADD_GOODS);
        ui->statusBar->showMessage ("Зачекайте, додаю товар...");

        QList<QSpinBox*> sb_list = add_goods->GetSbList ();
        QString brand = add_goods->GetBrand ();
        QString model_name = add_goods->GetModelName ();
        int model_id = sdb->SelectRow (MODEL_ID, MODELS_TABLE, MODEL_NAME, add_goods->GetModelName ()).toInt ();
        for (int i = 0; i < sb_list.size (); ++i) {
            if (sb_list[i]->value ()) {
                int size = i + 36;
                for (int count = 0; count < sb_list[i]->value (); ++count) {
                    QVariantList data = QVariantList() << model_id
                                                       << model_name
                                                       << brand
                                                       << size
                                                       << QDateTime::currentDateTime ().toString ("dd.MM.yyyy hh:mm:ss");

                    if (!sdb->InsertDataIntoTable(ADD_GOODS_QUERY, ADD_GOODS_BIND_VALUES, data)) {
                        ui->statusBar->showMessage ("Невдалось додати товари! Проблема з підключеням до бази даних");
                    }
                }
            }
        }
        Update (main_table_view->currentIndex ().row ());
        SetState(DEFAULT);
        ui->statusBar->showMessage ("Додано " + QString::number(add_goods->GetGoodsCount ()) + " одиниць моделі " + add_goods->GetModelName ());
    }
}

void StoreMainWindow::onActionSaleGoods() {
    sale_goods = new SaleDialog(sdb, this);
    if (sale_goods->exec () == QDialog::Accepted) {
        QVariantList goods_id = sdb->SelectRow ("*", AVAILABLE_GOODS_TABLE, MODEL_NAME, GOODS_SIZE, sale_goods->GetModel (), sale_goods->GetSize (), GOODS_COL_COUNT);
        QString model_id = sdb->SelectRow (MODEL_ID, MODELS_TABLE, MODEL_NAME, sale_goods->GetModel ());

        QVariantList data = {

        };
    }
}

void StoreMainWindow::onActionAddModel() {
    AddModelDialog* add_model = new AddModelDialog (sdb, this);
    if(add_model->exec () == QDialog::Accepted) {
        QPixmap pic(add_model->getPhotoPath ());
        QByteArray pic_byte_arr;
        QBuffer buff(&pic_byte_arr);
        buff.open (QIODevice::WriteOnly);
        pic.save (&buff, "JPG");

        QVariantList data = QVariantList() << add_model->getModel ()
                                           << add_model->getCategory ()
                                           << add_model->getSeason ()
                                           << add_model->getBrand ()
                                           << QString::number (add_model->getWholesalepr ())
                                           << QString::number (add_model->getRetailpr ())
                                           << pic_byte_arr
                                           << QDateTime::currentDateTime ().toString ("dd.MM.yyyy hh:mm:ss");

        if (!sdb->InsertDataIntoTable (ADD_MODEL_QUERY, ADD_MODEL_BIND_VALUES, data)) {
            ui->statusBar->showMessage ("Невдалось додати товари! Проблема з підключеням до бази даних");
        }
        Update(model->rowCount ());
        ui->statusBar->showMessage ("Додано модель " + add_model->getModel () + ", виробник " + add_model->getBrand ());
    }
}

void StoreMainWindow::onActionDelModel() {
    if(sdb->SelectCount (AVAILABLE_GOODS_TABLE,
                         MODEL_ID,
                         model->data (model->index (main_table_view->currentIndex ().row (), 0)).toString ())) {
        QMessageBox::warning (this, "Помилка!", "Неможливо видалити модель, є наявні товари!");
    }
    else{
        QString model_name = model->data (model->index (main_table_view->currentIndex ().row (), 1)).toString ();
        QMessageBox* msgbox = new QMessageBox(QMessageBox::Question,
                                              "Видалити модель",
                                              "Ви дійсно бажаєте видалити з бази даних модель " + model_name +"?",
                                              QMessageBox::No | QMessageBox::Yes,
                                              this);
        if(msgbox->exec () == QMessageBox::Yes) {
            if(sdb->DeleteRow (MODELS_TABLE, MODEL_NAME, model_name)){
                ui->statusBar->showMessage ("Видалено модель " + model_name);
            }
            Update(main_table_view->currentIndex ().row () - 1);
        }
    }
}

void StoreMainWindow::onActionReport() {

}

void StoreMainWindow::ShowPic() {
    QPixmap pic;
    pic.loadFromData (model->data(model->index (main_table_view->currentIndex ().row (), 7)).toByteArray ());
    if (!pic.isNull ()){
        pic_label->setPixmap (pic.scaledToWidth (300));
        if(pic.width () > pic.height ()){
            pic_label->setAlignment (Qt::AlignTop);
        }
        else {
            pic_label->setAlignment (Qt::AlignCenter);
        }
    }
}

void StoreMainWindow::ShowGoodsInfo() {
    GoodsInfoTableInit();
    int sum_count(0);
    int model_id = model->data (model->index (main_table_view->currentIndex ().row (), 0)).toInt ();
    for (int size = 36; size <= 46; ++size) {
        int count = sdb->SelectCount (
                AVAILABLE_GOODS_TABLE,
                MODEL_ID,
                GOODS_SIZE,
                QString::number (model_id),
                QString::number (size));
        sum_count += count;
        if(count) {
            goods_info_table->insertRow (goods_info_table->rowCount ());
            for(int col = 0; col < 2; ++col) {
                QTableWidgetItem* item = new QTableWidgetItem (col == 0 ? QString::number (size) : QString::number (count));
                goods_info_table->setItem(goods_info_table->rowCount() - 1, col, item);
            }
        }
    }
    goods_info_table->insertRow (goods_info_table->rowCount ());
    for(int col = 0; col < 2; ++col) {
        QTableWidgetItem* item = new QTableWidgetItem (col == 0 ? "ВСЬОГО" : QString::number (sum_count));
        goods_info_table->setItem(goods_info_table->rowCount() - 1, col, item);
    }
}

void StoreMainWindow::Update(int row) {
    model->select ();
    main_table_view->selectRow (row);
    ShowPic ();
    ShowGoodsInfo ();
}

StoreMainWindow::~StoreMainWindow() {
    delete ui;
}
