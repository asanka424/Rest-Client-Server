#ifndef MYREQUESTHANDLER_H
#define MYREQUESTHANDLER_H

#include <QtCore/QObject>
#include <QMap>

#include "abstracthttpserverrequesthandler.h"

#include "abstractappshander.h"

class MyRequestHandler :  public QObject, public Tufao::AbstractHttpServerRequestHandler
{
    Q_OBJECT
public:
    explicit MyRequestHandler(QObject *parent = 0);
    ~MyRequestHandler();

public slots:
   bool handleRequest(Tufao::HttpServerRequest& request, Tufao::HttpServerResponse& response) override;


private:

   QMap<QString, QString> mQueryMap;
   QString mPostBody;
   QString mRequestMethod;
   QMap<QString, AbstractAppsHander*> mAppsMap;
};

#endif // REQUESTHANDLER_H
