#include "storemainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QApplication::setStyle(QStyleFactory::create("GTK+"));
    StoreMainWindow w;
    w.show();
    return a.exec();
}
