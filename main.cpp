#include "storemainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    //qDebug() << QStyleFactory::keys ();
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    StoreMainWindow w;
    w.show();
    return a.exec();
}
