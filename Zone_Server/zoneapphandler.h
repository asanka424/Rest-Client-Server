#ifndef ZONEAPPHANDLER_H
#define ZONEAPPHANDLER_H

#include <QObject>
#include <QStringList>
#include <QMap>

#include "httpserverrequest.h"
#include "httpserverresponse.h"

#include "abstractappshander.h"

class AbstractOperationsHandler;
class ZoneAppHandler : public AbstractAppsHander
{
    Q_OBJECT
public:
    explicit ZoneAppHandler(QObject *parent = 0);
    ~ZoneAppHandler();

    void handleRequest(QStringList &operations, QString &method, QByteArray &requestData, QMap<QString, QString> &queryMap,
                       QMap<QString,QString> &headerMap, Tufao::HttpServerResponse &response);
signals:

public slots:
private:
    QMap<QString, AbstractOperationsHandler*> mModules;
};

#endif // ZONEAPPHANDLER_H
