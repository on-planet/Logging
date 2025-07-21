#pragma once

#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <string>

/*
 * LogLevel类用于定义日志级别的枚举类型，并提供将日志级别转换为字符串的方法。
 * 日志级别包括DEBUG、INFO、WARN、ERROR、FATAL和OFF，分别表示不同的日志严重性。
 * 通过ToString方法，可以将日志级别转换为对应的字符串表示，方便在日志输出中使用。
 * 例如，DEBUG级别对应字符串"DEBUG"，INFO级别对应字符串"INFO"，以此类推。
 * 该类可以用于日志系统中，以便根据日志级别进行过滤、输出或其他处理。
 *
*/
namespace log{

    class LogLevel{
        public:

            // 定义日志级别的枚举类型   在 C++ 中，传统的 enum 类型的每个枚举成员在底层都对应一个整数值。默认情况下，第一个成员是 0，后续成员依次递增。
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