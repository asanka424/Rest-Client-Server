#ifndef APPDEPOTUTILITIES_GLOBAL_H
#define APPDEPOTUTILITIES_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(APPDEPOTUTILITIES_LIBRARY)
#  define APPDEPOTUTILITIESSHARED_EXPORT Q_DECL_EXPORT
#else
#  define APPDEPOTUTILITIESSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // APPDEPOTUTILITIES_GLOBAL_H
