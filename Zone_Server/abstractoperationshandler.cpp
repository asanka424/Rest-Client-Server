#include "abstractoperationshandler.h"
#include "dbhandler.h"

AbstractOperationsHandler::AbstractOperationsHandler(QObject *parent) : QObject(parent)
{

}

AbstractOperationsHandler::~AbstractOperationsHandler()
{

}

bool AbstractOperationsHandler::authenticateUser(const QString &user, const QString &authToken)
{
    bool retVal = false;
    QMap<QString, QString> userDetails = DBHandler::instance()->getUserDetails(user);

    QString dbAuthToken = userDetails["auth-token"];

    if (authToken == dbAuthToken)
    {
        QString dbExpiry = userDetails["expiry"];

        QDateTime expiry = QDateTime::fromString(dbExpiry,QString("yyyy-MM-dd hh:mm:ss"));
        QDateTime current = QDateTime::currentDateTime();

        if (expiry >= current)
        {
           retVal = true;
        }
    }


    return retVal;
}
