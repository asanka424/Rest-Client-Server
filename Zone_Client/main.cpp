#include <QApplication>
#include <QDateTime>
#include <QTimeZone>

#include <QDebug>

#include "controller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Controller controller;
    controller.startExecution();

    //QString jstr = JSONHandler::encodeSignUpRequest("asanka", "pass", 1);
    return a.exec();
}
