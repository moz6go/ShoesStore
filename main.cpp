#include "storemainwindow.h"
#include "database.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    //qDebug() << QStyleFactory::keys ();
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    DataBase* sdb = new DataBase();

    if (!sdb->ConnectToDataBase (DB_PATH)) {
        QMessageBox::critical (nullptr, "Error", "Неможливо з'єднатись з базою даних!", QMessageBox::Ok);
        return -1;
    }
    StoreMainWindow w(sdb);
    w.show();
    return a.exec();
}
