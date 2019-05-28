#include "storemainwindow.h"
#include "ui_storemainwindow.h"

StoreMainWindow::StoreMainWindow(DataBase* data_base, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StoreMainWindow)
{
    ui->setupUi(this);

    sdb = data_base;

    brand_table_count = 0;
    season_table_count = 0;
    category_table_count = 0;
    models_table_count = 0;
    available_goods_table_count = 0;
    sold_goods_table_count = 0;
    sold_goods_by_last_year_count = 0;
    available_goods_curr_model_count = 0;

    sql_model = new QSqlTableModel(this);
    sql_model->setTable (MODELS_TABLE);

    main_table_view = new QTableView(this);
    filter_model = new QSortFilterProxyModel(this);
    filter_model->setSourceModel (sql_model);
    filter_model->setFilterCaseSensitivity (Qt::CaseInsensitive);
    filter_model->setFilterKeyColumn (BY_MODEL_NAME);
    main_table_view->setModel (filter_model);

    for(int col = 0; col < sql_model->columnCount(); ++col) {
        sql_model->setHeaderData(col, Qt::Horizontal, MODELS_TABLE_HEADERS_LIST[col]);
    }

    MainTableInit ();

    toolbar = new QToolBar(this);
    search_line = new QLineEdit(this);
    search_combo = new QComboBox(this);
    BuildToolBar ();

    pic_label = new QLabel(this);
    //pic_label->setFixedSize (300, 190);

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

    QWidget* wgt = new QWidget(this);
    wgt->setLayout (h_main_layout);
    setCentralWidget (wgt);

    setGeometry (200,100,1000,580);

    QObject::connect (main_table_view, &QTableView::clicked, this, &StoreMainWindow::ShowPic);
    QObject::connect (main_table_view, &QTableView::clicked, this, &StoreMainWindow::ShowGoodsInfo);
    QObject::connect (main_table_view, &QTableView::clicked, this, &StoreMainWindow::UpdateButtons);

    QObject::connect (search_combo, &QComboBox::currentTextChanged, this, &StoreMainWindow::SetSearchType);
    QObject::connect (search_line, &QLineEdit::textChanged, this, &StoreMainWindow::SearchTextChanged);
    Update(0);
}

void StoreMainWindow::resizeEvent(QResizeEvent *event) {
    for(int i = 0; i < sql_model->columnCount(); ++i) {
        main_table_view->setColumnWidth(i, this->width() / sql_model->columnCount());
    }
    QMainWindow::resizeEvent(event);
}

void StoreMainWindow::MainTableInit() {
    main_table_view->setColumnHidden(0, true);
    main_table_view->setColumnHidden(7, true);
    main_table_view->setColumnHidden(8, true);
    main_table_view->verticalHeader ()->setSectionResizeMode (QHeaderView::Fixed);
    main_table_view->verticalHeader ()->setDefaultSectionSize (18);
    main_table_view->verticalHeader()->setVisible(false);
    main_table_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    main_table_view->setSelectionMode(QAbstractItemView::SingleSelection);
    main_table_view->resizeColumnsToContents();
    main_table_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    main_table_view->horizontalHeader()->setStretchLastSection(true);
    main_table_view->horizontalHeader ()->resizeSections (QHeaderView::ResizeToContents);
    main_table_view->horizontalHeader ()->setStyleSheet ("QHeaderView { font-size: 9pt; }");
    main_table_view->setStyleSheet ("QTableView { font-size: 9pt; }");
    main_table_view->setMinimumWidth (660);
    main_table_view->horizontalHeader ()->resizeSections (QHeaderView::ResizeToContents);
    main_table_view->setSortingEnabled (true);
}

void StoreMainWindow::TableInit(QTableWidget* table, QStringList headers) {
    table->clear ();
    table->setRowCount (0);
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels (headers);
    table->setFixedSize(300, 150);
    table->verticalHeader ()->setSectionResizeMode (QHeaderView::Fixed);
    table->verticalHeader ()->setDefaultSectionSize (18);
    table->verticalHeader ()->setVisible(false);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->resizeColumnsToContents();
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setStretchLastSection(true);
    table->setColumnWidth (0, 150);
    table->horizontalHeader ()->setStyleSheet ("QHeaderView { font-size: 9pt; }");
    table->setStyleSheet ("QTableWidget { font-size: 9pt; }");
}

