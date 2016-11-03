//
// Created by 杨浩 on 2016/10/21.
//
#include <jni.h>
#include "MMapLogger.h"

extern "C"
jint
Java_cn_longmaster_pengpeng_ywlog_YWLog_init(
	JNIEnv* env,
	jobject obj,
	jstring path,
	jlong size) {
	const char *pathStr = env->GetStringUTFChars(path, JNI_FALSE);
	return MMapLogger::INSTANCE()->init(pathStr, size);
}

extern "C"
jboolean
Java_cn_longmaster_pengpeng_ywlog_YWLog_uninit(
	JNIEnv* env,
	jobject obj) {
	return MMapLogger::INSTANCE()->uninit();
}

extern "C"
void
Java_cn_longmaster_pengpeng_ywlog_YWLog_flush(
	JNIEnv* env,
	jobject obj) {
	MMapLogger::INSTANCE()->flush();
}

extern "C"
void
Java_cn_longmaster_pengpeng_ywlog_YWLog_d(
	JNIEnv* env,
	jobject obj,
	jstring log) {
	const char *logStr = (const char *)(env->GetStringUTFChars(log, JNI_FALSE));
	MMapLogger::INSTANCE()->d(logStr);
}