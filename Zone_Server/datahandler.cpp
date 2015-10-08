#include "datahandler.h"
#include "myhttpserver.h"
#include "httpserverrequest.h"
#include "httpserverresponse.h"
#include "headers.h"
#include <QUrl>
#include <QUrlQuery>
#include "dbhandler.h"
#include "appdepotutilities.h"

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;
using std::placeholders::_4;

DataHandler::DataHandler(QObject *parent)
    :   AbstractOperationsHandler(parent)
{
    DataOperation getDataOp = std::bind(&DataHandler::getData,this, _1, _2, _3,_4);
    DataOperation putDataOp = std::bind(&DataHandler::putData,this, _1, _2, _3,_4);
    DataOperation getUsersOp = std::bind(&DataHandler::getUsers,this, _1, _2, _3,_4);

    mOperations["getdata"] = getDataOp;
    mOperations["putdata"] = putDataOp;
    mOperations["getusers"] = getUsersOp;
}

DataHandler::~DataHandler()
{

}

void DataHandler::handleOperations(QStringList &operations, QString &method, QByteArray &requestData, QMap<QString, QString> &queryMap,
                                   QMap<QString,QString> &headerMap, Tufao::HttpServerResponse &response)
{
    //extract user and auth-token
    if ((headerMap.contains("x-user")) && (headerMap.contains("x-auth-token")))
    {
        QString user = headerMap["x-user"];
        QString authToken = headerMap["x-auth-token"];

        //authenticate
        if (AbstractOperationsHandler::authenticateUser(user,authToken))
        {
            if (operations.size() > 0)
            {
                QString operation = operations[0];
                if (mOperations.contains(operation))
                {
                    mOperations[operation](method,requestData,queryMap,response);
                }
                else
                {
                    response.writeHead(Tufao::HttpResponseStatus::NOT_FOUND);
                    response.end(JSONHandler::encodeError("Bad request"));
                }
            }
            else
            {
                response.writeHead(Tufao::HttpResponseStatus::BAD_REQUEST);
                response.end(JSONHandler::encodeError("Bad request"));
            }

        }
        else
        {
            response.writeHead(Tufao::HttpResponseStatus::FORBIDDEN);
            response.end(JSONHandler::encodeError("User not logged in"));
        }
    }
    else
    {
        response.writeHead(Tufao::HttpResponseStatus::BAD_REQUEST);
        response.end(JSONHandler::encodeError("Bad Reqeust"));
    }

}

void DataHandler::getData(QString &method,QByteArray &requestData, QMap<QString, QString> &queryMap, Tufao::HttpServerResponse &response)
{
    //works only for GET
    if (method != "GET")
    {
        response.writeHead(Tufao::HttpResponseStatus::BAD_REQUEST);
        response.end(JSONHandler::encodeError("Bad Reqeust"));
    }
    else
    {
        if (queryMap.contains("user"))
        {
            QString user = queryMap["user"];
            QList<QList<QString> > data = DBHandler::instance()->getData(user);
            QByteArray encodedData = JSONHandler::encodeData(data);
            qDebug() << encodedData;
            response.writeHead(Tufao::HttpResponseStatus::OK);
            response.end(encodedData);

        }
        else
        {
            response.writeHead(Tufao::HttpResponseStatus::BAD_REQUEST);
            response.end(JSONHandler::encodeError("Bad Reqeust"));
        }
    }
}

void DataHandler::putData(QString &method,QByteArray &requestData, QMap<QString, QString> &queryMap, Tufao::HttpServerResponse &response)
{
    //works only for PUT
    if (method != "PUT")
    {
        response.writeHead(Tufao::HttpResponseStatus::BAD_REQUEST);
        response.end(JSONHandler::encodeError("Bad Reqeust"));
    }
    else
    {
        QVariantMap requestMap = JSONHandler::decodeJsonString(requestData);
        if ((requestMap.contains("user")) && (requestMap.contains("data")))
        {
            QString user = requestMap["user"].toString();
            QList<QVariant> variantData = requestMap["data"].toList();
            QList<QList<QString> > data;
            for (int i=0; i<variantData.size(); i++)
            {
                QList<QString> row;
                QList<QVariant> variantRow = variantData[i].toList();
                for (int j=0; j<variantRow.size(); j++)
                    row.append(variantRow[j].toString());

                data.append(row);

            }
            if (DBHandler::instance()->putData(user,data))
            {
                response.writeHead(Tufao::HttpResponseStatus::OK);
                response.end();
            }
            else
            {
                response.writeHead(Tufao::HttpResponseStatus::INTERNAL_SERVER_ERROR);
                response.end(JSONHandler::encodeError("Internal Error while updating"));
            }

        }
        else
        {
            response.writeHead(Tufao::HttpResponseStatus::BAD_REQUEST);
            response.end(JSONHandler::encodeError("Bad Reqeust"));
        }
    }
}

void DataHandler::getUsers(QString &method,QByteArray &requestData, QMap<QString, QString> &queryMap, Tufao::HttpServerResponse &response)
{
    //works only for PUT
    if (method != "GET")
    {
        response.writeHead(Tufao::HttpResponseStatus::BAD_REQUEST);
        response.end(JSONHandler::encodeError("Bad Reqeust"));
    }
    else
    {
        //QVariantMap requestMap = JSONHandler::decodeJsonString(requestData);
        if (queryMap.contains("user"))
        {
            QString user = queryMap["user"];
            QList<QPair<QString, int> > users;
            if (DBHandler::instance()->getUsers(user,users))
            {

                response.writeHead(Tufao::HttpResponseStatus::OK);
                response.end(JSONHandler::encodeUserList("users",users));
            }
            else
            {
                response.writeHead(Tufao::HttpResponseStatus::INTERNAL_SERVER_ERROR);
                response.end(JSONHandler::encodeError("Internal Error while updating"));
            }

        }
        else
        {
            response.writeHead(Tufao::HttpResponseStatus::BAD_REQUEST);
            response.end(JSONHandler::encodeError("Bad Reqeust"));
        }
    }
}

