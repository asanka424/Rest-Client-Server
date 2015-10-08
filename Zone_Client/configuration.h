#ifndef CONFIGURATION
#define CONFIGURATION

#include <QtCore/QSettings>
#include <QtCore/QFile>
#include <QtCore/QMutex>

class Configuration {
public:
    ~Configuration(){}

public:

    static Configuration* instance(void);

    inline bool validateSettingsFile(void) {
        return QFile::exists("../../Zone_Client/resources/Configurations.conf");
    }

    QString getServerHTTPSURL(void) {
        return getSettingFromConf<QString>("Server", "HTTPS-URL");
    }
    QString getServerHTTPURL(void) {
        return getSettingFromConf<QString>("Server", "HTTP-URL");
    }

    quint16 getServerPort(void) {
        return getSettingFromConf<quint16>("Server", "port");
    }



private:
    template<typename T>
    inline T getSettingFromConf(const QString& inGroup, const QString& inField) {
        //T ret = 0;

        if (inGroup == "" || inField == "")
            return 0;
        QSettings file("../../Zone_Client/resources/Configurations.conf", QSettings::IniFormat);
        QStringList keys = file.allKeys();
        file.beginGroup(inGroup);

        return file.value(inField).value<T>();
    }

private:
    static Configuration*   mInstance;
    static QMutex           mMut;
private:
    Configuration(){}
    Configuration(const Configuration&){}
    Configuration& operator=(const Configuration&){}
};

#endif // CONFIGURATION


