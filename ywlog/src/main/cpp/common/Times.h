//
// Created by 杨浩 on 2016/11/2.
//

#ifndef TESTAPP_TIMES_H
#define TESTAPP_TIMES_H

#include <sys/time.h>
#include <stdio.h>
#include <malloc.h>

/**
 * 获取当前时间的毫秒数
 */
unsigned long long currentTimeMillis() {
    timeval time;
    gettimeofday(&time, NULL);

    return ((unsigned long long) time.tv_sec) * 1000 + (unsigned int) time.tv_usec / 1000;
}

/**
 * 获取当前时间毫秒数的字符串。
 * 请在使用完毕后使用 delete 释放空间，否则会引起内存泄漏
 */
char *currentTimeMillisStr() {
    timeval millis;
    gettimeofday(&millis, NULL);

    char *timeFormat = (char *) malloc(15 * sizeof(char));
    sprintf(timeFormat, "%ld%03d\0", millis.tv_sec, (int) (millis.tv_usec / 1000));
    return timeFormat;
}

#endif //TESTAPP_TIMES_H
