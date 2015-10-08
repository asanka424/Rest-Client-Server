#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <QObject>

#include <abstractoperationshandler.h>
#include <functional>
#include <QMap>


class MyHttpServer;


class DataHandler : public AbstractOperationsHandler
{
    Q_OBJECT

 public:
    typedef std::function<void(QString &method,QByteArray &requestData, QMap<QString, QString> &queryMap, const QString &reqUser, Tufao::HttpServerResponse &response)> DataOperation;
     explicit DataHandler( QObject *parent = 0);
    ~DataHandler();
    void handleOperations(QStringList &operations, QString &method, QByteArray &requestData, QMap<QString, QString> &queryMap,
                          QMap<QString,QString> &headerMap, Tufao::HttpServerResponse &response);

private:
    void getData(QString &method,QByteArray &requestData, QMap<QString, QString> &queryMap, const QString &reqUser, Tufao::HttpServerResponse &response);
    void putData(QString &method,QByteArray &requestData, QMap<QString, QString> &queryMap, const QString &reqUser, Tufao::HttpServerResponse &response);
    void getUsers(QString &method,QByteArray &requestData, QMap<QString, QString> &queryMap,const QString &reqUser, Tufao::HttpServerResponse &response);
    //bool authenticateUser(const QString &user, const QString &authToken);



private:
    QMap<QString, DataOperation>  mOperations;
};

#endif // DATAHANDLER_H
