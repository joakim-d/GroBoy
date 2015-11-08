#include "config.h"
#include <QFile>
#include <QSettings>

const QString config_file_name = "config.ini";

Config::Config(){
    QFile config_file(config_file_name);
    config_file.open(QIODevice::ReadWrite);
    if(!config_file.isOpen()){
        bool res = QFile::copy(":/default_config.ini", config_file_name);
        Q_ASSERT(res);
    }
}

Input Config::input() const {
    Input res;
    QSettings settings(config_file_name, QSettings::IniFormat);

    res.up = settings.value("Input/up").toInt();
    res.down = settings.value("Input/down").toInt();
    res.left = settings.value("Input/left").toInt();
    res.right = settings.value("Input/right").toInt();
    res.select = settings.value("Input/select").toInt();
    res.start = settings.value("Input/start").toInt();
    res.b = settings.value("Input/b").toInt();
    res.a = settings.value("Input/a").toInt();

    return res;
}

void Config::saveInput(const Input &input) {
    QSettings settings(config_file_name, QSettings::IniFormat);

    settings.setValue("Input/up", input.up);
    settings.setValue("Input/down", input.down);
    settings.setValue("Input/left", input.left);
    settings.setValue("Input/right", input.right);
    settings.setValue("Input/select", input.select);
    settings.setValue("Input/start", input.start);
    settings.setValue("Input/b", input.b);
    settings.setValue("Input/a", input.a);

}
