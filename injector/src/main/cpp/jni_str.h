//
// Created by zfb457 on 2023/8/4.
//

#ifndef NATIVEINJECTOR_JNI_STR_H
#define NATIVEINJECTOR_JNI_STR_H

#include <jni.h>
#include <cstdlib>
#include <cstring>

char* jstringToChars(JNIEnv *env, jstring jstr);
jstring charsToJstring( JNIEnv* env, const char* cstr);

#endif //NATIVEINJECTOR_JNI_STR_H
