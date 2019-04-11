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
    QSqlRelationalTableModel* rel_model;
    DataBase* sdb;
    QToolBar* t_bar_;
    QAction* a_add_goods_;
    QAction* a_sale_goods_;
    QAction* a_add_new_model_;
    QAction* a_del_model_;
    QAction* a_report_;
    QHBoxLayout* h_main_layout_;
    QVBoxLayout* rv_layout_;
    QVBoxLayout* lv_layout_;

    QGridLayout* main_l_;
    QTableView* t_view_;
    QTableWidget* t_goods_info_;
    QLabel* l_pic_;
    QSplitter* splitter_;

    void resizeEvent(QResizeEvent *event);
    void GoodsInfoTableInit();
public:
    explicit StoreMainWindow(QWidget *parent = nullptr);
    void BuildToolBar();
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
