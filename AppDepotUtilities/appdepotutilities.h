#ifndef APPDEPOTUTILITIES_H
#define APPDEPOTUTILITIES_H

#include "appdepotutilities_global.h"
#include <QObject>

class APPDEPOTUTILITIESSHARED_EXPORT AppDepotUtilities
{

public:
    AppDepotUtilities();
};

class APPDEPOTUTILITIESSHARED_EXPORT JSONHandler : public QObject
{
    Q_OBJECT
public:
    explicit JSONHandler(QObject *parent = 0);
    ~JSONHandler();
    static QByteArray encodeLoginRequest(const QString &user, const QString &password);
    static QByteArray encodeSignUpRequest(const QString &user, const QString &password, int isAdmin = 0);
    static QByteArray encodeUserModRequest(const QString &user, const QString &oldPassword,
                                        const QString &newPassword, int isAdmin = 0);
    static QByteArray encodeUserDelRequest(const QString &user);

    static QJsonObject createJSONObject(const QByteArray &data);

    static QVariantMap decodeJsonString(const QByteArray &data);
    static QByteArray encodeError(const QByteArray& error);
    static QByteArray encodeSignInReply(const QString &user, int isAdmin, const QString &msg);
    static QByteArray encodeSignUpReply(const QString &msg);

    static QByteArray encodeData(const QList<QList<QString> > &data);
    static QByteArray encodeDataUpStream(const QString &user, const QList<QList<QString> > &data);

    static QByteArray encodeStringList(const QString &name, QList<QString> &data);
    static QByteArray encodeUserList(const QString &name, QList<QPair<QString, int> > &data);

    static QByteArray encodeField(const QString &name, const QString &value);





signals:

public slots:
};

#endif // APPDEPOTUTILITIES_H
