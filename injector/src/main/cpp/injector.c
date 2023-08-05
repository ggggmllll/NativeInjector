#include <jni.h>
#include "injector.h"

void init(JNIEnv *env, jobject thiz, jint pid, jint arch, jint mode) {
    // TODO: implement init()
}

jstring dumpStack(JNIEnv *env, jobject thiz) {
    // TODO: implement dumpStack()
}

jint injectLib(JNIEnv *env, jobject thiz, jstring path) {
    // TODO: implement injectLib()
}

jint pltHook(JNIEnv *env, jobject thiz, jstring path,
                                       jstring old_func_name, jstring new_func_name) {
    // TODO: implement pltHook()
}

jint inlineHook(JNIEnv *env, jobject thiz, jlong addr, jstring code,
                                          jint type) {
    // TODO: implement inlineHook()
}

jint trapHook(JNIEnv *env, jobject thiz, jlong addr, jintArray action,
                                        jint type) {
    // TODO: implement trapHook()
}