#include "dbhandler.h"

DBHandler* DBHandler::instance()
{
        if (!(mInstance)) {
            mMut.lock();
            if (!(mInstance))
                mInstance = new DBHandler();
            mMut.unlock();
        }
        return mInstance;
}
DBHandler* DBHandler::mInstance = 0;
QMutex         DBHandler::mMut;

DBHandler::~DBHandler(){}

