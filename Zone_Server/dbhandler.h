#ifndef DBHANDLER_H
#define DBHANDLER_H

#include <QObject>
#include <QtSql>
#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <QPair>
#include <QDateTime>


class DBHandler : public QObject
{
    Q_OBJECT
public:
    ~DBHandler();
public:

    static DBHandler* instance(void);
    QPair<QString, QString> getPassword(const QString &userName)
    {
        QPair<QString, QString> retVal;

        if (mDatabase.isOpen())
        {
            QSqlQuery query;
            query.prepare("SELECT Salt, Password from User where UserName = :name");
            query.bindValue(":name",userName);
            query.exec();
            QString driverError = query.lastError().driverText();
            QString dbrError = query.lastError().databaseText();
            QString salt,password;
            while (query.next())
            {
                salt = query.value(0).toString();
                password = query.value(1).toString();
                retVal = QPair<QString,QString>(salt,password);
            }
        }

        return retVal;
    }
    bool updateAuthToken(const QString &authToken, const QString &user)
    {
        bool retVal = false;
        if (mDatabase.isOpen())
        {
            QSqlQuery query;
            query.prepare("UPDATE User SET AuthToken=:authToken, Expiry=:exp WHERE UserName=:user");
            query.bindValue(":authToken", authToken);
            query.bindValue(":exp",QDateTime::currentDateTime().addDays(1).toString(QString("yyyy-MM-dd hh:mm:ss")));
            query.bindValue(":user",user);
            retVal = query.exec();
            QString driverError = query.lastError().driverText();
            QString dbrError = query.lastError().databaseText();
        }
        return retVal;

    }
    bool updateAuthTokenExpiry(const QString &user)
    {
        bool retVal = false;
        if (mDatabase.isOpen())
        {
            QSqlQuery query;
            query.prepare("UPDATE User SET Expiry=:exp WHERE UserName=:user");
            query.bindValue(":exp",QDateTime::currentDateTime().toString(QString("yyyy-MM-dd hh:mm:ss")));
            query.bindValue(":user",user);
            retVal = query.exec();
            QString driverError = query.lastError().driverText();
            QString dbrError = query.lastError().databaseText();
        }
        return retVal;

    }
    bool insertNewUser(const QString &authToken, const QString &user, const QString &password, const QString &salt, const QString &isAdmin)
    {
        bool retVal = false;
        if (mDatabase.isOpen())
        {
            QSqlQuery query;
            query.prepare("INSERT INTO User (UserName, Salt, Password, IsAdmin, AuthToken, Expiry) VALUES (:user, :salt, :password, :isAdmin, :authToken, :expiry)");
            query.bindValue(":user",user);
            query.bindValue(":salt",salt);
            query.bindValue(":password", password);
            query.bindValue(":isAdmin",isAdmin);
            query.bindValue(":authToken", authToken);
            query.bindValue(":expiry",QDateTime::currentDateTime().addDays(1).toString(QString("yyyy-mm-dd HH:MM:ss")));

            retVal = query.exec();
            QString driverError = query.lastError().driverText();
            QString dbrError = query.lastError().databaseText();
        }
        return retVal;

    }

