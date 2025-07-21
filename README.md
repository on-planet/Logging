# Logging
本项目使用 CMake 构建,实现简单日志库, 支持同步和异步日志记录, 以及自定义日志格式。

```text
.
├── include/          # 存放所有头文件 (.hpp)
│   ├── AsyncLogger.hpp
│   ├── FormatItem.hpp
│   ├── Formatter.hpp
│   ├── Level.hpp
│   ├── Logger.hpp
│   ├── LogSink.hpp
│   ├── Message.hpp
│   ├── SinkFactory.hpp
│   └── Util.hpp
├── src/              # 存放所有源文件 (.cpp)
│   ├── FormatItem.cpp
│   ├── Formatter.cpp
│   └── LogSink.cpp
├── example/          # 存放示例代码
│   └── main.cpp
└── CMakeLists.txt    # 根 CMakeLists 文件
```

### 编译与运行

```bash
# 1. 创建构建目录
mkdir build
cd build

# 2. 生成构建文件
cmake ..

# 3. 编译项目
make

# 4. 运行示例程序
./logging_app
```

### 基本使用方法（同步日志）

```cpp
#include "Logger.hpp"
#include "SinkFactory.hpp"

void basic_usage() {
    // 1. 创建一个同步 Logger 实例
    auto logger = std::make_shared<log::Logger>("my_app");

    // 2. 使用工厂创建一个控制台 Sink
    auto console_sink = log::SinkFactory::createSink<log::StdOutSink>();

    // 3. 将 Sink 添加到 Logger
    logger->AddSink(console_sink);

    // 4. 记录日志
    logger->Info("这是一条信息日志。");
    logger->Warn("这是一条警告日志。");
}
```
### 异步日志使用方法
```cpp
#include "AsyncLogger.hpp"
#include "SinkFactory.hpp"
#include <thread>
#include <vector>

void async_usage() {
// 1. 创建一个异步 Logger 实例
auto async_logger = std::make_shared<log::AsyncLogger>("async_worker");

    // 2. 创建控制台和文件 Sink
    auto console_sink = log::SinkFactory::createSink<log::StdOutSink>();
    auto file_sink = log::SinkFactory::createSink<log::FileSink>("./app_log.txt");

    // 3. 添加 Sinks
    async_logger->AddSink(console_sink);
    async_logger->AddSink(file_sink);

    // 4. 在多个线程中并发记录日志
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([&async_logger, i]() {
            async_logger->Info(__FILE__, __LINE__, "来自线程 ", i, " 的消息。");
        });
    }

    for (auto& t : threads) {
        t.join();
    }
    // 当 async_logger 离开作用域时，其析构函数会确保所有日志都被写入文件
}
```

### 自定义日志格式
```cpp
#include "Logger.hpp"
#include "SinkFactory.hpp"
#include "Formatter.hpp"

void custom_format_usage() {
// 1. 定义格式化模板
// %d{%H:%M:%S} 时间 | %p 级别 | %c 日志器名 | %m 消息 | %n 换行
std::string pattern = "%d{%H:%M:%S} [%p] %c: %m%n";
auto formatter = std::make_shared<log::Formatter>(pattern);

    // 2. 创建 Logger 并设置新的 Formatter
    auto logger = std::make_shared<log::Logger>("custom_logger", log::LogLevel::Level::DEBUG, formatter);

    // 3. 添加 Sink
    auto console_sink = log::SinkFactory::createSink<log::StdOutSink>();
    logger->AddSink(console_sink);

    // 4. 记录日志
    logger->Debug("这是自定义格式的日志。");
}
```