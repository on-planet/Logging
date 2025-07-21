#ifndef __UTIL_H__
#define __UTIL_H__

#pragma once

#include <ctime>

#include <sys/stat.h>
#include <string>

/*
 * 提供了日期和文件操作的实用工具类。
 * Date类用于获取当前时间戳和转换为本地时间。

 * File类用于检查文件是否存在、获取文件路径、创建目录等操作。
 *  Date::Now() 返回当前时间的时间戳（time_t类型）。
 *  Date::GetTimeSet() 返回当前时间的 struct tm 结构。
 *  File::IsFileExist(const std::string& file_path) 检查指定文件是否存在。
 *  File::GetPath(const std::string & file_path) 获取指定文件的所在目录路径。
 *  File::CreateDir(const std::string & file_path) 创建指定路径的目录。
 *
 */

namespace log{

    class Date{
        public:
            //获取时间 获取当前时间戳 time_t类型
            static time_t Now()
            {
                return time(nullptr); // 返回当前时间的时间戳
            }

            //获取时间戳  将Date::Now() 返回的 time_t 类型（时间戳）转换为 struct tm 类
            static struct tm GetTimeSet()
            {
                struct tm t;
                const time_t time_stamp = Date::Now();
                localtime_r(&time_stamp, &t);   // 将时间戳转换为本地时间
                return t; // 返回本地时间的 struct tm 结构
            }
    };

    class File{
        public:
            // 获取当前时间的字符串格式
            static bool IsFileExist(const std::string& file_path){  // 检查文件是否存在
                struct stat st; // 获取文件状态  在Linux下有一个stat的系统调用，通过这个系统调用我们能够查看文件的状态信息，但是这个系统调用在文件不存在的情况下会调用失败，返回-1
                if (stat(file_path.c_str(), &st) == 0){ // 如果文件存在，stat函数返回0
                    return true;
                }
                return false;
            }

            // 获取文件的所在目录路径
            static std::string GetPath(const std::string & file_path){
                size_t pos = file_path.find_last_of("/\\"); // 查找最后一个斜杠或反斜杠的位置
                if (pos == std::string::npos) {
                    return "."; // 如果没有找到斜杠，返回当前目录 此时返回 "." 代表当前目录，符合大多数文件操作的习惯。如果返回空字符串，可能会导致后续路径拼接或文件操作出错。
                }
                return file_path.substr(0, pos + 1); // 返回从开始到最后一个斜杠的子字符串
            }

            //创建目录
            static void CreateDir(const std::string & file_path){
                // 检查目录是否存在
                if (IsFileExist(file_path) || file_path.empty()) {
                    return; // 如果目录已存在，则不需要创建
                }
                umask(0); // 设置文件创建掩码为0，允许创建任何权限的文件

                size_t pos = 0;

                while ((pos = file_path.find_first_of("/\\", pos)) != std::string::npos){ // 遍历整个路径字符串
                    std::string parent_dir = file_path.substr(0, pos); // 获取当前目录的父目录路径
                    if ((!parent_dir.empty()) && !IsFileExist(parent_dir)){ // 检查父目录 不空 且 不存在
                        mkdir(parent_dir.c_str(), 0755); // 创建目录，权限为755
                    }
                    pos++;
                }
                if (!IsFileExist(file_path)){ // 最后检查目标目录是否存在
                    mkdir(file_path.c_str(), 0755); // 创建目标目录，权限为755
                }
            }

    };

}

#endif