#pragma once

#ifndef __SINK_FACTORY_H__
#define __SINK_FACTORY_H__

#include <memory>
#include "LogSink.hpp"

/*
 * SinkFactory类用于创建日志接收器（Sink）的工厂类。
 * 它提供了一个静态方法createSink，可以根据指定的接收器类型和参数创建对应的日志接收器实例。
 * 通过使用SinkFactory，用户可以方便地创建不同类型的日志接收器，而无需关心具体的实现细节。
 * 例如，可以创建标准输出接收器、文件接收器或按大小轮转的接收器等。
*/

namespace log {

    // SinkFactory类用于创建日志接收器（Sink）的工厂类
    class SinkFactory {
        public:
            using ptr = std::shared_ptr<SinkFactory>;
            template<class SinkType, typename... Args>
            static LogSink::ptr createSink(Args&&... args) {
                // 创建指定类型的日志接收器
                return std::make_shared<SinkType>(std::forward<Args>(args)...);
            }
    };

}

#endif
