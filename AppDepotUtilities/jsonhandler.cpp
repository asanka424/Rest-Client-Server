#include "appdepotutilities.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QVariantMap>

JSONHandler::JSONHandler(QObject *parent) : QObject(parent)
{

}

JSONHandler::~JSONHandler()
{

}

QByteArray JSONHandler::encodeLoginRequest(const QString &user, const QString &password)
{
    QJsonObject jData;
    jData["user"] = user;
    jData["password"] = password;

    QJsonDocument doc(jData);
    return doc.toJson();
}

QByteArray JSONHandler::encodeSignUpRequest(const QString &user, const QString &password, int isAdmin)
{

    QJsonObject jData;
    jData["user"] = user;
    jData["password"] = password;
    jData["is-admin"] = isAdmin;

    QJsonDocument doc(jData);
    return doc.toJson();
}
QByteArray JSONHandler::encodeUserModRequest(const QString &user, const QString &oldPassword,
                                    const QString &newPassword, int isAdmin)
{
    QJsonObject jRequest;
    jRequest["operation"] = "user-mod";
    jRequest["sub-type"] = "request";

    QJsonObject jData;
    jData["user"] = user;
    jData["old-password"] = oldPassword;
    jData["new-password"] = newPassword;
    jData["is-adming"] = isAdmin;
    jRequest["data"] = jData;

    QJsonDocument doc(jRequest);
    return doc.toJson();
}

QByteArray JSONHandler::encodeUserDelRequest(const QString &user)
{
    QJsonObject jRequest;
    jRequest["operation"] = "user-del";
    jRequest["sub-type"] = "request";

    QJsonObject jData;
    jData["user"] = user;
    jRequest["data"] = jData;

    QJsonDocument doc(jRequest);
    return doc.toJson();

}

QByteArray JSONHandler::encodeSignInReply(const QString &user, int isAdmin, const QString &msg)
{
    QJsonObject jData;
    jData["user"] = user;
    jData["is-admin"] = isAdmin;
    jData["msg"] = msg;

    QJsonDocument doc(jData);
    return doc.toJson();

}
QByteArray JSONHandler::encodeSignUpReply(const QString &msg)
{
    QJsonObject jData;
    jData["msg"] = msg;

    QJsonDocument doc(jData);
    return doc.toJson();

}

QJsonObject JSONHandler::createJSONObject(const QByteArray &data)
{
    QJsonDocument doc = QJsonDocument::fromBinaryData(data);
    return doc.object();
}

QVariantMap JSONHandler::decodeJsonString(const QByteArray &data)
{
    QVariantMap retVal;
    QJsonObject jOb;
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (!doc.isNull())
    {
        if (doc.isObject())
        {
            jOb = doc.object();
            retVal = jOb.toVariantMap();
        }
    }


    return retVal;
}

QByteArray JSONHandler::encodeError(const QByteArray &error)
{
    QJsonObject jRequest;
    jRequest["error"] = QString(error);
    QJsonDocument doc(jRequest);
    return doc.toJson();
}

QByteArray JSONHandler::encodeField(const QString &name, const QString &value)
{
    QJsonObject jRequest;
    jRequest[name] = QString(value);
    QJsonDocument doc(jRequest);
    return doc.toJson();
}

QByteArray JSONHandler::encodeStringList(const QString &name, QList<QString> &data)
{
    QJsonObject jObj;
    QJsonArray jdataArray;

    foreach (QString item, data) {
        jdataArray.append(QJsonValue(item));
    }
    jObj[name] = jdataArray;
    QJsonDocument doc(jObj);
    return doc.toJson();
}
QByteArray JSONHandler::encodeUserList(const QString &name, QList<QPair<QString, int> > &data)
{
    QJsonObject jObj;
    QJsonArray jdataArray;

    for (int i=0; i<data.size(); i++)
    {
        QPair<QString, int> row = data[i];
        QJsonArray jrow;
        jrow.append(row.first);
        jrow.append(row.second);

        jdataArray.append(jrow);
    }


    jObj[name] = jdataArray;
    QJsonDocument doc(jObj);
    return doc.toJson();
}

QByteArray JSONHandler::encodeData(const QList<QList<QString> > &data)
{
    QJsonObject jObj;
    QJsonArray jdataArray;


    for (int i=0; i<data.size(); i++)
    {
         QJsonArray jrow;
        QList<QString> row = data[i];
        for (int j=0; j<row.size(); j++)
        {
            jrow.append(QJsonValue(row[j]));
        }
        jdataArray.append(jrow);
    }
    jObj["data"] = jdataArray;

    QJsonDocument doc(jObj);
    return doc.toJson();
}

QByteArray JSONHandler::encodeDataUpStream(const QString &user, const QList<QList<QString> > &data)
{
    QJsonObject jObj;
    QJsonArray jdataArray;


    for (int i=0; i<data.size(); i++)
    {
         QJsonArray jrow;
        QList<QString> row = data[i];
        for (int j=0; j<row.size(); j++)
        {
            jrow.append(QJsonValue(row[j]));
        }
        jdataArray.append(jrow);
    }
    jObj["user"] = user;
    jObj["data"] = jdataArray;

    QJsonDocument doc(jObj);
    return doc.toJson();
}


