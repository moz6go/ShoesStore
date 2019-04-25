#include "storemainwindow.h"
#include "ui_storemainwindow.h"

StoreMainWindow::StoreMainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::StoreMainWindow) {
    ui->setupUi(this);
    QWidget* wgt = new QWidget(this);

    sdb = new DataBase(this);
    isDbInit = InitDataBase();

    model = new QSqlTableModel(this);
    model->setTable (MODELS_TABLE);

    main_table_view = new QTableView(this);
    filter_model = new QSortFilterProxyModel(this);
    filter_model->setSourceModel (model);
    filter_model->setFilterCaseSensitivity (Qt::CaseInsensitive);
    filter_model->setFilterKeyColumn (BY_MODEL_NAME);
    main_table_view->setModel (filter_model);

    for(int col = 0; col < model->columnCount(); ++col) {
        model->setHeaderData(col, Qt::Horizontal, MAIN_TABLE_HEADERS_LIST[col]);
    }

    MainTableInit ();

    toolbar = new QToolBar(this);
    search_line = new QLineEdit(this);
    search_combo = new QComboBox(this);
    BuildToolBar ();

    pic_label = new QLabel(this);

    goods_info_table = new QTableWidget(this);
    TableInit(goods_info_table, QStringList() << "Розмір" << "Кількість");

    summary_table = new QTableWidget(this);
    TableInit(summary_table, QStringList() << "Показник" << "Значення");

    h_main_layout = new QHBoxLayout();
    rv_layout = new QVBoxLayout();
    lv_layout = new QVBoxLayout();

    lv_layout->addWidget (main_table_view);
    rv_layout->addWidget (pic_label);
    rv_layout->addWidget (summary_table);
    rv_layout->addWidget (goods_info_table);

    h_main_layout->addLayout (lv_layout);
    h_main_layout->addLayout (rv_layout);
    wgt->setLayout (h_main_layout);
    setCentralWidget (wgt);

    setMinimumSize(1050, 600);

    QObject::connect (main_table_view, &QTableView::clicked, this, &StoreMainWindow::ShowPic);
    QObject::connect (main_table_view, &QTableView::clicked, this, &StoreMainWindow::ShowGoodsInfo);
    QObject::connect (search_combo, &QComboBox::currentTextChanged, this, &StoreMainWindow::SetSearchType);
    QObject::connect (search_line, &QLineEdit::textChanged, this, &StoreMainWindow::SearchTextChanged);
    Update(0);


}

void StoreMainWindow::resizeEvent(QResizeEvent *event) {
    for(int i = 0; i < model->columnCount(); ++i) {
        main_table_view->setColumnWidth(i, this->width() / model->columnCount());
    }
    QMainWindow::resizeEvent(event);
}

void StoreMainWindow::MainTableInit() {
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
    main_table_view->setSortingEnabled (true);

}

void StoreMainWindow::TableInit(QTableWidget* table, QStringList headers) {
    table->clear ();
    table->setRowCount (0);
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels (headers);
    table->setFixedSize(300, 160);
    table->verticalHeader ()->setSectionResizeMode (QHeaderView::Fixed);
    table->verticalHeader ()->setDefaultSectionSize (20);
    table->verticalHeader ()->setVisible(false);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->resizeColumnsToContents();
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setStretchLastSection(true);
    table->setColumnWidth (0, 150);
    table->horizontalHeader ()->setStyleSheet ("QHeaderView { font-size: 10pt; }");
    table->setStyleSheet ("QTableWidget { font-size: 10pt; }");
}

