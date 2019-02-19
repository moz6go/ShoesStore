#ifndef STOREMAINWINDOW_H
#define STOREMAINWINDOW_H

#include "general.h"

namespace Ui {
class StoreMainWindow;
}

class StoreMainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::StoreMainWindow *ui;
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
    QTableView* t_goods_info_;
    QPushButton* txt_;
    QSplitter* splitter_;
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
};

#endif // STOREMAINWINDOW_H