void StoreMainWindow::SetSummary() {
    TableInit(summary_table, QStringList() << "Показник" << "Значення");
    for (int row = 0; row < SUMMARY_ROWS.size (); ++row) {
        summary_table->insertRow (row);
        summary_table->setItem(row, 0, new QTableWidgetItem (SUMMARY_ROWS[row]));
    }
    double month_income(sdb->SelectSum (INCOME_BY_MONTH_QUERY)),
            month_costs(sdb->SelectSum (COSTS_BY_MONTH_QUERY)),
            year_income(sdb->SelectSum (INCOME_BY_YEAR_QUERY)),
            year_costs(sdb->SelectSum (COSTS_BY_YEAR_QUERY));

    summary_table->setItem(1, 1, new QTableWidgetItem(QString::number (sdb->SelectCount (AVAILABLE_GOODS_TABLE))));
    summary_table->setItem(2, 1, new QTableWidgetItem(QString("%L1").arg(sdb->SelectSum (AVAILABLE_GOODS_WPRICE_SUM_QUERY), 0, 'f', 2)));
    summary_table->setItem(4, 1, new QTableWidgetItem(QString("%L1").arg(month_income, 0, 'f', 2)));
    summary_table->setItem(5, 1, new QTableWidgetItem(QString("%L1").arg(month_costs, 0, 'f', 2)));
    summary_table->setItem(6, 1, new QTableWidgetItem(QString("%L1").arg((month_income - month_costs), 0, 'f', 2)));
    summary_table->setItem(8, 1, new QTableWidgetItem(QString("%L1").arg(year_income, 0, 'f', 2)));
    summary_table->setItem(9, 1, new QTableWidgetItem(QString("%L1").arg(year_costs, 0, 'f', 2)));
    summary_table->setItem(10, 1, new QTableWidgetItem(QString("%L1").arg((year_income - year_costs), 0, 'f', 2)));
}

void StoreMainWindow::BuildToolBar() {
    search_line->setFixedSize (150, SIZE_WID_2);
    search_line->setPlaceholderText ("Пошук...");

    search_combo->setMaximumHeight (SIZE_WID_2);
    for (int col = 1; col < 7; ++col){
        search_combo->addItem(MODELS_TABLE_HEADERS_LIST.at (col));
    }
    search_combo->setMaximumHeight (SIZE_WID_2);

    action_add_goods = toolbar->addAction(QPixmap(":/pics/add_goods.png"), "Прийняти товар", this, SLOT(onActionAddGoods()));
    toolbar->addSeparator ();
    action_sale_goods = toolbar->addAction(QPixmap(":/pics/sale_goods.png"), "Продати товар", this, SLOT(onActionSaleGoods()));
    action_return_goods = toolbar->addAction(QPixmap(":/pics/return_goods.png"), "Повернення товару", this, SLOT(onActionReturnGoods()));
    toolbar->addSeparator ();
    action_add_new_model = toolbar->addAction(QPixmap(":/pics/add_model.png"), "Додати нову модель", this, SLOT(onActionAddModel()));
    action_del_model = toolbar->addAction(QPixmap(":/pics/delete.png"), "Видалити модель", this, SLOT(onActionDelModel()));
    toolbar->addSeparator ();
    action_report = toolbar->addAction(QPixmap(":/pics/report.png"), "Згенерувати звіт", this, SLOT(onActionReport()));
    action_dictionary = toolbar->addAction (QPixmap(":/pics/dictionary.png"), "Змінити довідники", this, SLOT(onActionDictionary()));
    toolbar->addSeparator ();
    action_update = toolbar->addAction(QPixmap(":/pics/update.png"), "Оновити", this, SLOT(onActionUpdate()));
    toolbar->addSeparator ();
    action_res_copy = toolbar->addAction(QPixmap(":/pics/reserve_copy.png"), "Створити резервну копію бази даних", this, SLOT(onActionReserveCopy()));
    action_restore = toolbar->addAction(QPixmap(":/pics/restore.png"), "Відновити базу даних", this, SLOT(onActionRestore()));
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
        action_add_goods->setEnabled (true);
        action_sale_goods->setEnabled (true);
        action_return_goods->setEnabled (true);
        action_del_model->setEnabled (true);
        action_add_new_model->setEnabled (true);
        action_report->setEnabled (true);
        action_update->setEnabled (true);
        action_dictionary->setEnabled (true);
        action_res_copy->setEnabled (true);
        action_restore ->setEnabled (true);

        break;
    case DISABLED_ALL:
        action_add_goods->setDisabled (true);
        action_sale_goods->setDisabled (true);
        action_return_goods->setDisabled (true);
        action_del_model->setDisabled (true);
        action_add_new_model->setDisabled (true);
        action_report->setDisabled (true);
        action_update->setDisabled (true);
        action_dictionary->setDisabled (true);
        action_res_copy->setDisabled (true);
        action_restore ->setDisabled (true);
        break;
    }
}

