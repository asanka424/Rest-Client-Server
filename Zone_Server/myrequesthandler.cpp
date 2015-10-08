#include "myrequesthandler.h"

#include "httpserverrequest.h"
#include "httpserverresponse.h"
#include "headers.h"

#include <QUrlQuery>
#include "dbhandler.h"

#include "zoneapphandler.h"
#include "appdepotutilities.h"

MyRequestHandler::MyRequestHandler(QObject *parent)
    :   QObject(parent)
{
    mAppsMap["zone"] = new ZoneAppHandler(this);
}

MyRequestHandler::~MyRequestHandler()
{

}

bool MyRequestHandler::handleRequest(Tufao::HttpServerRequest& request, Tufao::HttpServerResponse& response)
{

    QPair<QString, QString> userAuth = DBHandler::instance()->getPassword(QString("asanka"));

    QString requestPath = request.url().path();
    QStringList operations = requestPath.split("/");
    if (operations.size() > 1)
    {
        QString app = operations.at(1);
        if (mAppsMap.contains(app))
        {
            QString requestMethod = QString(request.method());
            operations.removeAt(0);
            operations.removeAt(0);
            QByteArray requestData = request.readBody();
            QMap<QString,QString> queryMap,headerMap;
            QUrlQuery query(request.url().query());
            QList<QPair<QString,QString> > queryItems = query.queryItems();
            for (int i=0; i<queryItems.size(); i++)
            {
                QPair<QString, QString> item = queryItems[i];
                queryMap.insert(item.first,item.second);
            }
            Tufao::Headers headers = request.headers();
            Tufao::Headers::iterator i;
            for (i=headers.begin(); i != headers.end(); ++i)
                headerMap[QString(i.key())] = QString(i.value());
            AbstractAppsHander *handler = mAppsMap[app];
            handler->handleRequest(operations,requestMethod,requestData,queryMap,headerMap,response);
        }
        else
        {
            response.writeHead(Tufao::HttpResponseStatus::NOT_IMPLEMENTED);
            response.end(JSONHandler::encodeError(INVALID_APPLICATION));
        }
    }
    else
    {
        response.writeHead(Tufao::HttpResponseStatus::BAD_REQUEST);
        response.end(JSONHandler::encodeError(INVALID_REQUEST));
    }


}

