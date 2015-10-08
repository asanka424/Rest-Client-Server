#ifndef MYDATAMODEL_H
#define MYDATAMODEL_H

#include <QStandardItemModel>
#include <dropmenudelegate.h>
#include <QVector>
#include <QTimer>
#include <QHash>

class MyDataModel : public QObject
{
    Q_OBJECT
public:
    MyDataModel(QObject *parent = 0);
    ~MyDataModel();

    QStandardItemModel* getModel(){return mModel;}

public slots:
    void updateTime();



private:
    QStandardItemModel *mModel;
    QVector< QVector<QString> > *data;
    QTimer mTimer;
    QHash<QString, QString> mTimeZones;
};

#endif // MYDATAMODEL_H
