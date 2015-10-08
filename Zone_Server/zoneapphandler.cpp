#include "zoneapphandler.h"
#include "datahandler.h"
#include "userhandler.h"
#include "appdepotutilities.h"

#include "abstractoperationshandler.h"


ZoneAppHandler::ZoneAppHandler(QObject *parent) : AbstractAppsHander(parent)
{
    mModules["user"] = new UserHandler(this);
    mModules["data"] = new DataHandler(this);
}

ZoneAppHandler::~ZoneAppHandler()
{

}

void ZoneAppHandler::handleRequest(QStringList &operations, QString &method,QByteArray &requestData, QMap<QString, QString> &queryMap,
                       QMap<QString,QString> &headerMap, Tufao::HttpServerResponse &response)
{
    if (operations.isEmpty())
    {
        response.writeHead(Tufao::HttpResponseStatus::BAD_REQUEST);
        response.end(JSONHandler::encodeError(INVALID_REQUEST));
        return;
    }
    if (mModules.contains(operations[0]))
    {
        AbstractOperationsHandler *handler = mModules[operations[0]];
        operations.removeAt(0);
        handler->handleOperations(operations,method,requestData,queryMap,headerMap,response);

    }
    else
    {
        response.writeHead(Tufao::HttpResponseStatus::NOT_IMPLEMENTED);
        response.end(JSONHandler::encodeError(OPERATION_NOT_IMPLEMENTED));
    }
}
