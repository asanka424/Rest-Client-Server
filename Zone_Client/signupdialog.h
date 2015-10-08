#ifndef SIGNUPDIALOG_H
#define SIGNUPDIALOG_H

#include <QDialog>

namespace Ui {
class SignUpDialog;
}

class SignUpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SignUpDialog(QWidget *parent = 0);
    ~SignUpDialog();
    void clearFields();
private:
    void makeConnections();

private slots:
    void onSignUpClicked();
    void onCancelClicked();
    void onCheckNameClicked();
    void onSignInClicked();

signals:
    void signUpInvoked(const QString &user, const QString &pass, int isAdmin);
    void signInClicked();
    void checkNameInvoked(const QString &user);
private:
    Ui::SignUpDialog *ui;
};

#endif // SIGNINDIALOG_H
