#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(DataBase *data_base, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    sdb = data_base;

    brand_table_count = 0;
    season_table_count = 0;
    category_table_count = 0;
    models_table_count = 0;
    available_goods_table_count = 0;
    available_goods_by_model_count = 0;
    sold_goods_table_count = 0;
    sold_goods_by_last_year_count = 0;
    sold_goods_by_model_count = 0;

    main_tbl_sql_model = new QSqlQueryModel(this);
    main_tbl_sql_model->setQuery (SqlQueries::GoodsCountMainTable ());

    filter_model = new MyProxyModel(this);
    filter_model->setSourceModel (main_tbl_sql_model);
    filter_model->setFilterCaseSensitivity (Qt::CaseInsensitive);
    filter_model->setFilterKeyColumn (MODEL_NAME_COL);
    filter_model->sort (DATE_COL, Qt::DescendingOrder);

    ui->main_table_view->setModel (filter_model);

    for(int col = 0; col < main_tbl_sql_model->columnCount(); ++col) {
        main_tbl_sql_model->setHeaderData(col, Qt::Horizontal, MODELS_TABLE_HEADERS_LIST[col]);
    }

    toolbar = new QToolBar(this);
    search_line = new QLineEdit(this);
    search_combo = new QComboBox(this);
    BuildToolBar ();

    TableInit(ui->goods_info_table, QStringList() << "Розмір" << "Кількість");
    TableInit(ui->summary_table, QStringList() << "Показник" << "Значення");
    MainTableInit (ui->main_table_view);

    QObject::connect (ui->main_table_view, &QTableView::clicked, [=]{Update(ui->main_table_view->currentIndex().row());});
    QObject::connect (search_combo, &QComboBox::currentTextChanged, this, &MainWindow::SetSearchType);
    QObject::connect (search_line, &QLineEdit::textChanged, this, &MainWindow::SearchTextChanged);
    Update(0);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    for(int i = 0; i < main_tbl_sql_model->columnCount(); ++i) {
        ui->main_table_view->setColumnWidth(i, this->width() / main_tbl_sql_model->columnCount());
    }
    QMainWindow::resizeEvent(event);
}

void MainWindow::MainTableInit(QTableView *table) {
    table->setColumnHidden(MODEL_ID_COL, true);
    table->setColumnHidden(PIC_COL, true);
    table->setColumnHidden(DATE_COL, true);
    table->setColumnHidden(ARCHIVE_COL, true);
    table->verticalHeader ()->setSectionResizeMode (QHeaderView::Fixed);
    table->verticalHeader ()->setDefaultSectionSize (18);
    table->verticalHeader()->setVisible(false);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->resizeColumnsToContents();
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setStretchLastSection(true);
    table->horizontalHeader ()->resizeSections (QHeaderView::ResizeToContents);
    table->setSortingEnabled (true);
}

void MainWindow::TableInit(QTableWidget* table, QStringList headers) {
    table->clear ();
    table->setRowCount (0);
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels (headers);
    table->verticalHeader ()->setSectionResizeMode (QHeaderView::Fixed);
    table->verticalHeader ()->setDefaultSectionSize (18);
    table->verticalHeader ()->setVisible(false);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setStretchLastSection(true);
    table->setColumnWidth (0, 150);
}

void MainWindow::SetSummary() {
    TableInit(ui->summary_table, QStringList() << "Показник" << "Значення");
    for (int row = 0; row < SUMMARY_ROWS.size (); ++row) {
        ui->summary_table->insertRow (row);
        ui->summary_table->setItem(row, 0, new QTableWidgetItem (SUMMARY_ROWS[row]));
    }
    double month_income(sdb->SelectSum (SqlQueries::IncomeByMonth ())),
            month_costs(sdb->SelectSum (SqlQueries::CostsByMonth ())),
            year_income(sdb->SelectSum (SqlQueries::IncomeByYear ())),
            year_costs(sdb->SelectSum (SqlQueries::CostsByYear ()));

    ui->summary_table->setItem(1, 1, new QTableWidgetItem(QString::number (sdb->SelectCount (AVAILABLE_GOODS_TABLE))));
    ui->summary_table->setItem(2, 1, new QTableWidgetItem(QString("%L1").arg(sdb->SelectSum (SqlQueries::AvailableGoodsWpriceSum ()), 0, 'f', 2)));
    ui->summary_table->setItem(4, 1, new QTableWidgetItem(QString("%L1").arg(month_income, 0, 'f', 2)));
    ui->summary_table->setItem(5, 1, new QTableWidgetItem(QString("%L1").arg(month_costs, 0, 'f', 2)));
    ui->summary_table->setItem(6, 1, new QTableWidgetItem(QString("%L1").arg((month_income - month_costs), 0, 'f', 2)));
    ui->summary_table->setItem(8, 1, new QTableWidgetItem(QString("%L1").arg(year_income, 0, 'f', 2)));
    ui->summary_table->setItem(9, 1, new QTableWidgetItem(QString("%L1").arg(year_costs, 0, 'f', 2)));
    ui->summary_table->setItem(10, 1, new QTableWidgetItem(QString("%L1").arg((year_income - year_costs), 0, 'f', 2)));
}

