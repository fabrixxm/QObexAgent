#ifndef UTILS_H_1928387465
#define UTILS_H_1928387465

#include <iostream>
#include "properties_interface.h"

template <class T>
void check_reply(QDBusPendingReply<T> *reply, int exitval=-1){
    reply->waitForFinished();
    if (reply->isError()){
        std::cout << qPrintable(reply->error().name()) << " : "
                << qPrintable(reply->error().message())
                << std::endl;
        exit(exitval);
    }
}

#endif // UTILS_H