void StoreMainWindow::SetSummary() {
    TableInit(summary_table, QStringList() << "Показник" << "Значення");
    for (int row = 0; row < SUMMARY_ROWS.size (); ++row) {
        summary_table->insertRow (row);
        summary_table->setItem(row, 0, new QTableWidgetItem (SUMMARY_ROWS[row]));
    }

    summary_table->setItem(1, 1, new QTableWidgetItem(QString::number (sdb->SelectCount (AVAILABLE_GOODS_TABLE))));
    summary_table->setItem(2, 1, new QTableWidgetItem(QString::number (sdb->SelectGoodsSum ())));
    summary_table->setItem(3, 1, new QTableWidgetItem(QString::number (sdb->SelectSum (SOLD_GOODS_TABLE, SALE_PRICE))));
}

void StoreMainWindow::BuildToolBar() {
    search_line->setFixedSize (150, SIZE_WID_2);
    search_line->setPlaceholderText ("Пошук...");

    search_combo->setMaximumHeight (SIZE_WID_2);
    for (int col = 1; col < 7; ++col){
        search_combo->addItem(MAIN_TABLE_HEADERS_LIST.at (col));
    }
    search_combo->setMaximumHeight (SIZE_WID_2);

    action_add_goods = toolbar->addAction(QPixmap(":/pics/add_goods.png"), "Прийняти товар", this, SLOT(onActionAddGoods()));
    action_sale_goods = toolbar->addAction(QPixmap(":/pics/sale_goods.png"), "Продати товар", this, SLOT(onActionSaleGoods()));
    toolbar->addSeparator ();
    action_add_new_model = toolbar->addAction(QPixmap(":/pics/add_model.png"), "Додати нову модель", this, SLOT(onActionAddModel()));
    action_del_model = toolbar->addAction(QPixmap(":/pics/delete.png"), "Видалити модель", this, SLOT(onActionDelModel()));
    toolbar->addSeparator ();
    action_report = toolbar->addAction(QPixmap(":/pics/report.png"), "Згенерувати звіт", this, SLOT(onActionReport()));
    toolbar->addSeparator ();
    action_update = toolbar->addAction(QPixmap(":/pics/update.png"), "Оновити", this, SLOT(onActionUpdate()));
    toolbar->addSeparator ();
    toolbar->addWidget (search_combo);
    toolbar->addWidget (search_line);

    toolbar->setMovable (false);
    toolbar->setIconSize (QSize(SIZE_WID_1, SIZE_WID_1));
    addToolBar(Qt::TopToolBarArea, toolbar);
}

void StoreMainWindow::SwitchButtons(State state) {
    switch (state) {
    case ENABLED_ALL:
        action_sale_goods->setEnabled (true);
        action_add_goods->setEnabled (true);
        action_del_model->setEnabled (true);
        action_add_new_model->setEnabled (true);
        action_report->setEnabled (true);
        action_update->setEnabled (true);
        break;
    case DISABLED_ALL:
        action_sale_goods->setDisabled (true);
        action_add_goods->setDisabled (true);
        action_del_model->setDisabled (true);
        action_add_new_model->setDisabled (true);
        action_report->setDisabled (true);
        action_update->setDisabled (true);
        break;
    case DATA_BASE_ISNT_INIT:
        action_sale_goods->setDisabled (true);
        action_add_goods->setDisabled (true);
        action_del_model->setDisabled (true);
        action_add_new_model->setDisabled (true);
        action_report->setDisabled (true);
        action_update->setDisabled (true);
        break;

    case MODEL_TABLE_EMPTY:
        action_sale_goods->setDisabled (true);
        action_add_goods->setDisabled (true);
        action_del_model->setDisabled (true);
        action_add_new_model->setEnabled (true);
        action_report->setDisabled (true);
        action_update->setEnabled (true);
        break;
    case GOODS_TABLE_EMPTY:
        action_sale_goods->setDisabled (true);
        action_add_goods->setEnabled (true);
        action_del_model->setEnabled (true);
        action_add_new_model->setEnabled (true);
        action_report->setEnabled (true);
        action_update->setEnabled (true);
        break;
    }
}

