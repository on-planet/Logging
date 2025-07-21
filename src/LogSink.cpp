#include "../include/LogSink.hpp"
#include <stdexcept>

namespace log{

    void StdOutSink::LogtoSink(const char* data, size_t len){
        std::cout.write(data, len);  // 将日志消息输出到标准输出
        std::cout.flush();  // 刷新输出流，确保日志立即显示
    }

    FileSink::FileSink(const std::string& file_path) : _file_path(file_path) {
        if (!File::IsFileExist(File::GetPath(_file_path))) {  // 检查文件是否存在
            File::CreateDir(File::GetPath(_file_path));  // 如果不存在，创建目录
        }
        _file.open(_file_path, std::ios::app | std::ios::out);  // 打开文件，追加模式
        if (!_file.is_open()) {
            throw std::runtime_error("Failed to open log file: " + _file_path);  // 如果打开失败，抛出异常
        }
    }

    FileSink::~FileSink() {
        if (_file.is_open()) {
            _file.close();  // 确保在析构时关闭文件
        }
    }

    void FileSink::LogtoSink(const char* data, size_t len){
        _file.write(data, len);  // 将日志消息写入文件
        _file.flush();  // 刷新文件流，确保日志立即写入
    }

    RollBySizeSink::RollBySizeSink(const std::string& basename, size_t max_size) : _basename(basename), _max_size(max_size), _cur_size(0), _count(0){
        std::string file_name = GetFileName();  // 获取初始文件名
        _ofs.open(file_name, std::ios::app | std::ios::out);  // 打开文件，追加模式
        if (!_ofs.is_open()) {
            throw std::runtime_error("Failed to open log file: " + file_name);  // 如果打开失败，抛出异常
        }
    }

    RollBySizeSink::~RollBySizeSink(){
        if (_ofs.is_open()) {
            _ofs.close();  // 确保在析构时关闭文件
        }
    }

    void RollBySizeSink::LogtoSink(const char* data, size_t len){
        if (_cur_size + len > _max_size)
        {
            _ofs.close();
            std::string file_name = GetFileName();  // 获取新的文件名
            _ofs.open(file_name, std::ios::app | std::ios::out);  // 打开新的文件，追加模式
            if (!_ofs.is_open()) {
                throw std::runtime_error("Failed to open log file: " + file_name);  // 如果打开失败，抛出异常
            }
            _cur_size = 0;  // 重置当前文件大小
        }
        _ofs.write(data, len);  // 将日志消息写入文件
        _cur_size += len;
        _ofs.flush();  // 刷新文件流，确保日志立即写入
    }

    std::string RollBySizeSink::GetFileName() {
        std::string filename = _basename + "_" + std::to_string(_count) + ".log";  // 生成文件名
        _count++;  // 增加文件名计数器
        return filename;  // 返回生成的文件名
    }

}