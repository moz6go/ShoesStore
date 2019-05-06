#include "storemainwindow.h"
#include "database.h"
#include "settings.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    //qDebug() << QStyleFactory::keys ();
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    Settings my_settings;
    my_settings.ReadSettings ();
    QString db_path = my_settings.GetDataBasePath ();

    if(db_path.isEmpty () || !QFile::exists(db_path)) {
        db_path = QFileDialog::getOpenFileName (this, "Виберіть базу даних", QDir::currentPath ());
    }
    my_settings.SetDataBasePath (db_path);

    DataBase* sdb = new DataBase();
    StoreMainWindow w(sdb);

    w.show();
    return a.exec();
}