void MainWindow::BuildToolBar() {
    search_line->setFixedSize (150, SIZE_WID_2);
    search_line->setPlaceholderText ("Пошук...");
    search_line->setClearButtonEnabled (true);

    search_combo->setMaximumHeight (SIZE_WID_2);
    for (int col = MODEL_NAME_COL; col <= BRAND_COL; ++col) {
        search_combo->addItem(MODELS_TABLE_HEADERS_LIST.at (col));
    }
    search_combo->setMaximumHeight (SIZE_WID_2);

    action_add_goods = toolbar->addAction(QPixmap(":/pics/add_goods.png"), "Прийняти товар", this, SLOT(onActionAddGoods()));
    toolbar->addSeparator ();
    action_sale_goods = toolbar->addAction(QPixmap(":/pics/sale_goods.png"), "Продати товар", this, SLOT(onActionSaleGoods()));
    action_return_goods = toolbar->addAction(QPixmap(":/pics/return_goods.png"), "Повернення товару", this, SLOT(onActionReturnGoods()));
    toolbar->addSeparator ();
    action_add_new_model = toolbar->addAction(QPixmap(":/pics/add_model.png"), "Додати нову модель", this, SLOT(onActionAddModel()));
    action_edit_model = toolbar->addAction (QPixmap(":/pics/edit_model.png"), "Редагувати дані моделі", this, SLOT(onActionEditModel ()));
    action_del_model = toolbar->addAction(QPixmap(":/pics/delete.png"), "Видалити модель", this, SLOT(onActionDelModel()));
    toolbar->addSeparator ();
    action_add_to_archive = toolbar->addAction(QPixmap(":/pics/add_to_archive.png"), "Перемістити модель в архів", this, SLOT(onActionAddToArchive()));
    action_view_archive = toolbar->addAction(QPixmap(":/pics/view_archive.png"), "Переглянути архів", this, SLOT(onActionViewArchive()));
    toolbar->addSeparator ();
    action_report = toolbar->addAction(QPixmap(":/pics/report.png"), "Згенерувати звіт", this, SLOT(onActionReport()));
    action_dictionary = toolbar->addAction (QPixmap(":/pics/dictionary.png"), "Змінити довідники", this, SLOT(onActionDictionary()));
    toolbar->addSeparator ();
    action_update = toolbar->addAction(QPixmap(":/pics/update.png"), "Оновити", this, SLOT(onActionUpdate()));
    toolbar->addSeparator ();
    action_copy_restore = toolbar->addAction(QPixmap(":/pics/db_copy_restore.png"), "Резервна копія/відновлення бази даних", this, SLOT(onActionCopyRestoreDb()));
    toolbar->addSeparator ();
    toolbar->addWidget (search_combo);
    toolbar->addWidget (search_line);

    toolbar->setMovable (false);
    toolbar->setIconSize (QSize(SIZE_WID_1, SIZE_WID_1));
    addToolBar(Qt::TopToolBarArea, toolbar);
}

