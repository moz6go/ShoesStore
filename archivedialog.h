#ifndef ARCHIVEDIALOG_H
#define ARCHIVEDIALOG_H

#include "general.h"
#include "database.h"
#include "myproxymodel.h"
#include "sqlqueries.h"

namespace Ui {
class ArchiveDialog;
}

class ArchiveDialog : public QDialog
{
    Q_OBJECT
    Ui::ArchiveDialog *ui;
    DataBase* sdb;
    QSqlQueryModel* main_tbl_sql_model;
    MyProxyModel* filter_model;

    void ModelsTableInit();
public:
    explicit ArchiveDialog(DataBase* data_base, QWidget *parent = nullptr);
    ~ArchiveDialog();
private slots:
    void onRestorePbClick();
    void ShowPic();
    void Update(int row);
};

#endif // ARCHIVEDIALOG_H
