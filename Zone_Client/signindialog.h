#ifndef SIGNINDIALOG_H
#define SIGNINDIALOG_H

#include <QDialog>

namespace Ui {
class SignInDialog;
}

class SignInDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SignInDialog(QWidget *parent = 0);
    ~SignInDialog();
    void clearFields();
private:
    void makeConnections();

private slots:
    void onSignInClicked();
    void onCancelClicked();
    void onSignUpClicked();

signals:
    void signInInvoked(const QString &user, const QString &pass);
    void signUpInvoked();

private:
    Ui::SignInDialog *ui;
};

#endif // SIGNINDIALOG_H
