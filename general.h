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

const int SIZE_WID = 32;
const int LABEL_WID = 150;
const int LINE_WID = 200;

const QDoubleValidator PRICE_VALIDATOR(0.0, 1000000.0, 2);
#endif // GENERAL

