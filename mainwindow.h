#ifndef MAINWINDOW1_H
#define MAINWINDOW1_H

#include "general.h"
#include "addmodeldialog.h"
#include "addgoodsdialog.h"
#include "database.h"
#include "saledialog.h"
#include "reportdialog.h"
#include "dictionarydialog.h"
#include "returngoodsdialog.h"
#include "myproxymodel.h"
#include "sqlqueries.h"
#include "copyrestoredbdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::MainWindow *ui;
    QSqlTableModel* sql_model;
    MyProxyModel* filter_model;
    DataBase* sdb;
    QToolBar* toolbar;
    QAction* action_add_goods;
    QAction* action_sale_goods;
    QAction* action_return_goods;
    QAction* action_add_new_model;
    QAction* action_edit_model;
    QAction* action_del_model;
    QAction* action_report;
    QAction* action_dictionary;
    QAction* action_update;
    QAction* action_copy_restore;

    QLineEdit* search_line;
    QComboBox* search_combo;

    int brand_table_count;
    int season_table_count;
    int category_table_count;
    int models_table_count;
    int available_goods_table_count;
    int available_goods_by_model_count;
    int sold_goods_table_count;
    int sold_goods_by_last_year_count;
    int sold_goods_by_model_count;

    void resizeEvent(QResizeEvent *event);
    void MainTableInit();
    void TableInit(QTableWidget* table, QStringList headers);
    void SetSummary();
    void BuildToolBar();
    void SwitchButtons(State state);
    void CreateReportCSV(const QVector<QVariantList>& table, const QString& path);
    void ReserveCopyDb();
    void RestoreDb();

public:
    explicit MainWindow(DataBase* data_base, QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void onActionAddGoods();
    void onActionSaleGoods();
    void onActionReturnGoods();
    void onActionAddModel();
    void onActionEditModel();
    void onActionDelModel();
    void onActionReport();
    void onActionUpdate();
    void onActionDictionary();
    void onActionCopyRestoreDb();
    void SearchTextChanged(QString text);
    void SetSearchType(QString type);
    void ShowPic();
    void ShowGoodsInfo();
    void Update(int row);
    void UpdateButtons();
    void UpdateCounts();

signals:
    void finishAddGoods();
};

#endif // MAINWINDOW1_H