void MainWindow::SwitchButtons(State state) {
    switch (state) {
    case ENABLED_ALL:
        action_add_goods->setEnabled (true);
        action_sale_goods->setEnabled (true);
        action_return_goods->setEnabled (true);
        action_add_new_model->setEnabled (true);
        action_edit_model->setEnabled (true);
        action_del_model->setEnabled (true);
        action_report->setEnabled (true);
        action_update->setEnabled (true);
        action_dictionary->setEnabled (true);
        action_copy_restore->setEnabled (true);

        break;
    case DISABLED_ALL:
        action_add_goods->setDisabled (true);
        action_sale_goods->setDisabled (true);
        action_return_goods->setDisabled (true);
        action_add_new_model->setDisabled (true);
        action_edit_model->setDisabled (true);
        action_del_model->setDisabled (true);
        action_report->setDisabled (true);
        action_update->setDisabled (true);
        action_dictionary->setDisabled (true);
        action_copy_restore->setDisabled (true);
        break;
    }
}

void MainWindow::CreateReportCSV(const QVector<QVariantList>& table, const QString& path) {
    QFile report_csv(path + "/report " + QDateTime::currentDateTime ().toString (FS_DATE_TIME_FORMAT) + ".csv");
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
        ui->statusbar->showMessage ("Звіт сформовано і збережено в папці " + path);
    }
}

void MainWindow::onActionAddGoods() {
    QVariantList row = sdb->SelectRow ("*", MODELS_TABLE, MODEL_ID, filter_model->data (filter_model->index (ui->main_table_view->currentIndex ().row (), MODEL_ID_COL)).toString (), filter_model->columnCount ());
    AddGoodsDialog* add_goods = new AddGoodsDialog(sdb, row, this);
    if (add_goods->exec () == QDialog::Accepted) {
        SwitchButtons(DISABLED_ALL);
        ui->statusbar->showMessage ("Зачекайте, додаю товар...");

        QList<QSpinBox*> sb_list = add_goods->GetSbList ();
        QString brand = add_goods->GetBrand ();
        QString model_name = add_goods->GetModelName ();
        int model_id = sdb->Select (MODEL_ID, MODELS_TABLE, MODEL_NAME, add_goods->GetModelName ()).toInt ();
        for (int i = 0; i < sb_list.size (); ++i) {
            if (sb_list[i]->value ()) {
                QString size = i == WITHOUT_SIZE ? "б.р." : QString::number (i + 36);
                for (int count = 0; count < sb_list[i]->value (); ++count) {
                    QVariantList data = { model_id, model_name, brand, size, QDateTime::currentDateTime ().toString (SQL_DATE_TIME_FORMAT) };
                    QStringList columns = { MODEL_ID, MODEL_NAME, BRAND, GOODS_SIZE, GOODS_DATE };
                    if (!sdb->UpdateInsertData(sdb->GenerateInsertQuery (AVAILABLE_GOODS_TABLE, columns),
                                                  sdb->GenerateBindValues (columns),
                                                  data)) {
                        QMessageBox::critical (this, "Error","Невдалось додати товари! Проблема з підключеням до бази даних!\n\nПомилка:\n" + sdb->LastError ());
                        ui->statusbar->showMessage ("Невдалось додати товари! Проблема з підключеням до бази даних");
                    }
                }
            }
        }
        Update (ui->main_table_view->currentIndex ().row ());
        ui->statusbar->showMessage ("Додано " + QString::number(add_goods->GetGoodsCount ()) + " одиниць моделі " + add_goods->GetModelName ());
    }
    else {
        ui->statusbar->clearMessage ();
    }
}

void MainWindow::onActionSaleGoods() {
    QVariantList row = sdb->SelectRow ("*", MODELS_TABLE, MODEL_ID, filter_model->data (filter_model->index (ui->main_table_view->currentIndex ().row (), MODEL_ID_COL)).toString (), filter_model->columnCount ());
    SaleDialog* sale_goods = new SaleDialog(sdb, row, this);
    if (sale_goods->exec () == QDialog::Accepted) {
        for (int count = 0; count < sale_goods->GetCount (); ++count) {
            QVariantList data = sdb->SelectRow ("*", AVAILABLE_GOODS_TABLE, MODEL_NAME, GOODS_SIZE, sale_goods->GetModel (), sale_goods->GetSize (), GOODS_COL_COUNT);
            data.append (sale_goods->GetPrice ());
            data.append (sale_goods->GetProfit ());
            data.append (QDateTime::currentDateTime ().toString (SQL_DATE_TIME_FORMAT));

            QStringList columns = { MODEL_ID, MODEL_NAME, BRAND, GOODS_ID, GOODS_SIZE, GOODS_DATE, SALE_PRICE, PROFIT, SALE_DATE };
            if (!sdb->UpdateInsertData (sdb->GenerateInsertQuery (SOLD_GOODS_TABLE, columns),
                                        sdb->GenerateBindValues (columns),
                                        data))
            {
                QMessageBox::critical (this, "Error!", "Невдалось виконати операцію! Проблема з підключеням до бази даних!\n\nПомилка:\n" + sdb->LastError ());
                ui->statusbar->showMessage ("Невдалось виконати операцію! Проблема з підключеням до бази даних");
                return;
            }
            else {
                sdb->DeleteRow (AVAILABLE_GOODS_TABLE, GOODS_ID, data.at(GOODS_ID_COL).toString ());
                Update (ui->main_table_view->currentIndex ().row ());
                ui->statusbar->showMessage ("Продано модель " + sale_goods->GetModel () + " в кількості " + QString::number (sale_goods->GetCount ()) +
                                            " шт., розмір " + sale_goods->GetSize ());
            }
        }
    }
    else {
        ui->statusbar->clearMessage ();
    }
}

