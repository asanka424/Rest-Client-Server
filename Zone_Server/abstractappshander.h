#ifndef ABSTRACTAPPSHANDER_H
#define ABSTRACTAPPSHANDER_H

#include <QObject>
#include <httpserver.h>

#define INVALID_APPLICATION "Invalid Application"
#define INVALID_REQUEST "Bad Request"


class AbstractAppsHander : public QObject
{
    Q_OBJECT
public:
    explicit AbstractAppsHander(QObject *parent = 0);
    ~AbstractAppsHander();
    virtual void handleRequest(QStringList &operations, QString &method, QByteArray &requestData, QMap<QString, QString> &queryMap,
                               QMap<QString,QString> &headerMap, Tufao::HttpServerResponse &response) = 0;

signals:

public slots:
};

#endif // ABSTRACTAPPSHANDER_H
