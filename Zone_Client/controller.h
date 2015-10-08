#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

class MainWindow;
class SignInDialog;
class SignUpDialog;
class RequestManager;
class RestApiHandler;

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = 0);
    ~Controller();
    void updateState(int newState);
    void startExecution(){this->updateState(0);}
private:
    void makeConnections();

signals:

public slots:
    //void onupdateState(int newState);
    void onSignUpReplyReceived(bool status, const QVariantMap &replyMap);
    void onSignInReplyReceived(bool status, const QVariantMap &replyMap);
    void onCheckNameReplyReceived(bool status, const QVariantMap &replyMap);

    void onGetUserListInvoked(const QString &user);
    void onModifyUserInvoked(const QString &user, const QString &password, int isAdmin);

    void onLoadDataInvoked();

    void onDeleteUserInvoked(const QString &user);
    //void onReplyReceived(int status, const QVariantMap &jMap);
    void widgetClosed();
    void onSignInInvoked(const QString &user, const QString &password);
    void onSignUpInvoked();
    void onSignUpOperationInvoked(const QString &user, const QString &pass, int isAdmin);
    void onSignInClickedReceived();

    void onPutDataInvoked(const QString &user, const QList<QList<QString> > &data);


    void onCheckNameInvoked(const QString &user);

    void onSignOutInvoked();
    void onSignoutReplyReceived(bool status, const QVariantMap &jMap);
    void onOwnUserDeleted();


private:
    int mStatus; //0 -> initial, 1 -> signup, 2 ->  logged
    MainWindow *mMainWindow;
    SignInDialog *mSignInDialog;
    SignUpDialog *mSignUpDialog;
    RequestManager *mRequestManager;
    RestApiHandler *mRestApiHandler;
    QWidget *mActiveWidget;
};

#endif // CONTROLLER_H