    bool modifyUser(const QString &authToken, const QString &user, const QString &password, const QString &salt, const QString &isAdmin)
    {
        bool retVal = false;
        if (mDatabase.isOpen())
        {
            QSqlQuery query;
            query.prepare("UPDATE User SET Salt = :salt, Password = :password, IsAdmin = :isAdmin, AuthToken = :authToken, Expiry = :expiry  Where UserName = :user");
            query.bindValue(":user",user);
            query.bindValue(":salt",salt);
            query.bindValue(":password", password);
            query.bindValue(":isAdmin",isAdmin);
            query.bindValue(":authToken", authToken);
            query.bindValue(":expiry",QDateTime::currentDateTime().addDays(1).toString(QString("yyyy-mm-dd HH:MM:ss")));

            retVal = query.exec();
            QString driverError = query.lastError().driverText();
            QString dbrError = query.lastError().databaseText();
        }
        return retVal;

    }
    bool deleteUser(const QString &user)
    {
        bool retVal = false;
        if (mDatabase.isOpen())
        {
            //first delete his data
            QSqlQuery query;
            query.prepare("DELETE from Data WHERE UserId in (SELECT Id AS UserId FROM User WHERE UserName = :user)");
            query.bindValue(":user",user);

            retVal = query.exec();
            QString driverError = query.lastError().driverText();
            QString dbrError = query.lastError().databaseText();

            if (!retVal)
                return retVal;
            //now delete user
            query.prepare("DELETE from User WHERE UserName = :user");
            query.bindValue(":user",user);

            retVal = query.exec();
            driverError = query.lastError().driverText();
            dbrError = query.lastError().databaseText();


        }
        return retVal;

    }
    bool checkUser(const QString &user)
    {
        bool retVal = false;
        if (mDatabase.isOpen())
        {
            QSqlQuery query;
            query.prepare("SELECT count(*) From User WHERE UserName = :user");
            query.bindValue(":user",user);
            query.exec();
            if (query.next())
            {
                int count = query.value(0).toInt();
                if (count > 0)
                    retVal = true;
            }
        }
        return retVal;
    }
    QMap<QString, QString> getUserDetails(const QString &user)
    {
        QMap<QString, QString> retVal;
        if (mDatabase.isOpen())
        {
            QSqlQuery query;
            query.prepare("SELECT AuthToken, Expiry, IsAdmin From User WHERE UserName = :user");
            query.bindValue(":user",user);
            query.exec();
            if (query.next())
            {
                QString authToken = query.value("AuthToken").toString();
                QString expiry = query.value("Expiry").toString();
                int isAdmin = query.value("IsAdmin").toInt();

                retVal["auth-token"] = authToken;
                retVal["expiry"] = expiry;
                retVal["is-admin"] = QString::number(isAdmin);
            }
        }
        return retVal;
    }
    QList<QList<QString> > getData(const QString &user)
    {
         QList<QList<QString> > retVal;
        if (mDatabase.isOpen())
        {
            QSqlQuery query;
            query.prepare("select Data.Id, Data.Name, Data.TimeZone from Data join User on Data.UserId = User.Id where User.UserName = :user");
            query.bindValue(":user",user);
            query.exec();
            QList<QString> tmpList;
            while (query.next())
            {
                int id = query.value("Id").toInt();
                QString name = query.value("Name").toString();
                QString timeZone = query.value("TimeZone").toString();

                tmpList.clear();
                tmpList.append(QString::number(id));
                tmpList.append(name);
                tmpList.append(timeZone);

                retVal.append(tmpList);
            }
        }
        qDebug() << retVal;
        return retVal;
    }

    bool putData(const QString &user, const QList<QList<QString> > &data)
    {
        bool retVal = false;
        if (mDatabase.isOpen())
        {
            QSqlQuery query;
            query.prepare("DELETE from Data WHERE UserId in (SELECT Id AS UserId FROM User WHERE UserName = :user)");
            query.bindValue(":user",user);
            retVal = query.exec();
            if (!retVal)
                return retVal;
            for (int i=0; i<data.size(); i++)
            {
                QList<QString> row = data[i];
                query.prepare("INSERT INTO Data ( UserId, Name, TimeZone)  Select Id, :name, :timezone FROM User WHERE User.UserName = :user");
                query.bindValue(":name", row[1]);
                query.bindValue(":timezone", row[2]);
                query.bindValue(":user", user);
                retVal = query.exec();

                QString driverError = query.lastError().driverText();
                QString dbrError = query.lastError().databaseText();
                if (!retVal)
                    return retVal;
            }
            mDatabase.commit();
        }

        return retVal;
    }
    bool getUsers(const QString &user, QList<QPair<QString,int> > &userList)
    {
        bool retVal = false;
        userList.clear();
        if (mDatabase.isOpen())
        {
            QSqlQuery query;
            query.prepare("SELECT IsAdmin FROM User WHERE UserName = :user");
            query.bindValue(":user",user);
            retVal = query.exec();
            QString driverError = query.lastError().driverText();
            QString dbrError = query.lastError().databaseText();
            if (!retVal)
                return retVal;

            int isAdmin = 0;
            if (query.next())
                isAdmin = query.value("IsAdmin").toInt();

            if (isAdmin == 1)
            {
                query.prepare("SELECT UserName,IsAdmin FROM User");
                retVal = query.exec();
                if (!retVal)
                    return retVal;
                while(query.next())
                    userList.append(QPair<QString,int>(query.value("UserName").toString(),query.value("IsAdmin").toInt()));

            }
            else
                userList.append(QPair<QString, int>(user,0));
        }

        return retVal;
    }


private:


private:
    static DBHandler*   mInstance;
    static QMutex           mMut;
    QSqlDatabase mDatabase;

private:
    explicit DBHandler(QObject *parent = 0):QObject(parent)
    {

        mDatabase = QSqlDatabase::addDatabase("QSQLITE");
        mDatabase.setDatabaseName("../../Zone_Server/resources/zone_serverdb.db");
        mDatabase.open();
        if (mDatabase.isOpen())
            qDebug() << "DB Open";
        else
            qDebug() << "DB Error";
    }
    DBHandler(const DBHandler&){}
    DBHandler& operator=(const DBHandler&){}
};


#endif // DBHANDLER_H
