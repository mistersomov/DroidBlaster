//
// Created by msijo on 19.08.2023.
//

#pragma once

#include <cstdarg>
#include <android/log.h>
#include "Log.h"

void Log::info(const char *pMessage, ...) {
    va_list varArgs;
    va_start(varArgs, pMessage);
    __android_log_vprint(ANDROID_LOG_INFO, "DroidBlaster", pMessage, varArgs);
    va_end(varArgs);
}

void Log::debug(const char *pMessage, ...) {
    va_list varArgs;
    va_start(varArgs, pMessage);
    __android_log_vprint(ANDROID_LOG_DEBUG, "DroidBlaster", pMessage, varArgs);
    va_end(varArgs);
}

void Log::warn(const char *pMessage, ...) {
    va_list varArgs;
    va_start(varArgs, pMessage);
    __android_log_vprint(ANDROID_LOG_WARN, "DroidBlaster", pMessage, varArgs);
    va_end(varArgs);
}

void Log::error(const char *pMessage, ...) {
    va_list varArgs;
    va_start(varArgs, pMessage);
    __android_log_vprint(ANDROID_LOG_ERROR, "DroidBlaster", pMessage, varArgs);
    va_end(varArgs);
}