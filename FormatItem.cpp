#include "FormatItem.hpp"

#include <utility>

namespace log {
    // 构造函数，接受一个时间格式字符串，默认为"%H:%M:%S"
    TimeFormatItem::TimeFormatItem(const std::string& time = "%H:%M:%S"): _time(time.empty() ? "%H:%M:%S": time) {}

    // 格式化日志消息，将时间戳转换为指定格式并输出到流中
    void TimeFormatItem::Format(std::ostream& out, const LogMsg& msg) {
        time_t ts = msg.getTime_t();  // 获取日志消息的时间戳
        struct tm t{};
        char buffer[64] = {0};
        localtime_r(&ts, &t);  // 将时间戳转换为本地时间
        strftime(buffer, sizeof(buffer), "%H:%M:%S", &t);
        out << buffer;  // 将格式化后的时间输出到流中
    }

    // 日志级别格式化子类
    void LevelFormatItem::Format(std::ostream& out, const LogMsg& msg){
        out << LogLevel::ToString(msg.getLevel());  // 输出源码行号
    }

    // 日志名称格式化子类
    void LoggerFormatItem::Format(std::ostream& out, const LogMsg& msg){
        out << msg.getLogger();  // 输出日志名称
    }

    // 线程ID格式化子类
    void ThreadFormatItem::Format(std::ostream& out, const LogMsg& msg){
        out << log::LogMsg::getThreadID();  // 输出线程ID
    }

    // 文件名格式化子类
    void FileFormatItem::Format(std::ostream& out, const LogMsg& msg){
        out << msg.getFile();  // 输出源码文件名
    }

    // 行号格式化子类
    void LineFormatItem::Format(std::ostream& out, const LogMsg& msg){
        out << msg.getLine();  // 输出源码行号
    }

    // 日志内容格式化子类
    void MessageFormatItem::Format(std::ostream& out, const LogMsg& msg){
        out << msg.getPayload();  // 输出日志内容
    }

    // 制表符格式化子类
    void TabFormatItem::Format(std::ostream& out, const LogMsg& msg){
        out << "\t";  // 输出制表符
    }

    // 换行符格式化子类
    void NewlineFormatItem::Format(std::ostream& out, const LogMsg& msg){
        out << "\n";  // 输出换行符
    }

    // 原始字符格式化子类
    void OtherFormatItem::Format(std::ostream& out, const LogMsg& msg){
        out << _str;  // 输出原始字符
    }

    // 构造函数，接受一个字符串参数
    OtherFormatItem::OtherFormatItem(std::string  str) : _str(std::move(str)) {
        // 初始化原始字符
    }
}