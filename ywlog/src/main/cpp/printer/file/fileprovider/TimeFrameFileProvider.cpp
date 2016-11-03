//
// Created by 杨浩 on 2016/11/2.
//
#include "TimeFrameFileProvider.h"
#include <stdio.h>
#include <string.h>
#include "../../../common/Times.h"
#include "../../../common/Constants.h"

TimeFrameFileProvider::TimeFrameFileProvider(const char *dir) {
    mLastFD = -1;
    mLastFileName = NULL;
    //给文件夹路径加上斜杠
    int length = strlen(dir);
    if (dir[length - 1] != file_separator_char) {
        mDir = (char *) malloc(strlen(dir) + 1);
        strcpy(mDir, dir);
        strcat(mDir, file_separator_str);
    } else {
        mDir = (char *) malloc(strlen(dir));
        strcpy(mDir, dir);
    }
}

const char *TimeFrameFileProvider::getFilePath() {
    if (mLastFD != -1 && mLastFileName != NULL) {
        renameFile();
    }

}

void TimeFrameFileProvider::renameFile() {
    //当前文件路径
    char *currentFileName = (char *) malloc(strlen(mDir) + strlen(mLastFileName));
    strcpy(currentFileName, mDir);
    strcat(currentFileName, mLastFileName);

    //重命名后的文件路径
    const char *timeStr = currentTimeMillisStr();
    char *newFileName = (char *) malloc(strlen(currentFileName) + 1 + strlen(timeStr));
    strcpy(newFileName, currentFileName);
    strcat(newFileName, "-");
    strcat(newFileName, timeStr);

    rename(currentFileName, newFileName);

    delete currentFileName;
    delete timeStr;
    delete newFileName;
}
