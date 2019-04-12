#ifndef STOREMAINWINDOW_H
#define STOREMAINWINDOW_H

#include "general.h"
#include "addmodeldialog.h"
#include "addgoodsdialog.h"
#include "database.h"

namespace Ui {
class StoreMainWindow;
}

class StoreMainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::StoreMainWindow *ui;
    QSqlTableModel* model;
    DataBase* sdb;
    AddGoodsDialog* add_goods;
    QToolBar* toolbar;
    QAction* action_add_goods;
    QAction* action_sale_goods;
    QAction* action_add_new_model;
    QAction* action_del_model;
    QAction* action_report;
    QHBoxLayout* h_main_layout;
    QVBoxLayout* rv_layout;
    QVBoxLayout* lv_layout;

    QTableView* main_table_view;
    QTableWidget* goods_info_table;
    QLabel* pic_label;
    QSplitter* splitter;

    void resizeEvent(QResizeEvent *event);
    void MaibTableInit();
    void GoodsInfoTableInit();
    void BuildToolBar();
    void AddGoodsThread();
public:
    explicit StoreMainWindow(QWidget *parent = nullptr);
    ~StoreMainWindow();
private slots:
    void onActionAddGoods();
    void onActionSaleGoods();
    void onActionAddModel();
    void onActionDelModel();
    void onActionReport();
    void ShowPic();
    void ShowGoodsInfo();
};

#endif // STOREMAINWINDOW_H
