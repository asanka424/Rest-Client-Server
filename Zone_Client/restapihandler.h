#ifndef RESTAPIHANDLER_H
#define RESTAPIHANDLER_H

#include <QObject>
#include <QNetworkAccessManager>

#include <functional>

class RestApiHandler : public QObject
{
    Q_OBJECT
public:
    typedef std::function<void(int status, QNetworkReply *reply,const QVariantMap &resultMap)> ReplyHandle;
    explicit RestApiHandler(QObject *parent = 0);
    ~RestApiHandler();

    //Requests
    void sendSigninRequest(const QString &userName, const QString &password);
    void sendCheckUserNameRequest(const QString &userName);
    void sendSignUpRequest(const QString &user, const QString &password, int isAdmin);
    void sendGetDataRequest(const QString &user);
    void sendPutDataRequest(const QString &user, const QList<QList<QString> > &data);
    void sendGetUserListRequest(const QString &user);
    void sendModifyUserRequest(const QString &user, const QString &password, int isAdmin);
    void sendSignoutRequest();
    void sendDeleteUserRequest(const QString &user);

private:
    void sendRequest(const QString &path, const QString &method, const QByteArray &requestData, const QMap<QString, QString> &queryMap);
    void signInReplyHandle(int status, QNetworkReply *reply, const QVariantMap &resultMap);
    void checkUserNameReplyHandle(int status,  QNetworkReply *reply, const QVariantMap &resultMap);
    void signUpReplyHandle(int status, QNetworkReply *reply,const QVariantMap &resultMap);
    void getDataReplyHandle(int status, QNetworkReply *reply,const QVariantMap &resultMap);
    void putDataReplyHandle(int status, QNetworkReply *reply,const QVariantMap &resultMap);
    void getUserListReplyHandle(int status, QNetworkReply *reply,const QVariantMap &resultMap);
    void modifyUserReplyHandle(int status, QNetworkReply *reply,const QVariantMap &resultMap);
    void signoutReplyHandle(int status, QNetworkReply *reply,const QVariantMap &resultMap);
    void deleteUserReplyHandle(int status, QNetworkReply *reply,const QVariantMap &resultMap);






signals:
    void signInReplyReceived(bool,const QVariantMap &map);
    void signUpReplyReceived(bool,const QVariantMap &map);
    void checkUserNameReplyReceived(bool,const QVariantMap &map);
    void getDataReplyReceived(bool,const QVariantMap &map);
    void putDataReplyReceived(bool,const QVariantMap &map);
    void getUserListReplyReceived(bool,const QVariantMap &map);
    void modifyUserReplyReceived(bool,const QVariantMap &map);
    void signoutReplyReceived(bool,const QVariantMap &mapl);
    void deleteUserReplyReceived(bool,const QVariantMap &mapl);


public slots:
    void onRequestFinished();
private:
    QNetworkAccessManager *mNAM;
    QString mAuthToken;
    QString mUser;
    QMap<QString, ReplyHandle> mReplyHandleMap;
    QString mServerHttpUrl, mServerHttpsUrl;
};

#endif // RESTAPIHANDLER_H
