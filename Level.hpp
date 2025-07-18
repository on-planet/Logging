#pragma once

#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <string>

namespace log{

    class LogLevel{
        public:

            // 定义日志级别的枚举类型
            enum Level{
                UNKNOWN = 0,
                DEBUG,
                INFO,
                WARN,
                ERROR,
                FATAL,
                OFF
            };

            //将日志级别转换为字符串
            static std::string ToString(const Level level) {
                switch (level) {
                    case DEBUG: return "DEBUG";
                    case INFO: return "INFO";
                    case WARN: return "WARN";
                    case ERROR: return "ERROR";
                    case FATAL: return "FATAL";
                    case OFF: return "OFF";
                    default: return "UNKNOWN"; // 如果级别未知，返回UNKNOWN
                }
            }
    };
}

#endif