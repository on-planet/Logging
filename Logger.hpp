#pragma once

#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "Level.hpp"
#include "Formatter.hpp"
#include "LogSink.hpp"
#include "Message.hpp"

#include <mutex>
#include <sstream>

/*
 * Logger类用于记录日志消息，支持多种日志级别（DEBUG、INFO、WARN、ERROR、FATAL）。
 * 它提供了可变参数模板方法，允许用户以不同的方式记录日志消息。
 * Logger可以添加多个日志接收器（Sink），如标准输出、文件输出等。
 * 通过使用Formatter，Logger可以格式化日志消息的输出格式。
 *
 */

namespace log{

    class Logger{

        public:

            Logger(const std::string& name = "root", LogLevel::Level level = LogLevel::Level::UNKNOWN,
                   Formatter::ptr formatter = nullptr, std::vector<LogSink::ptr> sinks = {})
                : _logger(name), _level(level) {
                // 设置一个默认的格式化器
                _formatter = std::make_shared<Formatter>("%d{%H:%M:%S}[%p][%c][%f:%l]%T%m%n");
            }

            virtual ~Logger() = default;
            using ptr = std::shared_ptr<Logger>;

            //可变参数模板，用于设置UNKNOWN级别日志
            template<class... Args>
            void Unknown(const std::string& file, size_t line, const Args&... args) {
                LogtoLevel(LogLevel::Level::UNKNOWN, file, line, args...);
            }

            // 可变参数模板，用于 Debug 级别日志
            template<class... Args>
            void Debug(const std::string& file, size_t line, const Args&... args) {
                LogtoLevel(LogLevel::Level::DEBUG, file, line, args...);
            }

            // 可变参数模板，用于 Info 级别日志
            template<class... Args>
            void Info(const std::string& file, size_t line, const Args&... args) {
                    LogtoLevel(LogLevel::Level::INFO, file, line, args...);
            }

            // 可变参数模板，用于 Warn 级别日志
            template<class... Args>
            void Warn(const std::string& file, size_t line, const Args&... args) {
                    LogtoLevel(LogLevel::Level::WARN, file, line, args...);
            }

            // 可变参数模板，用于 Error 级别日志
            template<class... Args>
            void Error(const std::string& file, size_t line, const Args&... args) {
                    LogtoLevel(LogLevel::Level::ERROR, file, line, args...);
            }

            // 可变参数模板，用于 Fatal 级别日志
            template<class... Args>
            void Fatal(const std::string& file, size_t line, const Args&... args) {
                    LogtoLevel(LogLevel::Level::FATAL, file, line, args...);
            }

            //可变参数模板，用于 Off 级别日志
            template<class... Args>
            void OFF(const std::string& file, size_t line, const Args&... args) {
                    LogtoLevel(LogLevel::Level::OFF, file, line, args...);
            }

            // 添加 LogSink
            void AddSink(LogSink::ptr sink) {
                    std::unique_lock<std::mutex> lock(_mutex);
                    _sinks.push_back(sink);
            }

        protected:
            // 分发日志消息到所有接收器
            virtual void dispatchLog(const std::string& formatted_msg){
                std::unique_lock<std::mutex> lock(_mutex);
                for (auto& sink : _sinks) {
                    sink->LogtoSink(formatted_msg.c_str(), formatted_msg.length());
                }
            }
        
        
            std::mutex _mutex; // 互斥锁，确保多线程环境下的安全访问
            std::string _logger; // 日志记录器名称
            LogLevel::Level _level; // 日志级别
            Formatter::ptr _formatter; // 日志格式化器
            std::vector<LogSink::ptr> _sinks; // 日志接收器列表

        private:
            template<class... Args>
            void LogtoLevel(LogLevel::Level level, const std::string& file, size_t line, const Args&... args) {
                // 1. 判断日志级别是否需要记录
                if (level < _level) {  // 如果当前日志级别低于 Logger 的级别，则不记录日志
                    return;
                }

                // 2. 使用 stringstream 构造日志消息主体
                std::stringstream ss;
                // C++17 折叠表达式，将所有参数写入流
                (ss << ... << args);
                // 3. 创建日志消息对象
                LogMsg msg(level, _logger, file, line, ss.str());
                // 4. 使用格式化器生成最终日志字符串
                std::string formatted_msg = _formatter->Format(msg);
                // 5. 调用 dispatchLog 方法将格式化后的日志消息发送到所有接收器
                dispatchLog(formatted_msg);
            }

    };


}

#endif
