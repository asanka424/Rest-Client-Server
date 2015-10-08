#include "controller.h"

#include <mainwindow.h>
#include <signindialog.h>
#include <signupdialog.h>
#include <restapihandler.h>
#include <QVariant>
#include <QMessageBox>

Controller::Controller(QObject *parent) : QObject(parent),mStatus(0),mActiveWidget(0)
{
    mMainWindow = new MainWindow();
    mSignInDialog = new SignInDialog();
    mSignUpDialog = new SignUpDialog();
    mRestApiHandler = new RestApiHandler();


    makeConnections();
}

Controller::~Controller()
{
    mMainWindow->deleteLater();
    mSignInDialog->deleteLater();
    mSignUpDialog->deleteLater();
    mRestApiHandler->deleteLater();
}

void Controller::makeConnections()
{
    //Signin

    connect(mSignInDialog,SIGNAL(signInInvoked(QString,QString)),this,SLOT(onSignInInvoked(QString,QString)));
    connect(mSignInDialog,SIGNAL(signUpInvoked()),this,SLOT(onSignUpInvoked()));
    connect(mRestApiHandler,SIGNAL(signInReplyReceived(bool,QVariantMap)),this,SLOT(onSignInReplyReceived(bool,QVariantMap)));

    //Signup

    connect(mSignUpDialog,SIGNAL(signUpInvoked(QString,QString,int)),this,SLOT(onSignUpOperationInvoked(QString,QString,int)));
    connect(mSignUpDialog,SIGNAL(signInClicked()),this,SLOT(onSignInClickedReceived()));
    connect(mSignUpDialog,SIGNAL(checkNameInvoked(QString)),SLOT(onCheckNameInvoked(QString)));

    //RestAPI
    connect(mRestApiHandler,SIGNAL(signUpReplyReceived(bool,QVariantMap)),this,SLOT(onSignUpReplyReceived(bool,QVariantMap)));
    connect(mRestApiHandler,SIGNAL(checkUserNameReplyReceived(bool,QVariantMap)),this,SLOT(onCheckNameReplyReceived(bool,QVariantMap)));
    connect(mRestApiHandler,SIGNAL(getDataReplyReceived(bool,QVariantMap)),mMainWindow,SLOT(onGetDataReplyReceived(bool,QVariantMap)));
    connect(mRestApiHandler,SIGNAL(putDataReplyReceived(bool,QVariantMap)),mMainWindow,SLOT(onPutDataReplyReceived(bool,QVariantMap)));
    connect(mRestApiHandler,SIGNAL(signoutReplyReceived(bool,QVariantMap)),this,SLOT(onSignoutReplyReceived(bool,QVariantMap)));
    connect(mRestApiHandler,SIGNAL(deleteUserReplyReceived(bool,QVariantMap)),mMainWindow,SLOT(onDeleteUserReplyReceived(bool,QVariantMap)));



    //connect(mRestApiHandler,SIGNAL(replyReceived(int,QVariantMap)),this,SLOT(onSignInReplyReceived(bool,QVariantMap)),Qt::QueuedConnection);


    connect(mMainWindow,SIGNAL(putDataInvoked(QString,QList<QList<QString> >)),this,SLOT(onPutDataInvoked(QString,QList<QList<QString> >)));
    connect(mMainWindow,SIGNAL(modifyUserInvoked(QString,QString,int)),this,SLOT(onModifyUserInvoked(QString,QString,int)));
    connect(mRestApiHandler,SIGNAL(modifyUserReplyReceived(bool,QVariantMap)),mMainWindow,SIGNAL(modifyUserReplyReceived(bool,QVariantMap)));
    connect(mMainWindow,SIGNAL(getUserListInvoked(QString)),this,SLOT(onGetUserListInvoked(QString)));
    connect(mRestApiHandler,SIGNAL(getUserListReplyReceived(bool,QVariantMap)),mMainWindow,SIGNAL(getUserListReplyReceived(bool,QVariantMap)));
    connect(mMainWindow,SIGNAL(deleteUserInvoked(QString)),this,SLOT(onDeleteUserInvoked(QString)));
    connect(mMainWindow,SIGNAL(signout()),this,SLOT(onSignOutInvoked()));
    connect(mMainWindow,SIGNAL(ownUserDeleted()),this,SLOT(onOwnUserDeleted()));
}
void Controller::widgetClosed()
{
    exit(0);
}