void StoreMainWindow::CreateReportCSV(const QVector<QVariantList>& table, const QString& path) {
    QFile report_csv(path + "/report " + QDateTime::currentDateTime ().toString ("yyyy-MM-dd hh-mm-ss") + ".csv");
    if(report_csv.open(QIODevice::WriteOnly)){
        QTextStream fout(&report_csv);
#if defined(_WIN32)
        fout << "sep =,\n";
#endif
        for (auto& row : table) {
            for(const QVariant& cell : row ) {
                fout << "\"" + cell.toString () + "\",";
            }
            fout << '\n';
        }
        report_csv.close ();
        ui->statusBar->showMessage ("Звіт сформовано і збережено в папці " + path);
    }
}

void StoreMainWindow::onActionAddGoods() {
    AddGoodsDialog* add_goods = new AddGoodsDialog(sdb, main_table_view->currentIndex ().row (), filter_model, this);
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
                                                       << QDateTime::currentDateTime ().toString ("yyyy-MM-dd hh:mm:ss");
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
        ui->statusBar->showMessage ("Додано " + QString::number(add_goods->GetGoodsCount ()) + " одиниць моделі " + add_goods->GetModelName ());
    }
}

void StoreMainWindow::onActionSaleGoods() {
    SaleDialog* sale_goods = new SaleDialog(sdb, main_table_view->currentIndex ().row (), filter_model, this);
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
            data.append (QDateTime::currentDateTime ().toString ("yyyy-MM-dd hh:mm:ss"));

            QStringList columns = { MODEL_ID, MODEL_NAME, BRAND, GOODS_ID, GOODS_SIZE, GOODS_DATE, SALE_PRICE, PROFIT, SALE_DATE };
            if (!sdb->InsertDataIntoTable (sdb->GenerateInsertQuery (SOLD_GOODS_TABLE, columns),
                                           sdb->GenerateBindValues (columns),
                                           data))
            {
                ui->statusBar->showMessage ("Невдалось виконати операцію! Проблема з підключеням до бази даних");
                return;
            }
            else {
                sdb->DeleteRow (AVAILABLE_GOODS_TABLE, GOODS_ID, data[3].toString ());
                Update (main_table_view->currentIndex ().row ());
                ui->statusBar->showMessage ("Продано модель " + sale_goods->GetModel () + " в кількості " + QString::number (sale_goods->GetCount ()) +
                                            " шт., розмір " + sale_goods->GetSize ());
            }
        }
    }
}

void StoreMainWindow::onActionReturnGoods() {
    ReturnGoodsDialog* return_goods = new ReturnGoodsDialog(sdb, this);
    if(return_goods->exec () == QDialog::Accepted){
        QVariantList data = sdb->SelectRow ("*", SOLD_GOODS_TABLE, GOODS_ID, return_goods->GetGoodsId (), GOODS_COL_COUNT);
        QStringList columns = { MODEL_ID, MODEL_NAME, BRAND, GOODS_ID, GOODS_SIZE, GOODS_DATE };
        if (!sdb->InsertDataIntoTable (sdb->GenerateInsertQuery (AVAILABLE_GOODS_TABLE, columns),
                                       sdb->GenerateBindValues (columns),
                                       data))
        {
            ui->statusBar->showMessage ("Невдалось виконати операцію! Проблема з підключеням до бази даних");
            return;
        }
        else {
            sdb->DeleteRow (SOLD_GOODS_TABLE, GOODS_ID, return_goods->GetGoodsId ());
            Update (main_table_view->currentIndex ().row ());
            ui->statusBar->showMessage ("Повернено від покупця товар, модель " + return_goods->GetModelName () +
                                        " виробника " + return_goods->GetBrand () +
                                        ", розмір " + return_goods->GetSize ());
        }
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
                                           << QDateTime::currentDateTime ().toString ("yyyy-MM-dd hh:mm:ss");
        QStringList columns = { MODEL_NAME, SEASON, CATEGORY, BRAND, WHOLESALE_PRICE, RETAIL_PRICE, PIC, DATE };
        if (!sdb->InsertDataIntoTable (sdb->GenerateInsertQuery (MODELS_TABLE, columns),
                                       sdb->GenerateBindValues (columns),
                                       data)) {
            ui->statusBar->showMessage ("Невдалось додати товари! Проблема з підключеням до бази даних");
            return;
        }
        Update(filter_model->rowCount ());
        ui->statusBar->showMessage ("Додано модель " + add_model->getModel () + ", виробник " + add_model->getBrand ());
    }
}

