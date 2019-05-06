#ifndef SETTINGS_H
#define SETTINGS_H
#include "general.h"

class Settings {
    QSettings my_conf;
    QString db_path;
public:
    Settings();
    void ReadSettings();
    void WriteSettings();
    QString GetDataBasePath();
    void SetDataBasePath(const QString& path);
    ~Settings();
};

#endif // SETTINGS_H
