#include "signindialog.h"
#include "ui_signindialog.h"
#include <QMessageBox>

SignInDialog::SignInDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SignInDialog)
{
    ui->setupUi(this);
    makeConnections();
}

SignInDialog::~SignInDialog()
{
    delete ui;
}

void SignInDialog::makeConnections()
{
    connect(ui->btn_cancel,SIGNAL(clicked()),this,SLOT(onCancelClicked()));
    connect(ui->btn_signIn,SIGNAL(clicked()),this,SLOT(onSignInClicked()));
    connect(ui->lbl_signUp,SIGNAL(linkActivated(QString)),this,SLOT(onSignUpClicked()));
}

void SignInDialog::onSignInClicked()
{
    // check user and paasword
    if ((ui->le_userName->text().isEmpty()) || (ui->le_password->text().isEmpty()))
    {
        QMessageBox::critical(this,tr("Error"),tr("Either User Name and Password cannot be empty"));
    }
    else
    {
        emit signInInvoked(ui->le_userName->text(),ui->le_password->text());
    }

}

void SignInDialog::onCancelClicked()
{
    this->close();
}

void SignInDialog::onSignUpClicked()
{
    emit signUpInvoked();
}
void SignInDialog::clearFields()
{
    ui->le_password->clear();
    ui->le_password->clear();
}