void MainWindow::onActionReturnGoods() {
    ReturnGoodsDialog* return_goods = new ReturnGoodsDialog(sdb, this);
    if(return_goods->exec () == QDialog::Accepted){
        QVariantList data = sdb->SelectRow ("*", SOLD_GOODS_TABLE, GOODS_ID, return_goods->GetGoodsId (), GOODS_COL_COUNT);
        QStringList columns = { MODEL_ID, MODEL_NAME, BRAND, GOODS_ID, GOODS_SIZE, GOODS_DATE };
        if (!sdb->UpdateInsertData (sdb->GenerateInsertQuery (AVAILABLE_GOODS_TABLE, columns),
                                       sdb->GenerateBindValues (columns),
                                       data))
        {
            QMessageBox::critical (this, "Error!", "Невдалось виконати операцію! Проблема з підключеням до бази даних!\n\nПомилка:\n" + sdb->LastError ());
            ui->statusbar->showMessage ("Невдалось виконати операцію! Проблема з підключеням до бази даних");
            return;
        }
        else {
            sdb->DeleteRow (SOLD_GOODS_TABLE, GOODS_ID, return_goods->GetGoodsId ());
            Update (ui->main_table_view->currentIndex ().row ());
            ui->statusbar->showMessage ("Повернено від покупця товар, модель " + return_goods->GetModelName () +
                                        " виробника " + return_goods->GetBrand () +
                                        ", розмір " + return_goods->GetSize ());
        }
    }
    else {
        ui->statusbar->clearMessage ();
    }
}

void MainWindow::onActionAddModel() {
    AddModelDialog* add_model = new AddModelDialog (sdb,  QVariantList(), this);
    if(add_model->exec () == QDialog::Accepted) {
        QByteArray pic_byte_arr;
        if(add_model->getPhotoPath ().size ()){
            QPixmap pic(add_model->getPhotoPath ());
            QBuffer buff(&pic_byte_arr);
            buff.open (QIODevice::WriteOnly);
            pic.save (&buff, "JPG");
        }

        QVariantList data = { add_model->getModel (),
                              add_model->getSeason (),
                              add_model->getCategory (),
                              add_model->getBrand (),
                              add_model->getWholesalepr (),
                              add_model->getRetailpr (),
                              pic_byte_arr,
                              QDateTime::currentDateTime ().toString (SQL_DATE_TIME_FORMAT),
                              0 };
        QStringList columns = { MODEL_NAME, SEASON, CATEGORY, BRAND, WHOLESALE_PRICE, RETAIL_PRICE, PIC, DATE, ARCHIVE };
        if (!sdb->UpdateInsertData (sdb->GenerateInsertQuery (MODELS_TABLE, columns),
                                       sdb->GenerateBindValues (columns),
                                       data)) {
            QMessageBox::critical (this, "Error!", "Невдалось додати модель! Проблема з підключеням до бази даних!\n\nПомилка:\n" + sdb->LastError ());
            ui->statusbar->showMessage ("Невдалось додати модель! Проблема з підключеням до бази даних");
            return;
        }
        Update(filter_model->rowCount ());
        ui->statusbar->showMessage ("Додано модель " + add_model->getModel () + ", виробник " + add_model->getBrand ());
    }
    else {
        ui->statusbar->clearMessage ();
    }
}

