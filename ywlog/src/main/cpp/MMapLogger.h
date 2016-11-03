#pragma once

#ifndef MMAP_LOGGER_H
#define MMAP_LOGGER_H

#include <sys/mman.h>
#include <pthread.h>
#include <stdio.h>
#include <fcntl.h>
#include "common/BlockingQueue.h"

/**
 * MMapLogger使用Linux mmap相关API来进行日志储存。
 * 日志内容将会输出到 init() 指定的文件下。
 * 使用MMapLogger::INSTANCE()来进行相关调用
 *
 * tipsA: 因为mmap只能指定固定大小的缓冲区，为了方便管理，所有日志将循环输出到日志文件中
 * 当遇到文件结尾时，日志将从文件头开始写。该方案导致：
 * 1、之前的日志被覆盖
 * 2、日志文件的行数不代表日志输出顺序
 *
 * tipsB: 因为tipsA固定了文件大小，因此，如果单条日志超过了文件大小，将会写入失败。
 *
 * tipsC: 写入的日志会自动加入时间戳和回车符。考虑到效率原因，该时间戳只保存数字，并不格式化。
 *
 */

class LogInfo {
public:
    const char *time;
    const char *log;

public:
    LogInfo() {
        time = NULL;
        log = NULL;
    }
};

class MMapLogger {
public:
    static MMapLogger *INSTANCE();

    /**
     * 初始化MMapLogger。传入日志文件的路径，以及日志文件的大小。
     * 初始化成功将返回 0.
     * 其他情况请查看实现
     */
    int init(const char *file, unsigned long size);

    /**
     * 反初始化MMapLogger。
     * 成功将返回 true
     * 失败或者该Logger未曾初始化则返回 false
     */
    bool uninit();

    /**
     * 同步将日志写入文件。
     * 成功将返回 true
     * 失败或者该Logger未曾初始化则返回 false
     */
    bool flush();

    /**
     * 写入一条日志。该条日志将会自动加上时间戳以及回车符
     */
    void d(const char *str);

private:
    //获取当前时间的字符串
    char *getCurrentTimeStr();

    //运行在后台线程：把日志写入mmap
    static void *runForWrite(void *parent);

    //检查cursor是否写入条件
    bool checkCursorInBounds(unsigned long length);

    //读入文件中存储的cursor
    unsigned long readCursor();

    //把cursor写入文件
    void writeCursor(unsigned long cursor);

private:
    MMapLogger() : mHasInit(false) { }

    bool mHasInit;
    char *mBuf;
    unsigned long mSize;
    unsigned long mCursor;
    BlockingQueue<LogInfo *> mQueue;
    pthread_t mThreadId;

    static MMapLogger *sMMapLogger;
};

#endif //MMAP_LOGGER_H