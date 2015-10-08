#include <QCoreApplication>

#include "myhttpserver.h"
#include <QHostAddress>
#include <QDebug>
#include <QFile>
#include <QSslKey>
#include <QSslCertificate>

#include <HttpServerRequest>
#include <myrequesthandler.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFile key(":/resources/private.pem");
    key.open(QIODevice::ReadOnly);


    QFile cert(":/resources/public.pem");
    cert.open(QIODevice::ReadOnly);

#ifdef HTTPS_SERVER
    Tufao::HttpsServer *server = new Tufao::HttpsServer;

    server->setPrivateKey(QSslKey(&key, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey,
                                 "12345"));
    server->setLocalCertificate(QSslCertificate(&cert));
    qDebug() << "HTTPS server started";
#else
    Tufao::HttpServer *server = new Tufao::HttpServer;
    qDebug() << "HTTP server started";
#endif

    MyRequestHandler *requestHandler = new MyRequestHandler();

    QObject::connect(server, &MyHttpServer::requestReady,
                         [requestHandler](Tufao::HttpServerRequest &req,Tufao::HttpServerResponse &res)
                         {
                            QObject::connect(&req, &Tufao::HttpServerRequest::end, [requestHandler,&req,&res]()
                                                                                        {
                                                                                            requestHandler->handleRequest(req,res);
                                                                                        }
                            );
                         });
    bool status = server->listen(QHostAddress::Any,8080);
    if (status)
        qDebug() << "Server Started";
    else
        qDebug() << "Server failed";
    return a.exec();
}