void MainWindow::onActionEditModel() {
    QVariantList row = sdb->SelectRow ("*", MODELS_TABLE, MODEL_ID, filter_model->data (filter_model->index (ui->main_table_view->currentIndex ().row (), MODEL_ID_COL)).toString (), filter_model->columnCount ());
    AddModelDialog* edit_model = new AddModelDialog(sdb, row, this);
    if (edit_model->exec () == QDialog::Accepted){
        QVariantList data = { edit_model->getModel(),
                              edit_model->getSeason(),
                              edit_model->getCategory(),
                              edit_model->getBrand(),
                              edit_model->getWholesalepr(),
                              edit_model->getRetailpr(),
                              QDateTime::currentDateTime ().toString (SQL_DATE_TIME_FORMAT) };
        QStringList columns = { MODEL_NAME, SEASON, CATEGORY, BRAND, WHOLESALE_PRICE, RETAIL_PRICE, DATE };

        if (edit_model->getPhotoPath ().isEmpty () && edit_model->isPicDeleted ()){
            data.append (QByteArray());
            columns.append (PIC);
        }
        else if(!edit_model->getPhotoPath ().isEmpty ()) {
            QPixmap pic(edit_model->getPhotoPath ());
            QByteArray pic_byte_arr;
            QBuffer buff(&pic_byte_arr);
            buff.open (QIODevice::WriteOnly);
            pic.save (&buff, "JPG");

            data.append (pic_byte_arr);
            columns.append (PIC);
        }

        if (!sdb->UpdateInsertData (sdb->GenerateUpdateQuery (MODELS_TABLE, columns, MODEL_ID, row.at(MODEL_ID_COL).toString ()),
                                    sdb->GenerateBindValues (columns),
                                    data)) {
            QMessageBox::critical (this, "Error!", "Невдалось відредагувати модель! Проблема з підключеням до бази даних!\n\nПомилка:\n" + sdb->LastError ());
            ui->statusbar->showMessage ("Невдалось відредагувати модель! Проблема з підключеням до бази даних");
            return;
        }
        Update(ui->main_table_view->currentIndex ().row ());
        ui->statusbar->showMessage ("Відредаговано модель " + edit_model->getModel () + ", виробник " + edit_model->getBrand ());
    }
    else {
        ui->statusbar->clearMessage ();
    }
}

void MainWindow::onActionDelModel() {
    QString model_name = filter_model->data (filter_model->index (ui->main_table_view->currentIndex ().row (), MODEL_NAME_COL)).toString ();
    QMessageBox msgbox;
    msgbox.setIcon (QMessageBox::Question);
    msgbox.setWindowTitle ("Видалити модель");
    msgbox.setText ("Ви дійсно бажаєте видалити з бази даних модель " + model_name +"?");
    msgbox.addButton ("Видалити", QMessageBox::AcceptRole);
    msgbox.addButton ("Скасувати", QMessageBox::RejectRole);
    if(msgbox.exec () == QMessageBox::AcceptRole) {
        if(sdb->DeleteRow (MODELS_TABLE, MODEL_NAME, model_name)){
            ui->statusbar->showMessage ("Видалено модель " + model_name);
        }
        Update(ui->main_table_view->currentIndex ().row () - 1);
    }
}

void MainWindow::onActionAddToArchive() {
    QString model_id = filter_model->data (filter_model->index (ui->main_table_view->currentIndex ().row (), MODEL_ID_COL)).toString ();
    QString model_name = filter_model->data (filter_model->index (ui->main_table_view->currentIndex ().row (), MODEL_NAME_COL)).toString ();
    QVariantList data = { 1 };
    QStringList columns = { ARCHIVE };

    if (!sdb->UpdateInsertData (sdb->GenerateUpdateQuery (MODELS_TABLE, columns, MODEL_ID, model_id),
                                sdb->GenerateBindValues (columns),
                                data)) {
        QMessageBox::critical (this, "Error!", "Невдалось перемістити модель в архів! Проблема з підключеням до бази даних!\n\nПомилка:\n" + sdb->LastError ());
        return;
    }
    else {
        ui->statusbar->showMessage ("Модель " + model_name + " переміщено в архів");
    }
    Update(ui->main_table_view->currentIndex ().row ());
}

