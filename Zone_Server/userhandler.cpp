#include "userhandler.h"
#include "myhttpserver.h"
#include "httpserverrequest.h"
#include "httpserverresponse.h"
#include "headers.h"
#include "dbhandler.h"
#include "appdepotutilities.h"
#include <QCryptographicHash>

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;
using std::placeholders::_4;
using std::placeholders::_5;

UserHandler::UserHandler(QObject *parent)
    :   AbstractOperationsHandler(parent)
{
    AbstractOperationsHandler::Operation signInOp = std::bind(&UserHandler::signIn, this, _1,_2,_3,_4,_5);
    AbstractOperationsHandler::Operation signUpOp = std::bind(&UserHandler::signUp, this, _1, _2, _3, _4,_5);
    AbstractOperationsHandler::Operation userListOp = std::bind(&UserHandler::userList, this, _1, _2, _3, _4,_5);
    AbstractOperationsHandler::Operation userDelOp = std::bind(&UserHandler::userDel, this, _1, _2, _3, _4,_5);
    AbstractOperationsHandler::Operation userModOp = std::bind(&UserHandler::userMod, this, _1, _2, _3, _4,_5);
    AbstractOperationsHandler::Operation checkUserOp = std::bind(&UserHandler::checkUser,this, _1, _2, _3, _4, _5);
    AbstractOperationsHandler::Operation signoutOp = std::bind(&UserHandler::userSignout,this, _1, _2, _3, _4, _5);


    mOperations["signin"] = signInOp;
    mOperations["signup"] = signUpOp;
    mOperations["userlist"] = userListOp;
    mOperations["delete"] = userDelOp;
    mOperations["modify"] = userModOp;
    mOperations["checkname"] = checkUserOp;
    mOperations["signout"] = signoutOp;

    qsrand(qrand());
}

void UserHandler::loadOperations()
{

}

UserHandler::~UserHandler()
{

}

void UserHandler::handleOperations(QStringList &operations, QString &method, QByteArray &requestData, QMap<QString, QString> &queryMap,
                                   QMap<QString,QString> &headerMap, Tufao::HttpServerResponse &response)
{
    if (operations.size() == 1)
    {
        QString operation = operations[0];
        if (mOperations.contains(operation))
            mOperations[operation](method,requestData,queryMap,headerMap,response);
        else
        {
            response.writeHead(Tufao::HttpResponseStatus::NOT_IMPLEMENTED);
            response.end(JSONHandler::encodeError(OPERATION_NOT_IMPLEMENTED));
        }
    }
    else
    {
        response.writeHead(Tufao::HttpResponseStatus::BAD_REQUEST);
        response.end(JSONHandler::encodeError(INVALID_REQUEST));
    }
}

void UserHandler::signIn(QString &method,QByteArray &requestData, QMap<QString, QString> &queryMap,
                         QMap<QString,QString> &headerMap, Tufao::HttpServerResponse &response)
{
    //signin is a post operation
    if (method != "POST")
    {
        response.writeHead(Tufao::HttpResponseStatus::BAD_REQUEST);
        response.end(JSONHandler::encodeError(INVALID_METHOD));
    }
    else
    {
        QVariantMap requestMap = JSONHandler::decodeJsonString(requestData);
        if ((requestMap.contains("user")) && (requestMap.contains("password")))
        {
            QString user = requestMap["user"].toString();
            QString password = requestMap["password"].toString();

            QPair<QString, QString> userAuth = DBHandler::instance()->getPassword(QString(user));
            QString salt = userAuth.first;
            QString dbPass = userAuth.second;

            QString encodedPass = QString(QCryptographicHash::hash((salt+password).toUtf8(),QCryptographicHash::Md5).toHex());
            if (dbPass == encodedPass)
            {
                //sign in successfull.
                QString authToken = getRandomString(16);
                QMap<QString, QString> userDetails = DBHandler::instance()->getUserDetails(user);
                int isAdmin = (userDetails["is-Admin"] == "1")?1:0;
                if (DBHandler::instance()->updateAuthToken(authToken,user))
                {
                    Tufao::Headers headers;
                    headers.insert("x-auth-token",authToken.toUtf8());
                    headers.insert("x-user",user.toUtf8());
                    response.writeHead(Tufao::HttpResponseStatus::OK, headers);
                    QString msg = "Sign in successfull";
                    QByteArray replyData = JSONHandler::encodeSignInReply(user,isAdmin,msg);
                    response.end(replyData);
                }
                else
                {
                    response.writeHead(Tufao::HttpResponseStatus::INTERNAL_SERVER_ERROR);
                    response.end(JSONHandler::encodeError("Internal Server Error"));
                }

            }
            else
            {
                response.writeHead(Tufao::HttpResponseStatus::BAD_REQUEST);
                response.end(JSONHandler::encodeError("Authentication Failed"));
            }

        }
        else
        {
            response.writeHead(Tufao::HttpResponseStatus::EXPECTATION_FAILED);
            response.end(JSONHandler::encodeError(PARAMETERS_NOT_FOUND));

        }

    }

}

