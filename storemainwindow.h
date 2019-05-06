#ifndef STOREMAINWINDOW_H
#define STOREMAINWINDOW_H

#include "general.h"
#include "addmodeldialog.h"
#include "addgoodsdialog.h"
#include "database.h"
#include "saledialog.h"
#include "reportdialog.h"
#include "dictionarydialog.h"

namespace Ui {
    class StoreMainWindow;
}

class StoreMainWindow : public QMainWindow
{
    Q_OBJECT
    QSettings my_conf;
    Ui::StoreMainWindow *ui;
    QString db_path;
    QSqlTableModel* model;
    QSortFilterProxyModel* filter_model;
    DataBase* sdb;
    AddGoodsDialog* add_goods;
    SaleDialog* sale_goods;
    QToolBar* toolbar;
    QAction* action_add_goods;
    QAction* action_sale_goods;
    QAction* action_add_new_model;
    QAction* action_del_model;
    QAction* action_report;
    QAction* action_dictionary;
    QAction* action_update;
    QAction* action_search;

//    SearchType search_type;
    QLineEdit* search_line;
    QComboBox* search_combo;
    QComboBox* search_combo_comp;

    QHBoxLayout* h_main_layout;
    QVBoxLayout* rv_layout;
    QVBoxLayout* lv_layout;

    QTableView* main_table_view;
    QTableWidget* goods_info_table;
    QTableWidget* summary_table;

    QLabel* pic_label;

    void resizeEvent(QResizeEvent *event);
    void MainTableInit();
    void TableInit(QTableWidget* table, QStringList headers);
    void SetSummary();
    void BuildToolBar();
    void SwitchButtons(State state);
    void CreateReportCSV(const QVector<QVariantList>& table, const QString& path);
public:
    explicit StoreMainWindow(DataBase* data_base, QWidget *parent = nullptr);
    ~StoreMainWindow();
private slots:
    void onActionAddGoods();
    void onActionSaleGoods();
    void onActionAddModel();
    void onActionDelModel();
    void onActionReport();
    void onActionUpdate();
    void onActionDictionary();
    void SearchTextChanged(QString text);
    void SetSearchType(QString type);
    void ShowPic();
    void ShowGoodsInfo();
    void Update(int row);
    void UpdateButtons();
signals:
    void finishAddGoods();
};

#endif // STOREMAINWINDOW_H
