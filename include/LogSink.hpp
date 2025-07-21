#pragma once

#ifndef __LOG_SINK_H__
#define __LOG_SINK_H__

#include <iostream>
#include <memory>
#include "Util.hpp"
#include <fstream>


/*
 * LogSink类用于定义日志接收器的接口，派生类可以实现不同的日志输出方式。
 * 例如，StdOutSink类可以将日志输出到标准输出，FileSink类可以将日志写入文件，
 * RollBySizeSink类可以根据文件大小进行日志轮转。
 * 通过使用LogSink类，用户可以灵活地选择日志输出方式，以满足不同的需求。
 *
*/
namespace log{

    // 日志接收器基类，定义了日志接收器的接口
    class LogSink{
        public:
            using ptr = std::shared_ptr<LogSink>;
            virtual ~LogSink() = default;  // 虚析构函数，确保派生类的析构函数被调用
            virtual void LogtoSink(const char* data, size_t len) = 0;  // 纯虚函数，派生类必须实现该方法来处理日志消息
    };

    class StdOutSink : public LogSink{
        public:
            void LogtoSink(const char* data, size_t len) override;
    };

    // 文件日志接收器，将日志写入指定的文件
    class FileSink : public LogSink{
        public:
            FileSink(const std::string& file_path);
            ~FileSink() override;

            void LogtoSink(const char* data, size_t len) override;

        private:
            std::string _file_path;  // 日志文件路径
            std::ofstream _file;  // 文件输出流
    };

    // 轮转日志接收器，根据文件大小进行日志轮转
    class RollBySizeSink : public LogSink{
        public:
            RollBySizeSink(const std::string& basename, size_t max_size);
            ~RollBySizeSink() override;
            void LogtoSink(const char* data, size_t len) override;
        private:
            std::string GetFileName(); // 获取文件名

            std::string _basename; // 基础文件名
            size_t _max_size; // 最大文件大小
            size_t _cur_size; // 当前文件大小
            std::ofstream _ofs; // 文件输出流
            size_t _count; // 文件名计数器
    };
}

#endif
