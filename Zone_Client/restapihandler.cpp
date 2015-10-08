#include "restapihandler.h"
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>
#include <QMap>
#include <QMapIterator>
#include <QNetworkReply>
#include <QDebug>
#include "appdepotutilities.h"
#include "configuration.h"


using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;
RestApiHandler::RestApiHandler(QObject *parent) : QObject(parent)
{
    mNAM = new QNetworkAccessManager(this);
    QObject::connect(mNAM,&QNetworkAccessManager::sslErrors,[](QNetworkReply *reply, QList<QSslError> errors){reply->ignoreSslErrors();});

    ReplyHandle signinReply = std::bind(&RestApiHandler::signInReplyHandle,this, _1,_2,_3);
    ReplyHandle signupReply = std::bind(&RestApiHandler::signUpReplyHandle,this, _1,_2,_3);
    ReplyHandle checkUserReply = std::bind(&RestApiHandler::checkUserNameReplyHandle,this, _1,_2,_3);
    ReplyHandle getDataReply = std::bind(&RestApiHandler::getDataReplyHandle,this, _1,_2,_3);
    ReplyHandle putDataReply = std::bind(&RestApiHandler::putDataReplyHandle,this, _1,_2,_3);
    ReplyHandle getUserListReply = std::bind(&RestApiHandler::getUserListReplyHandle,this, _1,_2,_3);
    ReplyHandle modifyUserReply = std::bind(&RestApiHandler::modifyUserReplyHandle,this, _1,_2,_3);
    ReplyHandle signoutReply = std::bind(&RestApiHandler::signoutReplyHandle,this, _1,_2,_3);
    ReplyHandle deleteUserReply = std::bind(&RestApiHandler::deleteUserReplyHandle,this, _1,_2,_3);



    mReplyHandleMap["/zone/user/signin"] = signinReply;
    mReplyHandleMap["/zone/user/signup"] = signupReply;
    mReplyHandleMap["/zone/user/checkname"] = checkUserReply;
    mReplyHandleMap["/zone/data/getdata"] = getDataReply;
    mReplyHandleMap["/zone/data/putdata"] = putDataReply;
    mReplyHandleMap["/zone/data/getusers"] = getUserListReply;
    mReplyHandleMap["/zone/user/modify"] = modifyUserReply;
    mReplyHandleMap["/zone/user/signout"] = signoutReply;
    mReplyHandleMap["/zone/user/delete"] = deleteUserReply;

    Configuration::instance()->validateSettingsFile();

    mServerHttpsUrl = Configuration::instance()->getServerHTTPSURL();
    mServerHttpUrl = Configuration::instance()->getServerHTTPURL();



}

RestApiHandler::~RestApiHandler()
{

}

void RestApiHandler::onRequestFinished()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QNetworkRequest request = reply->request();
    QString ops = request.url().path();


    if (mReplyHandleMap.contains(ops))
    {
        if ((reply->error() == QNetworkReply::NoError) || (reply->error() > 199))
        {

            int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            QByteArray replyData = reply->readAll();
            qDebug() << replyData;
            QVariantMap jMap = JSONHandler::decodeJsonString(replyData);

            mReplyHandleMap[ops](status,reply,jMap);
        }
        else
        {
            QVariantMap tmp;
            mReplyHandleMap[ops](-1,reply,tmp);
        }
    }
    else
    {
        reply->deleteLater();
    }


}


void RestApiHandler::sendSigninRequest(const QString &userName, const QString &password)
{
    QByteArray request = JSONHandler::encodeLoginRequest(userName,password);
    QString path = "/zone/user/signin";
    QString method = "POST";
    QMap<QString,QString> query;
    this->sendRequest(path,method,request,query);

}

void RestApiHandler::sendCheckUserNameRequest(const QString &userName)
{
    QByteArray request;
    QString path = "/zone/user/checkname";
    QString method = "GET";
    QMap<QString,QString> query;
    query["user"] = userName;
    this->sendRequest(path,method,request,query);

}

void RestApiHandler::sendGetUserListRequest(const QString &user)
{
    QByteArray request;
    QString path = "/zone/data/getusers";
    QString method = "GET";
    QMap<QString,QString> query;
    query["user"] = user;
    this->sendRequest(path,method,request,query);

}

void RestApiHandler::sendSignUpRequest(const QString &user, const QString &password, int isAdmin)
{
    QByteArray request = JSONHandler::encodeSignUpRequest(user,password,isAdmin);
    QString path = "/zone/user/signup";
    QString method = "POST";
    QMap<QString,QString> query;
    this->sendRequest(path,method,request,query);
}
void RestApiHandler::sendModifyUserRequest(const QString &user, const QString &password, int isAdmin)
{
    QByteArray request = JSONHandler::encodeSignUpRequest(user,password,isAdmin);
    QString path = "/zone/user/modify";
    QString method = "POST";
    QMap<QString,QString> query;
    this->sendRequest(path,method,request,query);
}

void RestApiHandler::sendGetDataRequest(const QString &user)
{
    QByteArray request;
    QString path = "/zone/data/getdata";
    QString method = "GET";
    QMap<QString,QString> query;
    query["user"] = user;
    this->sendRequest(path,method,request,query);
}

void RestApiHandler::sendPutDataRequest(const QString &user, const QList<QList<QString> > &data)
{
    QByteArray request = JSONHandler::encodeDataUpStream(user,data);
    QString path = "/zone/data/putdata";
    QString method = "PUT";
    QMap<QString,QString> query;
    this->sendRequest(path,method,request,query);
}

void RestApiHandler::sendDeleteUserRequest(const QString &user)
{
    QByteArray request;
    QString path = "/zone/user/delete";
    QString method = "DELETE";
    QMap<QString,QString> query;
    query["user"] = user;
    this->sendRequest(path,method,request,query);
}

