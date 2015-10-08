#include "mydatamodel.h"
#include <QTimer>
#include <QModelIndex>
#include <QDateTime>
#include <QTimeZone>
#include <QFile>

MyDataModel::MyDataModel(QObject *parent):QObject(parent)
{
    QList<QByteArray> ids = QTimeZone::availableTimeZoneIds();
    mModel = new QStandardItemModel(0,4,this);
/*    for (int i=0; i<2; i++)
        for (int j=0; j<3; j++)
        {
            QModelIndex index = mModel->index(i,j,QModelIndex());
            mModel->setData(index,QVariant("test"));
        }
*/
    mTimer.setInterval(500);

    connect(&mTimer,SIGNAL(timeout()),this,SLOT(updateTime()));
    mTimer.start();
    mModel->setHeaderData(0,Qt::Horizontal,tr("Id"));
    mModel->setHeaderData(1,Qt::Horizontal,tr("Name"));
    mModel->setHeaderData(2,Qt::Horizontal,tr("Time Zone"));
    mModel->setHeaderData(3,Qt::Horizontal,tr("Time"));
}

void MyDataModel::updateTime()
{
    for (int j=0; j<mModel->rowCount(); j++)
    {
        QModelIndex index = mModel->index(j,3,QModelIndex());
        QModelIndex index_tz = mModel->index(j,2,QModelIndex());
        QString tz_full = index_tz.data().toString();
        QString tz = tz_full.split(" ")[0];
        QDateTime utc = QDateTime::currentDateTimeUtc();
        QByteArray b_tz(tz.toStdString().c_str());
        //QByteArray b_tz("Asia/Colombo");
        QTimeZone zone(b_tz);
        utc.setTimeZone(zone);
        int offset = utc.offsetFromUtc();
        QDateTime nnd = utc.addSecs(offset);
        QString date = nnd.toString(QString("yyyy-MM-dd HH:mm:ss"));

        mModel->setData(index,QVariant(date));
    }

}

MyDataModel::~MyDataModel()
{

}



