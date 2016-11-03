package com.leaa.testapp;

import android.content.Context;
import android.util.Log;

import java.io.File;

import cn.longmaster.pengpeng.ywlog.YWLog;

/**
 * Created by 杨浩 on 2016/10/24.
 */
public class YWLogTest {
    public static void testMMap(Context context) {
        Log.d("YWLogTest", "测试MMap类型");

        Log.d("YWLogTest", "初始化开始");
        File sdcard = context.getExternalCacheDir();
        String filePath = sdcard.getAbsolutePath() + File.separatorChar + "log.txt";

        int initResult = YWLog.init(filePath, 128 * 1024);//100k
        Log.d("YWLogTest", "初始化结束" + String.valueOf(initResult));
        if (initResult == 0) {

            runLog();
            runLog();
            runLog();
            runLog();
            runLog();

            Log.d("YWLogTest", "uninit开始");
            YWLog.uninit();
            Log.d("YWLogTest", "uninit结束");
        } else {
            Log.d("YWLogTest", "YWLog init 失败");
        }
    }

    private static void runLog() {
        new Thread() {
            @Override
            public void run() {
                long id = Thread.currentThread().getId();
                Log.d("YWLogTest", "线程:" + id + " 写入开始");
                for (int i = 0; i < 100; i++) {
                    YWLog.d(id + "测试测试" + i);
                }
                Log.d("YWLogTest", "线程:" + id + " 写入结束");
            }
        }.start();
    }
}