void RestApiHandler::sendSignoutRequest()
{
    QByteArray request;
    QString path = "/zone/user/signout";
    QString method = "POST";
    QMap<QString,QString> query;
    this->sendRequest(path,method,request,query);
}
void RestApiHandler::sendRequest(const QString &path,const QString &method,
                                 const QByteArray &requestData, const QMap<QString, QString> &_queryMap)
{
    QNetworkRequest request;
#ifdef HTTPS_SERVER
    QUrl qUrl(mServerHttpsUrl);
    qDebug() << "Client in HTTPS mode";
#else
    QUrl qUrl(mServerHttpUrl);
    qDebug() << "Client in HTTP mode";
#endif
    qUrl.setPath(path);
    request.setUrl(qUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(QString("text/plain")));
    request.setHeader(QNetworkRequest::UserAgentHeader, QVariant(QString("Zone Client, V0.1")));
    request.setRawHeader("x-user", mUser.toUtf8());
    request.setRawHeader("x-auth-token", mAuthToken.toUtf8());


    if (method == "GET")
    {
       QMap<QString, QString> queryMap = _queryMap;
       QMap<QString, QString>::iterator i;
       QUrlQuery urlQuery;
        for (i = queryMap.begin(); i != queryMap.end(); ++i)
        {
            urlQuery.addQueryItem(i.key(),i.value());
        }
        qUrl.setQuery(urlQuery);
        request.setUrl(qUrl);
        QNetworkReply *reply = mNAM->get(request);
        connect(reply,SIGNAL(finished()),this,SLOT(onRequestFinished()),Qt::QueuedConnection);

    }
    else if (method == "POST")
    {
        QNetworkReply *reply = mNAM->post(request,requestData);
        qDebug() << requestData;
        connect(reply,SIGNAL(finished()),this,SLOT(onRequestFinished()),Qt::QueuedConnection);
    }
    else if (method == "PUT")
    {
        QNetworkReply *reply = mNAM->put(request,requestData);
        connect(reply,SIGNAL(finished()),this,SLOT(onRequestFinished()),Qt::QueuedConnection);
    }
    else if (method == "DELETE")
    {
        QMap<QString, QString> queryMap = _queryMap;
        QMap<QString, QString>::iterator i;
        QUrlQuery urlQuery;
         for (i = queryMap.begin(); i != queryMap.end(); ++i)
         {
             urlQuery.addQueryItem(i.key(),i.value());
         }
         qUrl.setQuery(urlQuery);
         request.setUrl(qUrl);
        QNetworkReply *reply = mNAM->deleteResource(request);
        connect(reply,SIGNAL(finished()),this,SLOT(onRequestFinished()),Qt::QueuedConnection);
    }


}

void RestApiHandler::signInReplyHandle(int status, QNetworkReply *reply, const QVariantMap &resultMap)
{
    if (status == 200)
    {
        if (reply->hasRawHeader("x-auth-token"))
        {
            mAuthToken = reply->rawHeader("x-auth-token");
            mUser = reply->rawHeader("x-user");
            emit signInReplyReceived(true, resultMap);
        }
    }
    else
        emit signInReplyReceived(false,resultMap);
    reply->deleteLater();
}

void RestApiHandler::checkUserNameReplyHandle(int status, QNetworkReply *reply,const QVariantMap &resultMap)
{
    if (status == 200)
        emit checkUserNameReplyReceived(true, resultMap);
    else
        emit checkUserNameReplyReceived(false,resultMap);
    reply->deleteLater();
}

void RestApiHandler::signUpReplyHandle(int status, QNetworkReply *reply,const QVariantMap &resultMap)
{
    if (status == 200)
        emit signUpReplyReceived(true, resultMap);
    else
        emit signUpReplyReceived(false,resultMap);

    reply->deleteLater();
}
void RestApiHandler::getDataReplyHandle(int status, QNetworkReply *reply,const QVariantMap &resultMap)
{
    if (status == 200)
        emit getDataReplyReceived(true, resultMap);
    else
        emit getDataReplyReceived(false,resultMap);

    reply->deleteLater();
}

void RestApiHandler::putDataReplyHandle(int status, QNetworkReply *reply,const QVariantMap &resultMap)
{
    if (status == 200)
        emit putDataReplyReceived(true, resultMap);
    else
        emit putDataReplyReceived(false,resultMap);

    reply->deleteLater();
}

void RestApiHandler::getUserListReplyHandle(int status, QNetworkReply *reply,const QVariantMap &resultMap)
{
    if (status == 200)
        emit getUserListReplyReceived(true, resultMap);
    else
        emit getUserListReplyReceived(false,resultMap);

    reply->deleteLater();
}

void RestApiHandler::modifyUserReplyHandle(int status, QNetworkReply *reply,const QVariantMap &resultMap)
{
    if (status == 200)
        emit modifyUserReplyReceived(true, resultMap);
    else
        emit modifyUserReplyReceived(false,resultMap);

    reply->deleteLater();
}

void RestApiHandler::deleteUserReplyHandle(int status, QNetworkReply *reply,const QVariantMap &resultMap)
{
    if (status == 200)
        emit deleteUserReplyReceived(true, resultMap);
    else
        emit deleteUserReplyReceived(false,resultMap);

    reply->deleteLater();
}

void RestApiHandler::signoutReplyHandle(int status, QNetworkReply *reply,const QVariantMap &resultMap)
{
    if (status == 200)
    {
        //clear authentication details
        mUser = "";
        mAuthToken = "";
        emit signoutReplyReceived(true, resultMap);
    }
    else
        emit signoutReplyReceived(false,resultMap);

    reply->deleteLater();
}