void UserHandler::signUp(QString &method, QByteArray &requestData, QMap<QString, QString> &queryMap,
                         QMap<QString,QString> &headerMap, Tufao::HttpServerResponse &response)
{
    //signup is a post operation
    if (method != "POST")
    {
        response.writeHead(Tufao::HttpResponseStatus::BAD_REQUEST);
        response.end(JSONHandler::encodeError(INVALID_METHOD));
    }
    else
    {
        QVariantMap requestMap = JSONHandler::decodeJsonString(requestData);
        if ((requestMap.contains("user")) && (requestMap.contains("password")))
        {
            QString user = requestMap["user"].toString();
            QString password = requestMap["password"].toString();
            int isAdmin = requestMap["is-admin"].toInt();

            //create salt
            QString salt = getRandomString(12);
            QString authtoken = getRandomString(16);
            QString encodedPass = QString(QCryptographicHash::hash((salt+password).toUtf8(),QCryptographicHash::Md5).toHex());
            if (DBHandler::instance()->checkUser(user))
            {
                //user exist
                QByteArray replyData = JSONHandler::encodeError("User exits");
                response.writeHead(Tufao::HttpResponseStatus::CONFLICT);
                response.end(replyData);
            }
            else
            {
                if (DBHandler::instance()->insertNewUser(authtoken,user,encodedPass,salt,QString::number(isAdmin)))
                {
                    QString msg;
                    if (isAdmin == 1)
                        msg = "Your signup request has been received. You will be notified after your Account is setup";
                    else
                        msg = "Signup is successfull. You can Log in now";

                    QByteArray data = JSONHandler::encodeSignUpReply(msg);
                    response.writeHead(Tufao::HttpResponseStatus::OK);
                    response.end(data);
                }
                else
                {
                    response.writeHead(Tufao::HttpResponseStatus::INTERNAL_SERVER_ERROR);
                    response.end(JSONHandler::encodeError(INTERNAL_ERROR));
                }
            }
        }
        else
        {
            response.writeHead(Tufao::HttpResponseStatus::EXPECTATION_FAILED);
            response.end(JSONHandler::encodeError(PARAMETERS_NOT_FOUND));

        }

    }

}
void UserHandler::checkUser(QString &method, QByteArray &requestData, QMap<QString, QString> &queryMap, QMap<QString, QString> &headerMap, Tufao::HttpServerResponse &response)
{
    //only works for GET
    if (method != "GET")
    {
        response.writeHead(Tufao::HttpResponseStatus::BAD_REQUEST);
        response.end(JSONHandler::encodeError(INVALID_METHOD));
    }
    else
    {
        if (queryMap.contains("user"))
        {
            QString user = queryMap["user"];
            bool exits = DBHandler::instance()->checkUser(user);
            if (exits)
            {
                response.writeHead(Tufao::HttpResponseStatus::CONFLICT);
                response.end(JSONHandler::encodeError("User Exists"));
            }
            else
            {
                response.writeHead(Tufao::HttpResponseStatus::OK);
                response.end();
            }
        }
        else
        {
            response.writeHead(Tufao::HttpResponseStatus::BAD_REQUEST);
            response.end(JSONHandler::encodeError("Request format error"));
        }
    }
}

void UserHandler::userList(QString &method,QByteArray &requestData, QMap<QString, QString> &queryMap,
                           QMap<QString,QString> &headerMap, Tufao::HttpServerResponse &respons)
{

}
void UserHandler::userMod(QString &method,QByteArray &requestData, QMap<QString, QString> &queryMap,
                          QMap<QString,QString> &headerMap, Tufao::HttpServerResponse &response)
{
    if (method == "POST")
        if ((headerMap.contains("x-user")) && (headerMap.contains("x-auth-token")))
        {
            QString user = headerMap["x-user"];
            QString authToken = headerMap["x-auth-token"];

            if (AbstractOperationsHandler::authenticateUser(user,authToken))
            {
                QMap<QString, QString> userDetails = DBHandler::instance()->getUserDetails(user);
                int isAdmin = userDetails["is-admin"].toInt();
                QString dbPassword = userDetails["password"];

                QVariantMap requestMap = JSONHandler::decodeJsonString(requestData);
                if ((requestMap.contains("user")) && (requestMap.contains("password")) && (requestMap.contains("is-admin")))
                {
                    QString reqUser = requestMap["user"].toString();
                    QString reqPassword = requestMap["password"].toString();
                    int reqIsAdmin = requestMap["is-admin"].toInt();

                    if (reqPassword.isEmpty()) reqPassword = dbPassword;

                    if ((reqUser == user) || (isAdmin == 1))
                    {
                        QString salt = getRandomString(12);
                        QString authtoken = getRandomString(16);
                        QString encodedPass = QString(QCryptographicHash::hash((salt+reqPassword).toUtf8(),QCryptographicHash::Md5).toHex());

                        if (DBHandler::instance()->modifyUser(authtoken,reqUser,encodedPass,salt,QString::number(reqIsAdmin)))
                        {
                            response.writeHead(Tufao::HttpResponseStatus::OK);
                            response.end(JSONHandler::encodeField("user",reqUser));
                        }
                        else
                        {
                            response.writeHead(Tufao::HttpResponseStatus::INTERNAL_SERVER_ERROR);
                            response.end(JSONHandler::encodeError("Internal Server Error"));
                        }

                    }
                    else
                    {
                        response.writeHead(Tufao::HttpResponseStatus::FORBIDDEN);
                        response.end(JSONHandler::encodeError("Request format error"));
                    }


                }
                else
                {
                    response.writeHead(Tufao::HttpResponseStatus::BAD_REQUEST);
                    response.end(JSONHandler::encodeError("Request format error"));
                }
            }
            else
            {
                response.writeHead(Tufao::HttpResponseStatus::FORBIDDEN);
                response.end(JSONHandler::encodeError("Request format error"));
            }
        }
        else
        {
            response.writeHead(Tufao::HttpResponseStatus::BAD_REQUEST);
            response.end(JSONHandler::encodeError("Request format error"));
        }
    else
    {
        response.writeHead(Tufao::HttpResponseStatus::NOT_FOUND);
        response.end(JSONHandler::encodeError("Request format error"));
    }
}


