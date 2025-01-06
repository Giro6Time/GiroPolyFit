#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <set>

#include "../common.h"
#include "../basic_types.h"

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

    static Logger *instance() { return instance_; }

    void register_client(LoggerClient *c);
    void unregister_client(LoggerClient *c);
    /// @brief 判断目标c是否已经注册
    bool is_client(LoggerClient *c);

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

    LoggerClient *default_client_;

    LoggerStream out_;
    LoggerStream warn_;
    LoggerStream error_;

    std::set<LoggerClient *> clients; // 已注册的clients (观察者模式)

    friend class LoggerStream;
};