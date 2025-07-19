#include "Logger.hpp"
#include "AsyncLogger.hpp" // 引入异步日志器头文件
#include "SinkFactory.hpp"
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

// 测试同步日志器
void testSyncLogger() {
    // 1. 创建一个同步 Logger 实例
    auto sync_logger = std::make_shared<log::Logger>("sync_logger", log::LogLevel::Level::INFO);

    // 2. 使用 SinkFactory 创建 Sink 实例
    auto stdout_sink = log::SinkFactory::createSink<log::StdOutSink>();
    auto file_sink = log::SinkFactory::createSink<log::FileSink>("./../logfile/sync_log.txt");

    // 3. 将 Sink 添加到 Logger
    sync_logger->AddSink(stdout_sink);
    sync_logger->AddSink(file_sink);

    std::cout << "--- 开始同步日志测试 ---" << std::endl;

    // 4. 记录日志
    for (int i = 0; i < 10; ++i) {
        sync_logger->Info(__FILE__, __LINE__, "这是一条同步日志: ", i);
    }

    std::cout << "--- 同步日志测试结束 ---" << std::endl;
}

// 测试异步日志器
void testAsyncLogger() {
    // 1. 创建一个异步 Logger 实例
    auto async_logger = std::make_shared<log::AsyncLogger>("async_logger", log::LogLevel::Level::INFO);

    // 2. 使用 SinkFactory 创建 Sink 实例
    auto stdout_sink = log::SinkFactory::createSink<log::StdOutSink>();
    auto file_sink = log::SinkFactory::createSink<log::FileSink>("./../logfile/async_log.txt");

    // 3. 将 Sink 添加到 Logger
    async_logger->AddSink(stdout_sink);
    async_logger->AddSink(file_sink);

    std::cout << "--- 开始异步日志测试 ---" << std::endl;

    // 4. 在多个线程中并发记录日志
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([&async_logger, i]() {
            for (int j = 0; j < 10; ++j) {
                async_logger->Warn(__FILE__, __LINE__, "线程 ", i, " 的异步日志: ", j);
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "--- 异步日志提交完成，等待后台线程写入... ---" << std::endl;
    // 当 async_logger 离开作用域时，其析构函数会确保所有日志都被写入文件
}

int main() {
    testSyncLogger();
    std::cout << "\n";
    testAsyncLogger();

    // 主线程暂停一小会，以便观察异步日志的后台输出
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "\n程序退出。\n";

    return 0;
}