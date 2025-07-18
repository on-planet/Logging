#pragma once
#ifndef __FORMATTER_H__
#define __FORMATTER_H__

#include <vector>
#include <memory>
#include <stdexcept>

#include "FormatItem.hpp"
#include "Message.hpp"

namespace log{

    class Formatter{
        public:
            using ptr = std::shared_ptr<Formatter>;
            Formatter(const std::string& pattern = "[%d{%H:%M:%S}][%p][%c][%t][%f:%l]%T%m%n");
            void Format(std::ostream& out, const LogMsg& msg) const;
            std::string Format(const LogMsg& msg) const;
        private:
            bool ParsePattern();

            static FormatItem::ptr CreateItem(const std::string& key, const std::string& value)
            {
                // 根据键值对创建对应的格式化项
                if (key == "d") {
                    return std::make_shared<TimeFormatItem>(value);
                } else if (key == "p") {
                    return std::make_shared<LevelFormatItem>();
                } else if (key == "c") {
                    return std::make_shared<LoggerFormatItem>();
                } else if (key == "t") {
                    return std::make_shared<ThreadFormatItem>();
                } else if (key == "f") {
                    return std::make_shared<FileFormatItem>();
                } else if (key == "l") {
                    return std::make_shared<LineFormatItem>();
                } else if (key == "m") {
                    return std::make_shared<MessageFormatItem>();
                } else if (key == "n") {
                    return std::make_shared<NewlineFormatItem>();
                } else if (key == "T") {
                    return std::make_shared<TabFormatItem>();
                }
                return std::make_shared<OtherFormatItem>(value);
            }
            std::string _pattern;  // 日志格式化模式
            std::vector<FormatItem::ptr> _items;  // 存储格式化项的向量
    };
}

#endif