void Controller::onSignInInvoked(const QString &user, const QString &password)
{
    mSignInDialog->setEnabled(false);
    mRestApiHandler->sendSigninRequest(user,password);
}

//void Controller::onReplyReceived(int status, const QVariantMap &jMap)
//{
//    QString operation = jMap["operation"].toString();
//}

void Controller::updateState(int newState)
{
    switch (newState)
    {
    case 0: // initial
        mMainWindow->hide();
        mMainWindow->clearFields();
        mSignUpDialog->hide();
        mSignUpDialog->clearFields();
        mSignInDialog->show();
        mSignInDialog->clearFields();
        mActiveWidget = mSignInDialog;
        break;
    case 1: //sign up
        mMainWindow->hide();
        mMainWindow->clearFields();
        mSignUpDialog->show();
        mSignInDialog->hide();
        mSignInDialog->clearFields();
        mActiveWidget = mSignUpDialog;
        break;
    case 2: //logged in
        mMainWindow->show();
        mSignUpDialog->hide();
        mSignInDialog->hide();
        mMainWindow->clearFields();
        mActiveWidget = mMainWindow;
        break;
    default:
        break;
    }
    mActiveWidget->setEnabled(true);

}

void Controller::onSignUpInvoked()
{
    updateState(1);
}

void Controller::onSignUpOperationInvoked(const QString &user, const QString &pass, int isAdmin)
{
    mSignUpDialog->setEnabled(false);
    mRestApiHandler->sendSignUpRequest(user,pass,isAdmin);
}

void Controller::onSignInClickedReceived()
{
    updateState(0);
}

void Controller::onSignUpReplyReceived(bool status, const QVariantMap &replyMap)
{
    if (!status)
    {
        QString error;
        if (replyMap.contains("error"))
        {
            error = replyMap["error"].toString();

        }
        else
            error = "Error while sign up for a new account";

        QMessageBox::critical(mSignUpDialog,tr("Error"),error);
        mSignUpDialog->clearFields();
        updateState(1);

    }
    else
    {
        QString info = replyMap["msg"].toString();
        QMessageBox::information(mSignUpDialog,tr("New User"),info);
        updateState(0);
    }
}

void Controller::onSignInReplyReceived(bool status, const QVariantMap &replyMap)
{
    if (!status)
    {
        QString error;
        if (replyMap.contains("error"))
        {
            error = replyMap["error"].toString();

        }
        else
            error = "Error while sign in";

        QMessageBox::critical(mSignUpDialog,tr("Error"),error);
        updateState(0);
    }
    else
    {
            QString info = replyMap["msg"].toString();
            QMessageBox::information(mSignUpDialog,tr("Sign In"),info);
            QString user = replyMap["user"].toString();
            updateState(2);
            mMainWindow->init(user);
            mMainWindow->setEnabled(false);
            mRestApiHandler->sendGetDataRequest(user);

    }
}


void Controller::onCheckNameInvoked(const QString &user)
{
    mRestApiHandler->sendCheckUserNameRequest(user);
}

void Controller::onCheckNameReplyReceived(bool status, const QVariantMap &replyMap)
{
    QString msg;
    if (!status)
        msg = replyMap["error"].toString();
    else
        msg = "User name available";

    QMessageBox::warning(mSignUpDialog,tr("User"),msg);
}

void Controller::onLoadDataInvoked()
{

}

void Controller::onPutDataInvoked(const QString &user, const QList<QList<QString> > &data)
{

    mRestApiHandler->sendPutDataRequest(user,data);
}

void Controller::onGetUserListInvoked(const QString &user)
{
    mRestApiHandler->sendGetUserListRequest(user);
}

void Controller::onModifyUserInvoked(const QString &user, const QString &password, int isAdmin)
{
    mRestApiHandler->sendModifyUserRequest(user,password,isAdmin);
}

void Controller::onSignOutInvoked()
{
    mRestApiHandler->sendSignoutRequest();
}

void Controller::onSignoutReplyReceived(bool status, const QVariantMap &jMap)
{
    if (status)
    {
        updateState(0);
        mMainWindow->clearFields();
    }
    else
    {
        QMessageBox::warning(mMainWindow,tr("Sign out"),jMap["error"].toString());
    }
}



void Controller::onOwnUserDeleted()
{
    updateState(0);
    mMainWindow->clearFields();
}

void Controller::onDeleteUserInvoked(const QString &user)
{
    mRestApiHandler->sendDeleteUserRequest(user);
}


