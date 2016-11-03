#include "MMapLogger.h"
#include "common/Log.h"

MMapLogger *MMapLogger::sMMapLogger = NULL;

MMapLogger *MMapLogger::INSTANCE() {
    if (sMMapLogger == NULL) {
        sMMapLogger = new MMapLogger();
    }
    return sMMapLogger;
}

int MMapLogger::init(const char *file, unsigned long size) {
    uninit();

    int fd = open(file, O_RDWR | O_CREAT);
    if (fd == -1) {
        //文件打开失败
        return 2;
    }
    if (ftruncate(fd, size) == -1) {
        //扩展文件失败
        return 3;
    }
    //建立内存映射,
    mBuf = (char *) mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mBuf == MAP_FAILED) {
        //建立MMap失败
        return 4;
    }
    close(fd);
    this->mSize = size;
    //读取文件中的Cursor
    this->mCursor = readCursor();
    //设置初始化成功标志位
    this->mHasInit = true;
    //启动写日志线程
    pthread_create(&mThreadId, NULL, &MMapLogger::runForWrite, this);

    return 0;
}

bool MMapLogger::uninit() {
    if (!mHasInit) {
        return false;
    }
    mHasInit = false;
    //关闭写日志线程
    char *logstr = new char[25];
    strcpy(logstr, "shutdown mmapLogger");
    LogInfo *info = new LogInfo();
    info->log = logstr;
    info->time = getCurrentTimeStr();
    mQueue.push(info);
    pthread_join(mThreadId, NULL);

    LOGI("main thread exit, thread-%ld", pthread_self());
    return munmap(mBuf, mSize) != -1;
}

bool MMapLogger::flush() {
    if (mHasInit) {
        msync(mBuf, mSize, MS_SYNC);
        return true;
    }
    return false;
}

void MMapLogger::d(const char *str) {
    if (mHasInit) {
        LogInfo *info = new LogInfo();
        info->log = str;
        info->time = getCurrentTimeStr();

        LOGI("log.d:thread-%ld, %s", pthread_self(), str);
        mQueue.push(info);
    }
}

char timeFormat[15];

char *MMapLogger::getCurrentTimeStr() {
    //取到当前时间
    timeval millis;
    gettimeofday(&millis, NULL);

    //经测试，如果格式化时间，耗时比不格式化多一倍，因此在日志文件中仅保留时间戳
    //待需要分析日志时，再格外用配套工具重新格式化时间
    sprintf(timeFormat, "%ld%03d\0", millis.tv_sec, (int) (millis.tv_usec / 1000));
    return timeFormat;
/*
    time_t today;
    time(&today);
    tm *date = localtime(&today);

    sprintf(timeFormat, "%02d-%02d %02d:%02d:%02d.%03d\0",
            date->tm_mon, date->tm_mday,
            date->tm_hour, date->tm_min, date->tm_sec,
            (int) (millis.tv_usec / 1000));
    return timeFormat;*/
}

void *MMapLogger::runForWrite(void *parent) {
    MMapLogger *logger = (MMapLogger *) parent;
    while (logger->mHasInit) {
        LogInfo *logInfo = logger->mQueue.poll();
        if (!logger->mHasInit) {
            delete logInfo->log;
            delete logInfo;
            break;
        }
        LOGI("log.write: thread-%ld, %s", pthread_self(), logInfo->log);
        //计算该条日志的长度
        unsigned long timeLength = strlen(logInfo->time);
        unsigned long logLength = strlen(logInfo->log);
        //判断剩余空间是否足够打印该条日志
        //timeLength + 1 是预留时间与日志的之间的空格
        //length + 1是预留回车符
        if (logger->checkCursorInBounds(timeLength + 1 + logLength + 1)) {
            try {
                //写入时间
                memcpy(&logger->mBuf[logger->mCursor], logInfo->time, timeLength);
                logger->mCursor += timeLength;
                //写入时间与日志之间的空格
                logger->mBuf[logger->mCursor++] = ' ';

                //写入日志
                memcpy(&logger->mBuf[logger->mCursor], logInfo->log, logLength);
                logger->mCursor += logLength;
                //写入回车符
                logger->mBuf[logger->mCursor++] = '\n';

                //把Cursor写入文件头
                logger->writeCursor(logger->mCursor);
            } catch (...) {

            }
        }
        delete logInfo->log;
        delete logInfo;
    }
    LOGI("write thread exit, thread-%ld", pthread_self());
    return 0;
}

bool MMapLogger::checkCursorInBounds(unsigned long length) {
    float sizeOfLong = sizeof(long);
    float sizeOfChar = sizeof(char);
    unsigned long chars = (unsigned long) (sizeOfLong / sizeOfChar);
    if (length >= mSize - chars + 1) {
        return false;
    }
    if (mCursor + length >= mSize
        || mCursor < chars + 1) {
        mCursor = chars + 1;
    }
    return true;
}

/**
 * 通过位运算将各个char合并到unsigned long中
 */
unsigned long MMapLogger::readCursor() {
    float sizeOfLong = sizeof(long);
    float sizeOfChar = sizeof(char);
    int chars = (int) (sizeOfLong / sizeOfChar);
    unsigned long cursor = 0;
    for (int i = 0; i < chars; i++) {
        unsigned long byte = ((unsigned long) (unsigned char) mBuf[i]) << (i * 8);
        cursor |= byte;
    }
    return cursor;
}

/**
 * 通过位运算将 unsigned long 的各个byte分解到 char大小并存储
 */
void MMapLogger::writeCursor(unsigned long cursor) {
    if (mHasInit) {
        float sizeOfLong = sizeof(long);
        float sizeOfChar = sizeof(char);
        int chars = (int) (sizeOfLong / sizeOfChar);
        unsigned long flag = 0xFF;
        for (int i = 0; i < chars; i++) {
            int byte = (int) ((cursor & flag) >> (i * 8));
            mBuf[i] = byte;
            flag = flag << 8;
        }
        mBuf[chars] = '\n';
    }
}