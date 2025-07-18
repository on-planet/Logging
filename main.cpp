#include "Formatter.hpp"
#include "Message.hpp"
#include "Util.hpp"
#include "Level.hpp"
#include <iostream>

int main()
{
    log::LogMsg msg;
    msg.setTime_t(log::Date::Now());
    msg.setLevel(log::LogLevel::Level::WARN);
    msg.setFile("test.cpp");
    msg.setLine(38);
    msg.setLogger("root");
    msg.setPayload("测试日志");
    log::Formatter formatter("%d{%H:%M:%S}[%p][%c][%f:%l]%T%m%n");
    std::cout << formatter.Format(msg) << std::endl;
    return 0;
}