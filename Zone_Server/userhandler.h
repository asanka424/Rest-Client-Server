#ifndef USERHANDLER_H
#define USERHANDLER_H

#include <functional>

#include <QObject>
#include "abstractoperationshandler.h"
#include <QMap>

class UserHandler : public AbstractOperationsHandler
{
    Q_OBJECT

 public:
     explicit UserHandler(QObject *parent = 0);
    ~UserHandler();
    void handleOperations(QStringList &operations, QString &method, QByteArray &requestData, QMap<QString, QString> &queryMap,
                          QMap<QString,QString> &headerMap, Tufao::HttpServerResponse &response);
    void loadOperations();
private:
    void signIn(QString &method,QByteArray &requestData, QMap<QString, QString> &queryMap,
                QMap<QString, QString> &headerMap, Tufao::HttpServerResponse &response);
    void signUp(QString &method,QByteArray &requestData, QMap<QString, QString> &queryMap,
                QMap<QString, QString> &headerMap, Tufao::HttpServerResponse &response);
    void userList(QString &method,QByteArray &requestData, QMap<QString, QString> &queryMap,
                QMap<QString, QString> &headerMap, Tufao::HttpServerResponse &response);
    void userMod(QString &method,QByteArray &requestData, QMap<QString, QString> &queryMap,
                QMap<QString, QString> &headerMap, Tufao::HttpServerResponse &response);
    void userDel(QString &method,QByteArray &requestData, QMap<QString, QString> &queryMap,
                QMap<QString, QString> &headerMap, Tufao::HttpServerResponse &response);
    void userSignout(QString &method,QByteArray &requestData, QMap<QString, QString> &queryMap,
                QMap<QString, QString> &headerMap, Tufao::HttpServerResponse &response);
    void checkUser(QString &method,QByteArray &requestData, QMap<QString, QString> &queryMap,
                QMap<QString, QString> &headerMap, Tufao::HttpServerResponse &response);
    QString getRandomString(int) const;


private:
    QMap<QString, AbstractOperationsHandler::Operation >  mOperations;

};



#endif // USERHANDLER_H
