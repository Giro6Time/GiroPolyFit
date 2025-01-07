#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <set>
#include <mutex>

#include "../common.h"
#include "../basic_types.h"
#include "basic.h"

#pragma once

class Logger;
class LoggerStream;
class LoggerStreamBuffer;

class LoggerStreamBuf : public std::stringbuf
{
public:
    LoggerStreamBuf(LoggerStream *loggerStream) : loggerStream_(loggerStream) {}

private:
    int sync();
    LoggerStream *loggerStream_;
};

class GIROPOLYFIT_API LoggerStream : public std::ostream
{
public:
    LoggerStream(Logger *logger);
    virtual ~LoggerStream();

protected:
    void notify(std::string &str);

private:
    Logger *logger_;

    friend class ::LoggerStreamBuf;
};

class GIROPOLYFIT_API LoggerClient
{
public:
    virtual void out_message(const std::string &message) = 0;
    virtual void warn_message(const std::string &message) = 0;
    virtual void error_message(const std::string &message) = 0;
    virtual ~LoggerClient();
};

class GIROPOLYFIT_API CoutLogger : public LoggerClient
{
public:
    CoutLogger();
    ~CoutLogger();
    /// @brief 输出INFO 此处直接flush，如果需要换行，则使用Logger::out() << endl或者在value中直接添加换行符
    void out_message(const std::string &message);
    /// @brief 输出WARNING 此处直接flush，如果需要换行，则使用Logger::warn() << endl或者在value中直接添加换行符
    void warn_message(const std::string &message);
    /// @brief 输出ERROR 此处直接flush，如果需要换行，则使用Logger::error() << endl或者在value中直接添加换行符
    void error_message(const std::string &message);
};

class FileLogger : public LoggerClient
{
public:
    FileLogger();
    FileLogger(const std::string &file_name);
    ~FileLogger();

    /// @brief 输出INFO 此处直接flush，如果需要换行，则使用Logger::out() << endl或者在value中直接添加换行符
    void out_message(const std::string &message);
    /// @brief 输出WARNING 此处直接flush，如果需要换行，则使用Logger::warn() << endl或者在value中直接添加换行符
    void warn_message(const std::string &message);
    /// @brief 输出ERROR 此处直接flush，如果需要换行，则使用Logger::error() << endl或者在value中直接添加换行符
    void error_message(const std::string &message);
    /// @return log的文件名
    const std::string get_log_file_name() { return log_file_name_; }

protected:
    void set_file_name(const std::string &value);

private:
    std::string log_file_name_;
    std::ostream *log_file_;
};

class GIROPOLYFIT_API Logger
{
public:
    static void initialize();
    static void terminate();
    Logger();
    ~Logger();

    static LoggerStream &out();
    static LoggerStream &warn();
    static LoggerStream &error();

    static Logger *instance()
    {
        if (!instance_) // singleton
        {
            // 预防一下多线程情况
            std::lock_guard<std::mutex> lock(mutex_);
            if (!instance_)
            { // Double-checked locking
                initialize();
            }
        }
        initialize();
        return instance_;
    }

    void register_client(LoggerClient *c);
    void unregister_client(LoggerClient *c);
    /// @brief 判断目标c是否已经注册
    bool is_client(LoggerClient *c);

    const std::string log_file_name();
    bool Logger::rename_log_file(const std::string& file_name);

protected:
    // fulush stream
    void notify(LoggerStream *from, std::string &message);

    LoggerStream &out_stream();
    LoggerStream &warn_stream();
    LoggerStream &error_stream();

    void notify_out(const std::string &message);
    void notify_warn(const std::string &message);
    void notify_error(const std::string &message);

private:
    static Logger *instance_;
    static std::mutex mutex_;

    LoggerClient *default_client_;
    FileLogger *file_client_; // FileClient不需要由外部对象来管理，因此直接由Logger管理。

    LoggerStream out_;
    LoggerStream warn_;
    LoggerStream error_;

    std::set<LoggerClient *> clients; // 已注册的clients (观察者模式)，当Logger的输出流收到消息时，将会通知所有的clients输出Log

    friend class LoggerStream;
};