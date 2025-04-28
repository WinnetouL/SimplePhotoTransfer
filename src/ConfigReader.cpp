#include "ConfigReader.h"

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>

QMap<QString, QString> ConfigReader::readConfig()
{
    QMap<QString, QString> configMap;

    QFile configFile("config.txt");
    QFileInfo configFileInfo(configFile);

    if (!configFileInfo.exists() || !configFileInfo.isReadable()) {
        QString error = "Cannot open config file: ./" + configFile.fileName();
        return QMap<QString, QString>({{"error", error}});
    }

    if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString error = "Failed to open config file: ./" + configFile.fileName();
        return QMap<QString, QString>({{"error", error}});
    }

    QTextStream in(&configFile);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty() || line.startsWith("# ")) {
            continue;
        }

        QStringList parts = line.split('=');
        if (parts.size() == 2) {
            configMap.insert(parts[0].trimmed(), parts[1].trimmed());
        }
    }

    if (!configMap.contains("search_directory")) {
        QString error = "Configuration file misconfigured.";
        return QMap<QString, QString>({{"error", error}});
    }

    QDir searchDir(configMap.value("search_directory"));  // TODO if path is "" QDir will init with current working directory
    QFileInfo searchDirInfo(searchDir.absolutePath());
    if (!searchDirInfo.exists() || !searchDirInfo.isReadable()) {
        QString error = "Der Ordner für die Übertragung der Bilder\n"
                        "konnte nicht gefunden werden.\n\n"
                        "Ist das Handy angeschlossen?\n"
                        "Hast du die Erlaubnis zum übertragen der Bilder erteilt?";
        return QMap<QString, QString>({{"error", error}});
    } else {
        configMap["search_directory"] = searchDir.absolutePath();
    }

    configFile.close();
    return configMap;
}
