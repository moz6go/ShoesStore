#ifndef REPORTDIALOG_H
#define REPORTDIALOG_H

#include <QDialog>
#include "general.h"
#include "database.h"

namespace Ui {
class ReportDialog;
}

class ReportDialog : public QDialog
{
    Q_OBJECT
    Ui::ReportDialog *ui;

    QComboBox* report_cb;
    QAction* report_action;
    QToolBar* tool_bar;
    QTableView* main_table;
    DataBase* sdb;
private slots:
    void onActionReport();

public:
    explicit ReportDialog(DataBase* data_base, QWidget *parent = nullptr);
    ~ReportDialog();

};

#endif // REPORTDIALOG_H
