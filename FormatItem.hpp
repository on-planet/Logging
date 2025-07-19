#pragma once

#ifndef FORMAT_ITEM_H_
#define FORMAT_ITEM_H_

#include <memory>
#include <iostream>
#include "Message.hpp"

/*
 * FormatItem类用于定义日志格式化项的基类，派生类可以实现不同的日志格式化功能。
 * 例如，TimeFormatItem类可以格式化时间戳，LevelFormatItem类可以格式化日志级别，
 * LoggerFormatItem类可以格式化日志名称，ThreadFormatItem类可以格式化线程ID，
 * FileFormatItem类可以格式化文件名，LineFormatItem类可以格式化行号，
 * MessageFormatItem类可以格式化日志内容，TabFormatItem类可以格式化制表符，
 * NewlineFormatItem类可以格式化换行符，OtherFormatItem类可以格式化原始字符。
*/

namespace log{
    // 定义格式化项的基类
    class FormatItem{
        public:
            using ptr = std::shared_ptr<FormatItem>;
            virtual ~FormatItem() = default;
            virtual void Format(std::ostream& out, const LogMsg& msg) = 0;  //  纯虚函数，格式化日志消息
    };

    // 日期格式化子类
    class TimeFormatItem : public FormatItem{
        public:
            explicit TimeFormatItem(const std::string& time);
            void Format(std::ostream& out, const LogMsg& msg) override;
        private:
            std::string _time;
    };

    // 日志级别格式化子类
    class LevelFormatItem : public FormatItem{
        public:
            void Format(std::ostream& out, const LogMsg& msg) override;
    };

    // 日志名称格式化子类
    class LoggerFormatItem : public FormatItem{
        public:
            void Format(std::ostream& out, const LogMsg& msg) override;
    };

    // 线程ID格式化子类
    class ThreadFormatItem : public FormatItem{
        public:
            void Format(std::ostream& out, const LogMsg& msg) override;
    };

    // 文件名格式化子类
    class FileFormatItem : public FormatItem{
        public:
            void Format(std::ostream& out, const LogMsg& msg) override;
    };

    // 行号格式化子类
    class LineFormatItem : public FormatItem{
        public:
            void Format(std::ostream& out, const LogMsg& msg) override;
    };

    // 日志内容格式化子类
    class MessageFormatItem : public FormatItem{
        public:
            void Format(std::ostream& out, const LogMsg& msg) override;
    };

    //制表符格式化子类
    class TabFormatItem : public FormatItem{
        public:
            void Format(std::ostream& out, const LogMsg& msg) override;
    };

    // 换行符格式化子类
    class NewlineFormatItem : public FormatItem{
        public:
            void Format(std::ostream& out, const LogMsg& msg) override;
    };

    //原始字符格式化子类
    class OtherFormatItem : public FormatItem{
        public:
            explicit OtherFormatItem(std::string str);
            void Format(std::ostream& out, const LogMsg& msg) override;
        private:
            std::string _str;  // 存储原始字符
    };
}

#endif