void MainWindow::onActionViewArchive() {
    ArchiveDialog* archive_dialog = new ArchiveDialog(sdb, this);
    archive_dialog->exec ();
    Update (0);
    ui->statusbar->showMessage ("");
}

void MainWindow::onActionReport() {
    ReportDialog* report_dialog = new ReportDialog(sdb, this);
    if(report_dialog->exec () == QDialog::Accepted) {
        QString path = QFileDialog::getExistingDirectory(this, tr("Зберегти звіт в ..."),
                                                        QDir::homePath (),
                                                        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if(!path.isEmpty ()) {
            QVector<QVariantList> table;
            switch (report_dialog->GetReportType ()) {
            case SOLD_GOODS_REPORT:
                table = sdb->SelectTable (SqlQueries::SelectTableForReport (SOLD_GOODS_TABLE, SALE_DATE, report_dialog->GetDateFrom (), report_dialog->GetDateTo ()));
                for (int row = 0; row < REPORT_SOLD_GOODS_HEADERS_LIST.size (); ++row) {
                    table[0][row] = QVariant(REPORT_SOLD_GOODS_HEADERS_LIST.at (row));
                }
                break;
            case AVAILABLE_GOODS_REPORT:
                table = sdb->SelectTable (SqlQueries::SelectTableForReport (AVAILABLE_GOODS_TABLE, GOODS_DATE, report_dialog->GetDateFrom (), report_dialog->GetDateTo ()));
                for (int row = 0; row < REPORT_AVAILABLE_GOODS_HEADERS.size (); ++row) {
                    table[0][row] = QVariant(REPORT_AVAILABLE_GOODS_HEADERS.at (row));
                }
                break;
            }
            CreateReportCSV (table, path);
        }
    }
}

void MainWindow::onActionUpdate() {
    Update(ui->main_table_view->currentIndex ().row ());
}

void MainWindow::onActionDictionary() {
    DictionaryDialog* dict_dialog = new DictionaryDialog(sdb, this);
    dict_dialog->exec ();
    Update(0);
}

void MainWindow::onActionCopyRestoreDb() {
    CopyRestoreDbDialog* copy_restore_db = new CopyRestoreDbDialog(this);
    if(copy_restore_db->exec () == QDialog::Accepted){
        if (copy_restore_db->GetOption () == CREATE_COPY_DB){
            ReserveCopyDb();
        }
        else {
            RestoreDb();
        }
    }
    else {
        ui->statusbar->clearMessage ();
    }
}

void MainWindow::ReserveCopyDb() {
    QString path = QFileDialog::getExistingDirectory(this, tr("Зберегти базу даних в ..."),
                                                    QDir::homePath (),
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(QFile::copy (DB_PATH, path + "/shoes_strore_db " + QDateTime::currentDateTime ().toString (FS_DATE_TIME_FORMAT) + ".sqlite3")){
        ui->statusbar->showMessage ("Резервну копію бази даних збережено в папці " + path );
    }
    else {
        ui->statusbar->showMessage ("Не вдалось зробити копію бази даних!");
        QMessageBox::critical (this, "Error!", "Не вдалось зробити копію бази даних!", QMessageBox::Ok);
    }
}

void MainWindow::RestoreDb() {
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
                    ui->statusbar->showMessage ("Базу даних відновлено!");
                    QMessageBox::information (this, "Shoes Store", "Для коректної роботи після відновлення бази даних, рекомендується перезапустити програму", QMessageBox::Ok);
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

void MainWindow::SearchTextChanged(QString text) {
    filter_model->setFilterFixedString (text);
    Update(ui->main_table_view->currentIndex ().row ());
}

void MainWindow::SetSearchType(QString type) {
    if(type == MODELS_TABLE_HEADERS_LIST[MODEL_NAME_COL]) {
        filter_model->setFilterKeyColumn (MODEL_NAME_COL);
    }
    else if(type == MODELS_TABLE_HEADERS_LIST[SEASON_COL]){
        filter_model->setFilterKeyColumn (SEASON_COL);
    }
    else if(type == MODELS_TABLE_HEADERS_LIST[CATEGORY_COL]){
        filter_model->setFilterKeyColumn (CATEGORY_COL);
    }
    else if(type == MODELS_TABLE_HEADERS_LIST[BRAND_COL]){
        filter_model->setFilterKeyColumn (BRAND_COL);
    }
}

void MainWindow::ShowPic() {
    QPixmap pic;
    pic.loadFromData (filter_model->data(filter_model->index (ui->main_table_view->currentIndex ().row (), PIC_COL)).toByteArray ());
    if (!pic.isNull ()){
        ui->pic_lbl->setPixmap (pic.scaledToHeight(ui->pic_lbl->height()));
    }
    else {
        ui->pic_lbl->setPixmap(QPixmap(":/pics/default_pic.png"));
    }
}

void MainWindow::ShowGoodsInfo() {
    if(filter_model->rowCount ()){
        TableInit(ui->goods_info_table, QStringList() << "Розмір" << "Кількість");
        int sum_count(0);
        QString model_id = filter_model->data (filter_model->index (ui->main_table_view->currentIndex ().row (), MODEL_ID_COL)).toString ();
        for (int i = S36; i <= WITHOUT_SIZE; ++i) {
            QString size = i == WITHOUT_SIZE ? "б.р." : QString::number (i + 36);
            int count = sdb->SelectCount (AVAILABLE_GOODS_TABLE,
                                          MODEL_ID,   GOODS_SIZE,
                                          "=",        "=",
                                          model_id,   size);
            sum_count += count;
            if(count) {
                ui->goods_info_table->insertRow (ui->goods_info_table->rowCount ());
                for(int col = 0; col < 2; ++col) {
                    QTableWidgetItem* item = new QTableWidgetItem (col == 0 ? size : QString::number (count));
                    ui->goods_info_table->setItem(ui->goods_info_table->rowCount() - 1, col, item);
                }
            }
        }
        ui->goods_info_table->insertRow (ui->goods_info_table->rowCount ());
        for(int col = 0; col < 2; ++col) {
            QTableWidgetItem* item = new QTableWidgetItem (col == 0 ? "ВСЬОГО" : QString::number (sum_count));
            ui->goods_info_table->setItem(ui->goods_info_table->rowCount() - 1, col, item);
        }
    }
}

void MainWindow::Update(int row) {
    main_tbl_sql_model->setQuery (SqlQueries::GoodsCountMainTable ());
    ui->main_table_view->selectRow (row);
    ShowPic ();
    SetSummary ();
    ShowGoodsInfo ();
    UpdateCounts ();
    UpdateButtons ();
}

void MainWindow::UpdateButtons() {
    action_add_goods->setEnabled (models_table_count);
    action_sale_goods->setEnabled (available_goods_table_count);
    action_return_goods->setEnabled (sold_goods_table_count);
    action_add_new_model->setEnabled (brand_table_count && season_table_count && category_table_count);
    action_edit_model->setEnabled (models_table_count /*&& !sold_goods_by_model_count && !available_goods_by_model_count*/);
    action_del_model->setEnabled (models_table_count && !sold_goods_by_last_year_count && !available_goods_by_model_count);
    action_add_to_archive->setEnabled (!available_goods_by_model_count && models_table_count);
    action_view_archive->setEnabled (models_table_count);
    action_report->setEnabled (available_goods_table_count || sold_goods_table_count);
    action_update->setEnabled (true);
    action_dictionary->setEnabled (true);
    action_copy_restore->setEnabled (true);
}

void MainWindow::UpdateCounts() {
    QString model_id = filter_model->data (filter_model->index (ui->main_table_view->currentIndex ().row (), MODEL_ID_COL)).toString ();
    brand_table_count = sdb->SelectCount(BRANDS_TABLE);
    season_table_count = sdb->SelectCount (SEASONS_TABLE);
    category_table_count = sdb->SelectCount (CATEGORIES_TABLE);
    models_table_count = sdb->SelectCount (MODELS_TABLE);
    available_goods_table_count = sdb->SelectCount (AVAILABLE_GOODS_TABLE);
    available_goods_by_model_count = sdb->SelectCount (AVAILABLE_GOODS_TABLE, MODEL_ID, "=", model_id);
    sold_goods_table_count = sdb->SelectCount(SOLD_GOODS_TABLE);
    sold_goods_by_last_year_count = sdb->Select (SqlQueries::SoldGoodsByLastYear (model_id)).toInt ();
    sold_goods_by_model_count = sdb->SelectCount (SOLD_GOODS_TABLE, MODEL_ID, "=", model_id);
}

MainWindow::~MainWindow() {
    delete ui;
}
