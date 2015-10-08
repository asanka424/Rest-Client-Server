#include "usermod.h"
#include "ui_usermod.h"
#include <QMessageBox>
#include <QVariant>
#include <QCloseEvent>

UserMod::UserMod(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserMod)
{
    ui->setupUi(this);
    makeConnections();
}

UserMod::~UserMod()
{
    delete ui;
}

void UserMod::makeConnections()
{
    connect(ui->btn_cancel,SIGNAL(clicked()),this,SLOT(onCancelClicked()));
    connect(ui->btn_modify,SIGNAL(clicked()),this,SLOT(onModifyClicked()));
    connect(ui->btn_delete,SIGNAL(clicked()),this,SLOT(onDeleteClicked()));
    connect(ui->cb_user,SIGNAL(currentIndexChanged(QString)),this,SLOT(onUserSelected()));
}

void UserMod::init(const QString &user)
{
    mUser = user;
    ui->cb_user->clear();
    ui->le_newPassword->clear();
    ui->le_retype->clear();
    emit getUserListInvoked(mUser);
    this->setEnabled(false);
}

void UserMod::onModifyClicked()
{
    int isAdmin = (ui->chkbox_admin->isChecked())? 1:0;
    if (!ui->le_newPassword->text().isEmpty())
        if (!(ui->le_newPassword->text() == ui->le_retype->text()))
        {
            QMessageBox::critical(this,tr("Error"),tr("Passwords must match"));
        }
        else
        {

            emit modifyUserInvoked(ui->cb_user->currentText(),ui->le_newPassword->text(),isAdmin);
            this->setEnabled(false);
        }
    else
    {
        if (isAdmin == mUserMap[ui->cb_user->currentText()])
            QMessageBox::information(this,tr("Info"),tr("Nothing to modilfy"));
        else
        {
            emit modifyUserInvoked(ui->cb_user->currentText(),ui->le_newPassword->text(),isAdmin);
            this->setEnabled(false);
        }
    }
}
void UserMod::onDeleteClicked()
{
    QMessageBox::StandardButton ans ;
    if (ui->cb_user->currentText() == mUser)
    {
        ans = QMessageBox::question(this,tr("Warning"), tr("You are trying to delete your account. Are you sure?"),QMessageBox::Yes|QMessageBox::No);
    }
    else
    {
        ans = QMessageBox::question(this,tr("Warning"), tr("You are about to delete ") + ui->cb_user->currentText() + tr(". Are you sure?"),QMessageBox::Yes|QMessageBox::No);

    }
    if (ans == QMessageBox::Yes)
        emit deleteUserInvoked(ui->cb_user->currentText());
    this->setEnabled(false);
}

void UserMod::onGetUserListResultReceived(bool status, const QVariantMap &data)
{
    ui->chkbox_admin->hide();
    if (status)
    {
        ui->cb_user->clear();
        mUserMap.clear();
        QList<QVariant> userList = data["users"].toList();
        for (int i=0; i<userList.size(); i++)
        {
            QVariantList item = userList[i].toList();
            ui->cb_user->addItem(item[0].toString());
            mUserMap[item[0].toString()] = item[1].toInt();
        }
        ui->cb_user->setCurrentText(mUser);
        this->onUserSelected();
        this->setEnabled(true);
    }
    else
    {
        QMessageBox::critical(this,tr("Error"), tr("Error while retrieving users"));
        this->hide();
    }
    if (mUserMap[mUser] == 1)
        ui->chkbox_admin->show();

}

void UserMod::onModifyUserResultReceived(bool status, const QVariantMap &data)
{
    this->setEnabled(true);
    if (status)
    {
        QString user = data["user"].toString();
        if (user == mUser)
        {
            QMessageBox::information(this,tr("User Modified"),tr("You have successfully modified your login. Please sign in Again."));
            //sign out
        }
        else
        {
            QMessageBox::information(this,tr("User Modified"),tr("User succssfully modified"));
            this->init(mUser);
        }


    }
    else
    {
        QMessageBox::critical(this,tr("Errir"),data["error"].toString());
    }
}

void UserMod::onCancelClicked()
{
    this->hide();
}

void UserMod::onUserSelected()
{
    QString user = ui->cb_user->currentText();
    ui->chkbox_admin->setChecked((mUserMap[user] == 1));
}

void UserMod::closeEvent(QCloseEvent *ev)
{
    this->hide();

    ev->ignore();
}
