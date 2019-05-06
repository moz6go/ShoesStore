#include "settings.h"

Settings::Settings(){}

void Settings::ReadSettings() {
    my_conf.beginGroup ("/Settings");
    db_path = my_conf.value ("/path", "").toString ();
    my_conf.endGroup ();
}

void Settings::WriteSettings() {
    my_conf.beginGroup ("/Settings");
    my_conf.setValue ("/path", db_path);
    my_conf.endGroup ();
}

QString Settings::GetDataBasePath() {
    return db_path;
}

void Settings::SetDataBasePath(const QString &path) {
    db_path = path;
}

Settings::~Settings(){
    WriteSettings ();
}
