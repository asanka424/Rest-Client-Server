#include "signupdialog.h"
#include "ui_signupdialog.h"
#include <QMessageBox>

SignUpDialog::SignUpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SignUpDialog)
{
    ui->setupUi(this);
    makeConnections();
}

SignUpDialog::~SignUpDialog()
{
    delete ui;
}

void SignUpDialog::makeConnections()
{
    connect(ui->btn_cancel,SIGNAL(clicked()),this, SLOT(onCancelClicked()));
    connect(ui->btn_checkName,SIGNAL(clicked()),this,SLOT(onCheckNameClicked()));
    connect(ui->btn_signUp,SIGNAL(clicked()),this,SLOT(onSignUpClicked()));
    connect(ui->lbl_signIn,SIGNAL(linkActivated(QString)),this,SLOT(onSignInClicked()));
}

void SignUpDialog::onSignUpClicked()
{
    // check user and paasword
    if ((ui->le_userName->text().isEmpty()) || (ui->le_password1->text().isEmpty()))
    {
        QMessageBox::critical(this,tr("Error"),tr("Either User Name and Password cannot be empty"));
    }
    else
    {
        if (ui->le_password1->text() == ui->le_password2->text())
            emit signUpInvoked(ui->le_userName->text(),ui->le_password1->text(), (ui->chkbox_adming->isChecked()?1:0));
        else
        {
            QMessageBox::critical(this,tr("Error"),tr("Passwords do not match"));
        }
    }
}

void SignUpDialog::onCancelClicked()
{
    this->close();
}

void SignUpDialog::onCheckNameClicked()
{
    if (!ui->le_userName->text().isEmpty())
        emit checkNameInvoked(ui->le_userName->text());
}

void SignUpDialog::onSignInClicked()
{
    emit signInClicked();
}

void SignUpDialog::clearFields()
{
    ui->le_password1->clear();
    ui->le_password2->clear();
    ui->le_userName->clear();
    ui->chkbox_adming->setChecked(false);
}
