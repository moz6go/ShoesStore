#include "reportdialog.h"
#include "ui_reportdialog.h"


ReportDialog::ReportDialog(DataBase* data_base, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReportDialog)
{
    ui->setupUi(this);
    sdb = data_base;
    QMainWindow* main_window = new QMainWindow();

    tool_bar = new QToolBar(this);
    report_cb = new QComboBox(this);
    tool_bar->addWidget (new QLabel("Виберіть тип звіту  ",this));
    tool_bar->addWidget (report_cb);
    report_action = tool_bar->addAction(QPixmap(":/pics/report.png"), "Згенерувати звіт", this, SLOT(onActionReport()));
    tool_bar->setMovable (false);
    tool_bar->setIconSize (QSize(SIZE_WID_1, SIZE_WID_1));
    main_window->addToolBar(Qt::TopToolBarArea, tool_bar);
    main_table = new QTableView(this);
    main_window->setCentralWidget (main_table);

    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget (main_window);
    setLayout (layout);
}

void ReportDialog::onActionReport() {

}

ReportDialog::~ReportDialog() {
    delete ui;
}
