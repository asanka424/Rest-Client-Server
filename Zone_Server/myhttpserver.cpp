#include "myhttpserver.h"
#include "HttpServerRequest"
using std::placeholders::_1;
using std::placeholders::_2;

MyHttpServer::MyHttpServer(QObject *parent)
    : Tufao::HttpsServer(parent)
{
    mRequestHandler = new MyRequestHandler();
    //mRouter = new Tufao::HttpServerRequestRouter(this);

   /* Tufao::HttpServerRequestRouter::Handler reqHandler = std::bind(&MyRequestHandler::handleRequest, mRequestHandler, _1, _2);
    mRouter->map(Tufao::HttpServerRequestRouter::Mapping(QRegularExpression("/zone/data"), reqHandler));
    mRouter->map(Tufao::HttpServerRequestRouter::Mapping(QRegularExpression("/zone/user"), reqHandler));*/





}

MyHttpServer::~MyHttpServer()
{

}

