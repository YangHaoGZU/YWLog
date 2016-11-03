//
// Created by 杨浩 on 2016/11/2.
//

#ifndef TESTAPP_TIMEFRAMEFILEPROVIDER_H
#define TESTAPP_TIMEFRAMEFILEPROVIDER_H

#include "FileProvider.h"

class TimeFrameFileProvider : public FileProvider {
public:
    TimeFrameFileProvider(const char *dir);

    const char * getFilePath();

private:
    /**
     * 重命名文件
     * 刚建立文件时，文件名为：startTimeMills。为建立文件时的时间
     * 当该文件写入完毕时，文件名更改为：startTimeMills-endTimeMills。为该文件存在的区间
     */
    void renameFile();

private:
    int mLastFD;
    char *mLastFileName;
    char *mDir;
};

#endif //TESTAPP_TIMEFRAMEFILEPROVIDER_H
