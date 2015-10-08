#ifndef MYHTTPSERVER_H
#define MYHTTPSERVER_H

#include <httpserver.h>
#include <httpsserver.h>
#include <httpserverrequestrouter.h>
#include "myrequesthandler.h"

class MyHttpServer : public Tufao::HttpsServer
{
    Q_OBJECT

public:
   explicit MyHttpServer(QObject *parent = 0);
   ~MyHttpServer();

signals:
   void newMessage(QByteArray message);

private:
   Tufao::HttpServerRequestRouter* mRouter;
   MyRequestHandler *mRequestHandler;
};

#endif // HTTPSERVER_H