void StoreMainWindow::onActionDelModel() {
    QString model_name = filter_model->data (filter_model->index (main_table_view->currentIndex ().row (), 1)).toString ();
    QMessageBox msgbox;
    msgbox.setIcon (QMessageBox::Question);
    msgbox.setWindowTitle ("Видалити модель");
    msgbox.setText ("Ви дійсно бажаєте видалити з бази даних модель " + model_name +"?");
    msgbox.addButton ("Видалити", QMessageBox::AcceptRole);
    msgbox.addButton ("Скасувати", QMessageBox::RejectRole);
    if(msgbox.exec () == QMessageBox::AcceptRole) {
        if(sdb->DeleteRow (MODELS_TABLE, MODEL_NAME, model_name)){
            ui->statusBar->showMessage ("Видалено модель " + model_name);
        }
        Update(main_table_view->currentIndex ().row () - 1);
    }
}

void StoreMainWindow::onActionReport() {
    ReportDialog* report_dialog = new ReportDialog(sdb, this);
    if(report_dialog->exec () == QDialog::Accepted) {
        QString path = QFileDialog::getExistingDirectory(this, tr("Зберегти звіт в ..."),
                                                        QDir::homePath (),
                                                        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if(!path.isEmpty ()) {
            QVector<QVariantList> table;
            QString table_name;
            switch (report_dialog->GetReportType ()) {
            case SOLD_GOODS_REPORT:
                table = sdb->SelectTable (SOLD_GOODS_TABLE, SALE_DATE, report_dialog->GetDateFrom (), report_dialog->GetDateTo ());
                table_name = SOLD_GOODS_TABLE;
                break;
            case AVAILABLE_GOODS_REPORT:
                table = sdb->SelectTable (AVAILABLE_GOODS_TABLE, GOODS_DATE, report_dialog->GetDateFrom (), report_dialog->GetDateTo ());
                table_name = AVAILABLE_GOODS_TABLE;
                break;
            }
            CreateReportCSV (table, path);
        }
    }
}

void StoreMainWindow::onActionUpdate() {
    Update(main_table_view->currentIndex ().row ());
}

void StoreMainWindow::onActionDictionary() {
    DictionaryDialog* dict_dialog = new DictionaryDialog(sdb, this);
    dict_dialog->exec ();
    Update(0);
}

void StoreMainWindow::onActionReserveCopy() {
    QString path = QFileDialog::getExistingDirectory(this, tr("Зберегти базу даних в ..."),
                                                    QDir::homePath (),
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(QFile::copy (DB_PATH, path + "/shoes_strore_db " + QDateTime::currentDateTime ().toString ("yyyy-MM-dd hh-mm-ss") + ".sqlite3")){
        ui->statusBar->showMessage ("Резервну копію бази даних збережено в папці " + path );
    }
    else {
        ui->statusBar->showMessage ("Не вдалось зробити копію бази даних!");
    }
}

void StoreMainWindow::onActionRestore() {
    QMessageBox msgbox;
    msgbox.setIcon (QMessageBox::Warning);
    msgbox.setWindowTitle ("Відновлення бази даних");
    msgbox.setText ("УВАГА!!! Після здійснення відновлення бази даних, поточні дані видаляться!!!\n\nРекомендуємо перед відновленням зберегти резервну копію поточної бази даних.");
    msgbox.addButton ("Відновити без збереження", QMessageBox::AcceptRole);
    msgbox.addButton ("Скасувати", QMessageBox::RejectRole);
    if(msgbox.exec () == QMessageBox::AcceptRole) {
        QString path = QFileDialog::getOpenFileName (this, "Виберіть файл бази даних", QDir::homePath (), "*.sqlite3");
        if (!path.isEmpty ()){
            sdb->CloseDataBase ();
            if (QFile::remove (DB_PATH)) {
                if(QFile::copy (path, DB_PATH)){
                    //sdb = new DataBase();
                    if (!sdb->ConnectToDataBase (DB_PATH)) {
                        QMessageBox::critical (this, "Error!", "Неможливо з'єднатись з базою даних!", QMessageBox::Ok);
                    }
                    Update(0);
                    ui->statusBar->showMessage ("Базу даних відновлено!");
                    QMessageBox::information (this, "Shoes Store", "Для коректної роботи після відновлення бази даних, рекомендується перезапутити програму", QMessageBox::Ok);
                }
                else {
                    QMessageBox::critical (this, "Error!", "Не вдалось відновити базу даних!", QMessageBox::Ok);
                }
            }
            else {
                QMessageBox::critical (this, "Error!", "Не вдалось відновити базу даних!", QMessageBox::Ok);
            }
        }
    }
}

void StoreMainWindow::SearchTextChanged(QString text) {
    filter_model->setFilterFixedString (text);
    Update(main_table_view->currentIndex ().row ());
}

void StoreMainWindow::SetSearchType(QString type) {
    if(type == MODELS_TABLE_HEADERS_LIST[1]) {
        filter_model->setFilterKeyColumn (BY_MODEL_NAME);
    }
    else if(type == MODELS_TABLE_HEADERS_LIST[2]){
        filter_model->setFilterKeyColumn (BY_SEASON);
    }
    else if(type == MODELS_TABLE_HEADERS_LIST[3]){
        filter_model->setFilterKeyColumn (BY_CATEGORY);
    }
    else if(type == MODELS_TABLE_HEADERS_LIST[4]){
        filter_model->setFilterKeyColumn (BY_BRAND);
    }
    else if(type == MODELS_TABLE_HEADERS_LIST[5]){
        filter_model->setFilterKeyColumn (BY_WPRICE);
    }
    else if(type == MODELS_TABLE_HEADERS_LIST[6]){
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
            int count = sdb->SelectCount (AVAILABLE_GOODS_TABLE,
                    MODEL_ID,                       GOODS_SIZE,
                    "=",                            "=",
                    QString::number (model_id),     QString::number (size));
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
    sql_model->select ();
    sql_model->sort (0, Qt::AscendingOrder);
    main_table_view->selectRow (row);
    ShowPic ();
    SetSummary ();
    ShowGoodsInfo ();
    UpdateButtons ();
}

void StoreMainWindow::UpdateButtons() {
    UpdateCounts ();
    action_add_goods->setEnabled (models_table_count);
    action_sale_goods->setEnabled (available_goods_table_count);
    action_return_goods->setEnabled (sold_goods_table_count);
    action_del_model->setEnabled (models_table_count && !sold_goods_by_last_year_count && !available_goods_curr_model_count);
    action_add_new_model->setEnabled (brand_table_count && season_table_count && category_table_count);
    action_report->setEnabled (available_goods_table_count || sold_goods_table_count);
    action_update->setEnabled (true);
    action_dictionary->setEnabled (true);
    action_restore->setEnabled (true);
    action_res_copy->setEnabled (QFile::exists (DB_PATH));
}

void StoreMainWindow::UpdateCounts() {
    QString model_id = filter_model->data (filter_model->index (main_table_view->currentIndex ().row (), 0)).toString ();
    brand_table_count = sdb->SelectCount(BRANDS_TABLE);
    season_table_count = sdb->SelectCount (SEASONS_TABLE);
    category_table_count = sdb->SelectCount (CATEGORIES_TABLE);
    models_table_count = sdb->SelectCount (MODELS_TABLE);
    available_goods_table_count = sdb->SelectCount (AVAILABLE_GOODS_TABLE);
    sold_goods_table_count = sdb->SelectCount(SOLD_GOODS_TABLE);
    sold_goods_by_last_year_count = sdb->SelectCount (SOLD_GOODS_TABLE,
                                                      MODEL_ID,     SALE_DATE,
                                                      "=",          ">=",
                                                      model_id,     "datetime('now', '-1 year')");
    available_goods_curr_model_count = sdb->SelectCount(AVAILABLE_GOODS_TABLE, MODEL_ID, "=", model_id);
}

StoreMainWindow::~StoreMainWindow() {
    delete ui;
}
