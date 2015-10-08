#include "configuration.h"


Configuration* Configuration::instance(){
        if (!(mInstance)) {
            mMut.lock();
            if (!(mInstance))
                mInstance = new Configuration();
            mMut.unlock();
        }
        return mInstance;
    }
    Configuration* Configuration::mInstance = 0;
    QMutex         Configuration::mMut;



