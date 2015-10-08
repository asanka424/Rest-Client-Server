#ifndef USERMOD_H
#define USERMOD_H

#include <QDialog>
#include <QMap>

namespace Ui {
class UserMod;
}

class UserMod : public QDialog
{
    Q_OBJECT

public:
    explicit UserMod(QWidget *parent = 0);
    ~UserMod();
    void init(const QString &user);

private:
    void makeConnections();
protected:
    void closeEvent(QCloseEvent *);

signals:
    void getUserListInvoked(const QString &user);
    void modifyUserInvoked(const QString &user, const QString &password, int isAdmin);
    void deleteUserInvoked(const QString &user);

public slots:
    void onGetUserListResultReceived(bool status, const QVariantMap &data);
    void onModifyUserResultReceived(bool status, const QVariantMap &data);
    void onDeleteClicked();
    void onModifyClicked();
    void onCancelClicked();
    void onUserSelected();


private:
    Ui::UserMod *ui;
    QString mUser;
    QMap<QString, int> mUserMap;
};

#endif // USERMOD_H
