#pragma once

#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include "Util.hpp"
#include "Level.hpp"
#include <thread>
#include <utility>

/*
 * LogMsg类用于表示一条日志消息，包含时间戳、日志级别、日志名称、线程ID、源码文件名、源码行号和日志内容等信息。
 * 它提供了获取和设置这些信息的方法，方便在日志系统中使用。
 *
*/
namespace log{

    class LogMsg{

        public:

            LogMsg() : _ctime(Date::Now()),
                _level(LogLevel::UNKNOWN),
                _logger("root"),
                _file(""),
                _tID(std::this_thread::get_id()),
                _line(0),
                _payload(""){}

            LogMsg(LogLevel::Level level,
                std::string logger,
                std::string file,
                size_t line,
                std::string  payload
                ) : _ctime(Date::Now()),
                    _level(level),
                    _logger(std::move(logger)),
                    _file(std::move(file)),
                    _tID(std::this_thread::get_id()),
                    _line(line),
                    _payload(std::move(payload)){}
            ~LogMsg(){}

            time_t getTime_t() const { return _ctime; }  //获取时间戳
            LogLevel::Level getLevel() const { return _level; } //获取日志级别
            std::string getLogger() const { return _logger; } //获取日志名称
            static std::thread::id getThreadID() { return std::this_thread::get_id(); } //获取线程ID
            std::string getFile() const { return _file; } //获取源码文件名
            size_t getLine() const { return _line; } //获取源码行号
            std::string getPayload() const { return _payload; } //获取日志内容

            void setTime_t(time_t ctime) { _ctime = ctime; } //设置时间戳
            void setLevel(LogLevel::Level level) { _level = level; }
            void setLogger(const std::string& logger) { _logger = logger; } //设置日志名称
            void setFile(const std::string& file) { _file = file; } //设置源码文件名
            void setLine(size_t line) { _line = line; } //设置源码行号
            void setPayload(const std::string& payload) { _payload = payload; } //设置日志内容
            void setThreadID(std::thread::id tid) { _tID = tid; } //设置线程ID

        protected:
            time_t _ctime;   //时间戳
            LogLevel::Level _level;  //日志级别
            std::string _logger;  //日志名称
            std::string _file;  //源码文件名
            std::thread::id _tID;  //线程ID
            size_t _line;  //源码行号
            std::string _payload;  //日志内容

    };

}


#endif