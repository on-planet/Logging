#include "../include/Formatter.hpp"
#include "../include/FormatItem.hpp"

#include <sstream>

namespace log{

    // 构造函数，接受一个日志格式化模式字符串，并解析该模式
    Formatter::Formatter(std::string pattern = "[%d{%H:%M:%S}][%p][%c][%t][%f:%l]%T%m%n") : _pattern(pattern) {
        if (!ParsePattern()) {
            throw std::runtime_error("Failed to parse pattern: " + _pattern);
        }
    }

    // Format方法，接受一个输出流和一个LogMsg对象，将日志消息格式化并输出到流中
    void Formatter::Format(std::ostream& out, const LogMsg& msg) const{
        for (auto& item: _items) {
            if (item) { // 检查格式化项是否有效
                item->Format(out, msg);  // 调用每个格式化项的Format方法
            }
        }
    }

    // Format方法，接受一个LogMsg对象，将其格式化为字符串并返回
    std::string Formatter::Format(const LogMsg& msg) const{
        std::stringstream ssm;
        Format(ssm, msg); // 将日志消息格式化为字符串
        return ssm.str(); // 返回格式化后的字符串
    }

    // 解析日志格式化模式字符串，将其转换为对应的格式化项
    bool Formatter::ParsePattern(){
        std::string str;  // 用于存储键
        for (size_t i = 0; i < _pattern.size(); i++){
            if (_pattern[i] != '%'){
                str.append(1, _pattern[i]);  // 如果不是'%'，则直接添加到str
                continue;
            }
            if ((i + 1) < _pattern.size() && _pattern[i + 1] == '%') {
                str.append(1, '%');  // 如果是'%%'，则添加一个'%'
                i++;  // 跳过下一个字符
                continue;
            }
            if (!str.empty()) {
                // 如果str不为空，创建一个OtherFormatItem并添加到_items
                _items.push_back(CreateItem("o", str));
                str.clear();  // 清空str
            }
            i++;  // 跳过'%'字符
            if (i >= _pattern.size()) {
                return false;  // 如果已经到达字符串末尾，返回false
            }
            std::string key(1, _pattern[i]);  // 获取下一个字符作为键
            std::string value;  // 用于存储值
            if ((i + 1) < _pattern.size() && _pattern[i + 1] == '{'){
                size_t end_bracket = _pattern.find('}', i + 2);  // 查找结束括号
                if (end_bracket == std::string::npos){ // 如果没有找到结束括号，返回false
                    return false;
                }
                value = _pattern.substr(i + 2, end_bracket - i - 2);  // 获取键对应的值
                i = end_bracket;  // 更新索引到结束括号的位置
            }
            _items.push_back(CreateItem(key, value)); // 根据键和值创建对应的格式化项
        }
        if (!str.empty()) {
            // 如果str不为空，创建一个OtherFormatItem并添加到_items
            _items.push_back(CreateItem("o", str));
        }
        return true;
    }

    FormatItem::ptr Formatter::CreateItem(const std::string& key, const std::string& value){
        // 根据键值对创建对应的格式化项
        if (key == "d") {
            return std::make_shared<TimeFormatItem>(value);
        }
        if (key == "p") {
            return std::make_shared<LevelFormatItem>();
        }
        if (key == "c") {
            return std::make_shared<LoggerFormatItem>();
        }
        if (key == "t") {
            return std::make_shared<ThreadFormatItem>();
        }
        if (key == "f") {
            return std::make_shared<FileFormatItem>();
        }
        if (key == "l") {
            return std::make_shared<LineFormatItem>();
        }
        if (key == "m") {
            return std::make_shared<MessageFormatItem>();
        }
        if (key == "n") {
            return std::make_shared<NewlineFormatItem>();
        }
        if (key == "T") {
            return std::make_shared<TabFormatItem>();
        }
        return std::make_shared<OtherFormatItem>(value);
    }

}