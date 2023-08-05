//
// Created by zfb457 on 2023/8/4.
//
#include "jni_str.h"

char* jstringToChars(JNIEnv *env, jstring jstr) {

    if (jstr == NULL || env == NULL)
        return NULL;

    const char* str = env->GetStringUTFChars(jstr, NULL);

    if (str == NULL)
        return NULL;

    char* ret = (char*) malloc(sizeof(str) + 1);
    strcpy(ret, str);
    env->ReleaseStringUTFChars(jstr, str);
    return ret;
}

jstring charsToJstring( JNIEnv* env, const char* cstr)
{
    return env->NewStringUTF(cstr);
}