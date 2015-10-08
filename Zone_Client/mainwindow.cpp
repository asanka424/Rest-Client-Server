#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QModelIndex>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDebug>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadTimeZones();
    model = new MyDataModel();



    ui->tableView->setModel(model->getModel());
    delegate = new DropMenuDelegate(timeZones);
    ui->tableView->setItemDelegateForColumn(2,delegate);
    int rows = model->getModel()->rowCount();
    for (int i=0; i<rows; i++)
        ui->tableView->openPersistentEditor(model->getModel()->index(i,1));
    ui->tableView->setSortingEnabled(true);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setColumnHidden(0,true);

    mUserModDialog = new UserMod(this);
    mUserModDialog->setModal(true);

        makeConnections();

}

void MainWindow::makeConnections()
{
    connect(ui->btn_add,SIGNAL(clicked()),this,SLOT(addNewRow()),Qt::QueuedConnection);
    connect(ui->btn_delete,SIGNAL(clicked()),this,SLOT(deleteRow()),Qt::QueuedConnection);
    connect(ui->btn_sync,SIGNAL(clicked()),this,SLOT(putData()));

    connect(mUserModDialog,SIGNAL(getUserListInvoked(QString)),this,SIGNAL(getUserListInvoked(QString)));
    connect(mUserModDialog,SIGNAL(deleteUserInvoked(QString)),this,SIGNAL(deleteUserInvoked(QString)));
    connect(mUserModDialog,SIGNAL(modifyUserInvoked(QString,QString,int)),this,SIGNAL(modifyUserInvoked(QString,QString,int)));
    connect(this,SIGNAL(getUserListReplyReceived(bool,QVariantMap)),mUserModDialog,SLOT(onGetUserListResultReceived(bool,QVariantMap)));
    connect(this,SIGNAL(modifyUserReplyReceived(bool,QVariantMap)),mUserModDialog,SLOT(onModifyUserResultReceived(bool,QVariantMap)));

}

MainWindow::~MainWindow()
{
    delete ui;
    delegate->deleteLater();
    model->deleteLater();


}

void MainWindow::init(const QString &user)
{
    mUser = user;

}

void MainWindow::loadTimeZones()
{
    QFile timeZoneFile(":Data/resources/timezones.csv");
    if (timeZoneFile.open(QIODevice::ReadOnly))
    {
        QTextStream out(&timeZoneFile);
        while (!out.atEnd())
        {
            timeZones.push_back(out.readLine());
        }
    }
}
void MainWindow::addNewRow()
{

    QList<QStandardItem*> newRow;
    for (int i=0;i<(model->getModel())->columnCount();i++)
    {
        QStandardItem *itm;
        if (i==2)
            itm = new QStandardItem(QString("Pacific/Midway ( -11:00 )"));
        else if (i==0)
            itm = new QStandardItem(QString("-1"));
        else
            itm = new QStandardItem();

        newRow.append(itm);
    }


    model->getModel()->appendRow(newRow);
    ui->tableView->openPersistentEditor(model->getModel()->index(model->getModel()->rowCount()-1,2));
    ui->tableView->resizeColumnsToContents();
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::deleteRow()
{
    QModelIndexList indexes = ui->tableView->selectionModel()->selection().indexes();
    QMap<int,bool> rowMap;
    for (int i = 0; i < indexes.count(); ++i)
    {
        QModelIndex index = indexes.at(i);
        rowMap.insert(index.row(),true);
        // To get the row/column numbers use index.row() / index.column()
    }
    QMap<int,bool>::iterator i;
    int pos=0;
    for (i = rowMap.begin(); i != rowMap.end(); ++i)
    {
        model->getModel()->removeRow(i.key() - pos);
        pos++;
    }
}

void MainWindow::onGetDataReplyReceived(bool status, const QVariantMap &map)
{
    if (status)
    {
        if (map.contains("data"))
        {
            while(model->getModel()->rowCount() > 0)
                model->getModel()->removeRow(0);
            QList<QVariant> dataList = map["data"].toList();
            for (int i=0; i<dataList.size(); i++)
            {
                QList<QVariant> dataRow = dataList[i].toList();
                QList<QStandardItem*> itemList;
                for (int j=0; j<dataRow.size(); j++)
                {
                    itemList.append(new QStandardItem(dataRow[j].toString()));
                }

                model->getModel()->appendRow(itemList);
                ui->tableView->openPersistentEditor(model->getModel()->index(model->getModel()->rowCount()-1,2));
                ui->tableView->resizeColumnsToContents();
                ui->tableView->horizontalHeader()->setStretchLastSection(true);
            }
        }
    }
    else
    {
        QMessageBox::critical(this,tr("Error"), QString("Error retrieving data for user " + mUser));
    }
    this->setEnabled(true);
}

void MainWindow::clearFields()
{
    while (model->getModel()->rowCount() > 0)
        model->getModel()->removeRow(0);
}

void MainWindow::putData()
{
    QList<QList<QString> > data;

    for (int i=0; i<model->getModel()->rowCount(); i++)
    {
        QList<QString> row;
        for (int j=0; j<model->getModel()->columnCount()-1; j++)
        {
            QModelIndex index = model->getModel()->index(i,j);
            row.append(index.data().toString());
        }
        data.append(row);
    }

    emit putDataInvoked(mUser, data);
}

void MainWindow::onPutDataReplyReceived(bool status, const QVariantMap &map)
{
    if (status)
    {
        QMessageBox::information(this,tr("Done"), tr("Successfully Synced"));
    }
    else
    {
        QMessageBox::critical(this,tr("Failed"),tr("Sync Failed. ") + map["error"].toString());
    }
    emit putDataDone();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "came to close";

    QEventLoop ev;
    connect(this,SIGNAL(putDataDone()),&ev,SLOT(quit()));
    putData();
    ev.exec();
    emit signout();
    event->accept();

}

void MainWindow::onDeleteUserReplyReceived(bool status, const QVariantMap &jMap)
{
    if (status)
    {
        QString user = jMap["user"].toString();
        if (user == mUser)
        {
            QMessageBox::information(mUserModDialog,tr("Info"),tr("Own account deleted. Please log in"));

            mUserModDialog->hide();
            emit ownUserDeleted();
        }
        else
        {
            QMessageBox::information(mUserModDialog,tr("Deleted"),tr("User ") + user + tr(" deleted."));
            mUserModDialog->init(mUser);
            mUserModDialog->setEnabled(true);
        }
    }
    else
    {
        QMessageBox::critical(this,tr("Error"), tr("Error in delete"));
    }
}

void MainWindow::on_actionUser_Management_triggered()
{
    mUserModDialog->init(mUser);
    mUserModDialog->show();
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionSign_Out_triggered()
{
    QEventLoop ev;
    connect(this,SIGNAL(putDataDone()),&ev,SLOT(quit()));
    putData();
    ev.exec();
    emit signout();

}
