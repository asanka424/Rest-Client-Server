#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <dropmenudelegate.h>
#include <mydatamodel.h>
#include <usermod.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void clearFields();
    void init(const QString &user);

private:
    void makeConnections();
protected:
    void closeEvent(QCloseEvent *);

public slots:
    void addNewRow();
    void deleteRow();
    void onGetDataReplyReceived(bool status, const QVariantMap &map);
    void putData();
    void onPutDataReplyReceived(bool status, const QVariantMap &map);
    void onDeleteUserReplyReceived(bool status, const QVariantMap &map);

signals:
    void putDataInvoked(const QString &user,const QList<QList<QString> > &data);
    void putDataDone();
    void getUserListInvoked(const QString &user);
    void deleteUserInvoked(const QString &user);
    void getUserListReplyReceived(bool, const QVariantMap &map);
    void modifyUserReplyReceived(bool, const QVariantMap &map);
    void modifyUserInvoked(const QString &user, const QString &password, int isAdmin);
    void signout();
    void ownUserDeleted();

private slots:
    void on_actionUser_Management_triggered();
    void on_actionExit_triggered();
    void on_actionSign_Out_triggered();

private:
    void loadTimeZones();


private:
    Ui::MainWindow *ui;
    MyDataModel *model;
    QString mUser;
    DropMenuDelegate *delegate;
    UserMod *mUserModDialog;
    QVector<QString> timeZones;
};

#endif // MAINWINDOW_H
