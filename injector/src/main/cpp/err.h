//
// Created by zfb457 on 2023/8/4.
//

#ifndef NATIVEINJECTOR_ERR_H
#define NATIVEINJECTOR_ERR_H

#include <android/log.h>

#define ERR_SUCCESS 0b0
#define ERR_ATTACH 0b1
#define ERR_CONT 0b10
#define ERR_DETACH 0b100
#define ERR_REGS 0b1000
#define ERR_CALL 0b10000

#define ERR_TAG "injector failed"
#define DEBUG_TAG "injector debug"
#define perror(...) __android_log_print(ANDROID_LOG_ERROR, ERR_TAG, __VA_ARGS__)
#define debug_log(...) __android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, __VA_ARGS__)

static int err = ERR_SUCCESS;

#endif //NATIVEINJECTOR_ERR_H
