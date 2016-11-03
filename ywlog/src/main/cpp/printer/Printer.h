//
// Created by 杨浩 on 2016/11/2.
//

#ifndef TESTAPP_PRINTER_H
#define TESTAPP_PRINTER_H

class Printer {
public:

    virtual void start() = 0;

    virtual void println(int logLevel, const char *tag, const char *msg) = 0;

    virtual void shutdown() = 0;
};

#endif //TESTAPP_PRINTER_H
