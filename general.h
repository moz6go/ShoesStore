#ifndef GENERAL
#define GENERAL

#include <QApplication>
#include <QMainWindow>
#include <QtDebug>
#include <QObject>
#include <QThread>
#include <QMutex>
#include <QMainWindow>
#include <QToolBar>
#include <QLabel>
#include <QHeaderView>
#include <QTableView>
#include <QSplitter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QFileSystemModel>
#include <QPushButton>
#include <QFileDialog>
#include <QtSql/QSql>

const int SIZE_WID = 32;

enum SIZES {

};

struct Model {
    QString model_name;
    QString season;
    QString brand;
    double wholesale_price;
    double retail_price;
    QString photo_path;
};

struct Product : public Model {

};

#endif // GENERAL

