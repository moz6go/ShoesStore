#include "storemainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StoreMainWindow w;
    w.show();
    return a.exec();
}