bool StoreMainWindow::InitDataBase() {
    if (sdb->ConnectToDataBase ()) {
        ui->statusBar->showMessage ("З'єднано з базою даних успішно!");
        return true;

    }
    else {
        QMessageBox::critical (this, "Error", "Неможливо з'єднатись з базою даних!", QMessageBox::Ok);
        return false;
    }
}


void StoreMainWindow::onActionAddGoods() {
    add_goods = new AddGoodsDialog(sdb, this);
    if (add_goods->exec () == QDialog::Accepted) {
        SwitchButtons(DISABLED_ALL);
        ui->statusBar->showMessage ("Зачекайте, додаю товар...");

        QList<QSpinBox*> sb_list = add_goods->GetSbList ();
        QString brand = add_goods->GetBrand ();
        QString model_name = add_goods->GetModelName ();
        int model_id = sdb->Select (MODEL_ID, MODELS_TABLE, MODEL_NAME, add_goods->GetModelName ()).toInt ();
        for (int i = 0; i < sb_list.size (); ++i) {
            if (sb_list[i]->value ()) {
                int size = i + 36;
                for (int count = 0; count < sb_list[i]->value (); ++count) {
                    QVariantList data = QVariantList() << model_id
                                                       << model_name
                                                       << brand
                                                       << size
                                                       << QDateTime::currentDateTime ().toString ("dd.MM.yyyy hh:mm:ss");
                    QStringList columns = { MODEL_ID, MODEL_NAME, BRAND, GOODS_SIZE, GOODS_DATE };
                    if (!sdb->InsertDataIntoTable(sdb->GenerateInsertQuery (AVAILABLE_GOODS_TABLE, columns),
                                                  sdb->GenerateBindValues (columns),
                                                  data)) {
                        ui->statusBar->showMessage ("Невдалось додати товари! Проблема з підключеням до бази даних");
                    }
                }
            }
        }
        Update (main_table_view->currentIndex ().row ());
        SwitchButtons(ENABLED_ALL);
        ui->statusBar->showMessage ("Додано " + QString::number(add_goods->GetGoodsCount ()) + " одиниць моделі " + add_goods->GetModelName ());
    }
}

void StoreMainWindow::onActionSaleGoods() {
    sale_goods = new SaleDialog(sdb, this);
    if (sale_goods->exec () == QDialog::Accepted) {
        for (int count = 0; count < sale_goods->GetCount (); ++count) {
            QVariantList data = sdb->SelectRow ("*",
                                                AVAILABLE_GOODS_TABLE,
                                                MODEL_NAME,
                                                GOODS_SIZE,
                                                sale_goods->GetModel (),
                                                sale_goods->GetSize (),
                                                GOODS_COL_COUNT);
            data.append (sale_goods->GetPrice ());
            data.append (sale_goods->GetProfit ());
            data.append (QDateTime::currentDateTime ().toString ("dd.MM.yyyy hh:mm:ss"));

            QStringList columns = { MODEL_ID, MODEL_NAME, BRAND, GOODS_ID, GOODS_SIZE, GOODS_DATE, SALE_PRICE, PROFIT, SALE_DATE };
            if (!sdb->InsertDataIntoTable (sdb->GenerateInsertQuery (SOLD_GOODS_TABLE, columns),
                                           sdb->GenerateBindValues (columns),
                                           data)) {
                ui->statusBar->showMessage ("Невдалось виконати операцію! Проблема з підключеням до бази даних");
            }
            else {
                sdb->DeleteRow (AVAILABLE_GOODS_TABLE, GOODS_ID, data[3].toString ());
            }
        }
        Update (main_table_view->currentIndex ().row ());
        ui->statusBar->showMessage ("Продано модель " + sale_goods->GetModel () + " в кількості " + QString::number (sale_goods->GetCount ()) +
                                    " шт., розмір " + sale_goods->GetSize ());
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
                                           << add_model->getSeason ()
                                           << add_model->getCategory ()
                                           << add_model->getBrand ()
                                           << QString::number (add_model->getWholesalepr ())
                                           << QString::number (add_model->getRetailpr ())
                                           << pic_byte_arr
                                           << QDateTime::currentDateTime ().toString ("dd.MM.yyyy hh:mm:ss");
        QStringList columns = {MODEL_NAME, SEASON, CATEGORY, BRAND, WHOLESALE_PRICE, RETAIL_PRICE, PIC, DATE };
        if (!sdb->InsertDataIntoTable (sdb->GenerateInsertQuery (MODELS_TABLE, columns),
                                       sdb->GenerateBindValues (columns),
                                       data)) {
            ui->statusBar->showMessage ("Невдалось додати товари! Проблема з підключеням до бази даних");
        }
        Update(filter_model->rowCount ());
        ui->statusBar->showMessage ("Додано модель " + add_model->getModel () + ", виробник " + add_model->getBrand ());
    }
}

