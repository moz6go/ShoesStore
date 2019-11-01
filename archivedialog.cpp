#include "archivedialog.h"
#include "ui_archivedialog.h"
#include "mainwindow.h"

ArchiveDialog::ArchiveDialog(DataBase* data_base, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ArchiveDialog)
{
    ui->setupUi(this);
    sdb = data_base;

    main_tbl_sql_model = new QSqlQueryModel(this);
    main_tbl_sql_model->setQuery (SqlQueries::ModelsInArchive ());

    filter_model = new MyProxyModel(this);
    filter_model->setSourceModel (main_tbl_sql_model);
    filter_model->setFilterCaseSensitivity (Qt::CaseInsensitive);
    filter_model->setFilterKeyColumn (MODEL_NAME_COL);
    filter_model->sort (DATE_COL, Qt::DescendingOrder);

    ui->models_table_view->setModel (filter_model);

    for(int col = 0; col < main_tbl_sql_model->columnCount(); ++col) {
        main_tbl_sql_model->setHeaderData(col, Qt::Horizontal, MODELS_TABLE_HEADERS_LIST[col]);
    }
    ui->models_table_view->setColumnHidden(WHOLESALE_PRICE_COL, true);
    ui->models_table_view->setColumnHidden(RETAIL_PRICE_COL, true);
    MainWindow::MainTableInit (ui->models_table_view);
    int col_width = ui->models_table_view->width () / 4;
    for (int c = 0; c < 4; ++c) {
        ui->models_table_view->setColumnWidth (c, col_width);
    }
    QObject::connect (ui->models_table_view, &QTableView::clicked, [=]{Update(ui->models_table_view->currentIndex().row());});
    QObject::connect (ui->cancel_pb, &QPushButton::clicked, this, &ArchiveDialog::reject);
    QObject::connect (ui->restore_pb, &QPushButton::clicked, this, &ArchiveDialog::onRestorePbClick);
    Update(0);

}

void ArchiveDialog::ShowPic() {
    QPixmap pic;
    pic.loadFromData (filter_model->data(filter_model->index (ui->models_table_view->currentIndex ().row (), PIC_COL)).toByteArray ());
    if (!pic.isNull ()){
        ui->pic_lbl->setPixmap (pic.scaledToHeight(ui->pic_lbl->height()));
    }
    else {
        ui->pic_lbl->setPixmap(QPixmap(":/pics/default_pic.png"));
    }
}

void ArchiveDialog::Update(int row) {
    main_tbl_sql_model->setQuery (SqlQueries::ModelsInArchive ());
    ui->models_table_view->selectRow (row);
    ui->restore_pb->setEnabled (main_tbl_sql_model->rowCount ());
    ShowPic ();
}

ArchiveDialog::~ArchiveDialog() {
    delete ui;
}

void ArchiveDialog::onRestorePbClick() {
    QString model_id = filter_model->data (filter_model->index (ui->models_table_view->currentIndex ().row (), MODEL_ID_COL)).toString ();
    QVariantList data = { 0 };
    QStringList columns = { ARCHIVE };

    if (!sdb->UpdateInsertData (sdb->GenerateUpdateQuery (MODELS_TABLE, columns, MODEL_ID, model_id),
                                sdb->GenerateBindValues (columns),
                                data)) {
        QMessageBox::critical (this, "Error!", "Невдалось відновити модель! Проблема з підключеням до бази даних!\n\nПомилка:\n" + sdb->LastError ());
        return;
    }
    Update(ui->models_table_view->currentIndex ().row ());
}

