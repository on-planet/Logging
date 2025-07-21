#pragma once

#ifndef __ASYNC_LOGGER_H__
#define __ASYNC_LOGGER_H__

#include "Logger.hpp"

#include <queue>
#include <thread>
#include <condition_variable>
#include <atomic>

namespace log
{
    // 异步日志记录器类，继承自 Logger
    class AsyncLogger : public Logger{
        public:
            using ptr = std::shared_ptr<AsyncLogger>;
            AsyncLogger(
                const std::string& name = "root",
                LogLevel::Level level = LogLevel::Level::UNKNOWN,
                Formatter::ptr formatter = nullptr,
                std::vector<LogSink::ptr> sinks = {}
                ): Logger(name, level, formatter, sinks), _running(true){
                _thread = std::thread(&AsyncLogger::consumeLogTask, this); // 启动工作线程
            }
            ~AsyncLogger() override{
                _running = false;
                _cond_var.notify_all();
                if (_thread.joinable()) {
                    _thread.join(); // 等待工作线程结束
                }
            }
        protected:
            void dispatchLog(const std::string& formatted_msg) override {
                {
                    // 异步处理日志消息
                    std::unique_lock<std::mutex> lock(_queue_mutex);
                    _log_queue.push(formatted_msg);
                }
                _cond_var.notify_one(); // 通知工作线程有新日志消息
            }

        private:
            void consumeLogTask(){
                while(_running){
                    std::unique_lock<std::mutex> lock(_queue_mutex);
                    _cond_var.wait(lock, [this]{
                        return !_log_queue.empty() || !_running; // 等待直到有日志消息或停止运行
                    });
                    if (!_running && _log_queue.empty()) {
                        return;; // 如果停止运行且队列为空，退出循环
                    }
                    std::string msg = std::move(_log_queue.front());
                    _log_queue.pop(); // 获取并移除队列中的日志消息
                    lock.unlock(); // 解锁互斥锁，以允许其他线程访问队列
                    for (auto& sink : _sinks){
                        sink->LogtoSink(msg.c_str(), msg.length()); // 将日志消息发送到所有接收器
                    }
                }
            }

            std::queue<std::string> _log_queue;
            std::mutex _queue_mutex;
            std::condition_variable _cond_var;
            std::thread _thread;
            std::atomic<bool> _running;
    };
}
#endif

