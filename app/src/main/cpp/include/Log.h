//
// Created by msijo on 19.08.2023.
//

#pragma once

class Log {
public:
    static void info(const char* pMessage, ...);
    static void debug(const char* pMessage, ...);
    static void warn(const char* pMessage, ...);
    static void error(const char* pMessage, ...);
};
