#ifndef CONFIGREADER_H
#define CONFIGREADER_H

#include <QMap>
#include <QString>

class ConfigReader
{
public:
    QMap<QString, QString> readConfig();
};

#endif // CONFIGREADER_H
