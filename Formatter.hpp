#pragma once
#ifndef __FORMATTER_H__
#define __FORMATTER_H__

#include <vector>
#include <memory>

#include "FormatItem.hpp"
#include "Message.hpp"

/*
    * Formatter类用于格式化日志消息，将其转换为指定的字符串格式。
    * 它支持自定义格式化模式，可以包含时间戳、日志级别、日志名称、线程ID、文件名、行号和日志内容等信息。
    * 通过解析格式化模式字符串，Formatter可以动态创建对应的格式化项，并在格式化日志消息时调用这些项的Format方法。
    * 使用Formatter时，可以通过传入一个日志格式化模式字符串来指定日志的输出格式。
 */

namespace log{

    class Formatter{

        public:
            using ptr = std::shared_ptr<Formatter>; // 智能指针类型别名
            Formatter(std::string pattern);
            void Format(std::ostream& out, const LogMsg& msg) const; // 格式化日志消息并输出到指定的输出流中
            std::string Format(const LogMsg& msg) const; // 将日志消息格式化为字符串并返回

        private:
            bool ParsePattern(); // 解析日志格式化模式字符串，将其转换为对应的格式化项

            static FormatItem::ptr CreateItem(const std::string& key, const std::string& value); // 根据键和值创建对应的格式化项
            std::string _pattern;  // 日志格式化模式
            std::vector<FormatItem::ptr> _items;  // 存储格式化项的向量
    };
}

#endif