void UserHandler::userSignout(QString &method,QByteArray &requestData, QMap<QString, QString> &queryMap,
                          QMap<QString,QString> &headerMap, Tufao::HttpServerResponse &response)
{
    if (method == "POST")
        if ((headerMap.contains("x-user")) && (headerMap.contains("x-auth-token")))
        {
            QString user = headerMap["x-user"];
            QString authToken = headerMap["x-auth-token"];

            if (AbstractOperationsHandler::authenticateUser(user,authToken))
            {


                if (DBHandler::instance()->updateAuthTokenExpiry(user))
                {
                    response.writeHead(Tufao::HttpResponseStatus::OK);
                    response.end(JSONHandler::encodeField("message","Successfully signed out"));
                }
                else
                {
                    response.writeHead(Tufao::HttpResponseStatus::INTERNAL_SERVER_ERROR);
                    response.end(JSONHandler::encodeError("Internal Server Error"));
                }
            }
            else
            {
                response.writeHead(Tufao::HttpResponseStatus::FORBIDDEN);
                response.end(JSONHandler::encodeError("Illegal operation"));
            }
        }
        else
        {
            response.writeHead(Tufao::HttpResponseStatus::BAD_REQUEST);
            response.end(JSONHandler::encodeError("Request format error"));
        }
    else
    {
        response.writeHead(Tufao::HttpResponseStatus::NOT_FOUND);
        response.end(JSONHandler::encodeError("Operation not found"));
    }
}

void UserHandler::userDel(QString &method,QByteArray &requestData, QMap<QString, QString> &queryMap,
                          QMap<QString,QString> &headerMap, Tufao::HttpServerResponse &response)
{
    if (method != "DELETE")
    {
        response.writeHead(Tufao::HttpResponseStatus::NOT_FOUND);
        response.end(JSONHandler::encodeError("Operation not found"));
    }
    else
    {
        if ((headerMap.contains("x-user")) && (headerMap.contains("x-auth-token")))
        {
            QString user = headerMap["x-user"];
            QString authToken = headerMap["x-auth-token"];

            if (AbstractOperationsHandler::authenticateUser(user,authToken))
            {

                if (queryMap.contains("user"))
                {
                    QString reqUser = queryMap["user"];
                    if (DBHandler::instance()->deleteUser(reqUser))
                    {
                        response.writeHead(Tufao::HttpResponseStatus::OK);
                        response.end(JSONHandler::encodeField("user",reqUser));
                    }
                    else
                    {
                        response.writeHead(Tufao::HttpResponseStatus::INTERNAL_SERVER_ERROR);
                        response.end(JSONHandler::encodeError("Internal Server Error"));
                    }
                }
                else
                {
                    response.writeHead(Tufao::HttpResponseStatus::BAD_REQUEST);
                    response.end(JSONHandler::encodeError("Request format error"));
                }

            }
            else
            {
                response.writeHead(Tufao::HttpResponseStatus::FORBIDDEN);
                response.end(JSONHandler::encodeError("Illegal operation"));
            }
        }
        else
        {
            response.writeHead(Tufao::HttpResponseStatus::BAD_REQUEST);
            response.end(JSONHandler::encodeError("Request format error"));
        }
    }
}
QString UserHandler::getRandomString(int len) const
{
   const QString possibleCharacters("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
   const int randomStringLength = len;

   QString randomString;
   for(int i=0; i<randomStringLength; ++i)
   {
       int index = qrand() % possibleCharacters.length();
       QChar nextChar = possibleCharacters.at(index);
       randomString.append(nextChar);
   }
   return randomString;
}