void StoreMainWindow::onActionDelModel() {
    if(sdb->SelectCount (AVAILABLE_GOODS_TABLE,
                         MODEL_ID,
                         filter_model->data (filter_model->index (main_table_view->currentIndex ().row (), 0)).toString ())) {
        QMessageBox::warning (this, "Помилка!", "Неможливо видалити модель, є наявні товари!");
    }
    else{
        QString model_name = filter_model->data (filter_model->index (main_table_view->currentIndex ().row (), 1)).toString ();
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

void StoreMainWindow::onActionUpdate() {
    isDbInit = !isDbInit ? InitDataBase() : true;
    Update(main_table_view->currentIndex ().row ());
}

void StoreMainWindow::SearchTextChanged(QString text) {
    filter_model->setFilterFixedString (text);
    Update(main_table_view->currentIndex ().row ());
}

void StoreMainWindow::SetSearchType(QString type) {
    if(type == MAIN_TABLE_HEADERS_LIST[1]) {
        filter_model->setFilterKeyColumn (BY_MODEL_NAME);
    }
    else if(type == MAIN_TABLE_HEADERS_LIST[2]){
        filter_model->setFilterKeyColumn (BY_SEASON);
    }
    else if(type == MAIN_TABLE_HEADERS_LIST[3]){
        filter_model->setFilterKeyColumn (BY_CATEGORY);
    }
    else if(type == MAIN_TABLE_HEADERS_LIST[4]){
        filter_model->setFilterKeyColumn (BY_BRAND);
    }
    else if(type == MAIN_TABLE_HEADERS_LIST[5]){
        filter_model->setFilterKeyColumn (BY_WPRICE);
    }
    else if(type == MAIN_TABLE_HEADERS_LIST[6]){
        filter_model->setFilterKeyColumn (BY_RPRICE);
    }
}

void StoreMainWindow::ShowPic() {
    QPixmap pic;
    pic.loadFromData (filter_model->data(filter_model->index (main_table_view->currentIndex ().row (), 7)).toByteArray ());
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
    if(filter_model->rowCount ()){
        TableInit(goods_info_table, QStringList() << "Розмір" << "Кількість");
        int sum_count(0);
        int model_id = filter_model->data (filter_model->index (main_table_view->currentIndex ().row (), 0)).toInt ();
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
}

void StoreMainWindow::Update(int row) {
    //qDebug() << row;
    model->select ();
    model->sort (0, Qt::AscendingOrder);
    main_table_view->selectRow (row);
    ShowPic ();
    SetSummary();
    ShowGoodsInfo ();

    if(!isDbInit){
        SwitchButtons (DATA_BASE_ISNT_INIT);
    }
    else if(!filter_model->rowCount ()){
        SwitchButtons (MODEL_TABLE_EMPTY);
    }
    else if (!sdb->SelectCount (AVAILABLE_GOODS_TABLE)){
        SwitchButtons (GOODS_TABLE_EMPTY);
    }
    else {
        SwitchButtons (ENABLED_ALL);
    }
}

StoreMainWindow::~StoreMainWindow() {
    delete ui;
}
