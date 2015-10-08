#ifndef ABSTRACTOPERATIONSHANDLER_H
#define ABSTRACTOPERATIONSHANDLER_H

#include <QObject>
#include <functional>

#include "httpserverrequest.h"
#include "httpserverresponse.h"

#define OPERATION_NOT_IMPLEMENTED "Operation Not Implemented"
#define INVALID_METHOD "Invalid Method"
#define PARAMETERS_NOT_FOUND "Expected Parameters Not Found"
#define INTERNAL_ERROR "Internal Error"

class AbstractOperationsHandler : public QObject
{
    Q_OBJECT
public:
    typedef std::function<void(QString &,QByteArray&,
                        QMap<QString, QString>&, QMap<QString,QString>&,
                        Tufao::HttpServerResponse&)> Operation;

    explicit AbstractOperationsHandler(QObject *parent = 0);
    ~AbstractOperationsHandler();
    virtual void handleOperations(QStringList &operations, QString &method, QByteArray &requestData, QMap<QString, QString> &queryMap,
                                  QMap<QString,QString> &headerMap, Tufao::HttpServerResponse &response) = 0;

    static bool authenticateUser(const QString &user, const QString &authToken);

signals:

public slots:
};

#endif // ABSTRACTOPERATIONSHANDLER_H
