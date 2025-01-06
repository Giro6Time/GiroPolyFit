#pragma warning(disable : 4355) // loggerStreamBuf中使用了未完成的类指针

#include "logger.h"
#define FLUSH std::flush

// LoggerStreamBUf

// 同步消息
int LoggerStreamBuf::sync()
{
    std::string str(this->str());
    // 通过str里面的关键字选择通知一个log流
    loggerStream_->notify(str);
    this->str("");
    return 0;
}

// LoggerStream

LoggerStream::LoggerStream(Logger *logger) : std::ostream(new LoggerStreamBuf(this)), logger_(logger)
{
}

LoggerStream::~LoggerStream()
{
    std::streambuf *buf = rdbuf();
    delete buf;
}

void LoggerStream::notify(std::string &str)
{
    logger_->notify(this, str);
}

// LoggerClient

LoggerClient::~LoggerClient()
{
}

// CoutLogger
CoutLogger::CoutLogger()
{
}

CoutLogger::~CoutLogger()
{
    // if (Logger::instance())
    // 	Logger::instance()->unregister_client(this);
}

void CoutLogger::out_message(const std::string &message) { std::cout << "INFO " << message << FLUSH; }
void CoutLogger::warn_message(const std::string &message) { std::cout << "WARNING " << message << FLUSH; }
void CoutLogger::error_message(const std::string &message) { std::cout << "ERROR " << message << FLUSH; }

#ifdef GIROPOLYFIT_EXPORTS
Logger *Logger::instance_ = nil;
#endif

void Logger::initialize()
{
    instance_ = new Logger();
    Logger::out() << "Project initialized" << std::endl;
}

void Logger::terminate()
{
    Logger::out() << "terminated... Bye!" << std::endl;
    // assert (instance_ != nil) ;
    delete instance_;
    instance_ = nil;
}

void Logger::register_client(LoggerClient *c)
{
    clients.insert(c);
}

void Logger::unregister_client(LoggerClient *c)
{
    clients.erase(c);
}

bool Logger::is_client(LoggerClient *c)
{
    return clients.find(c) != clients.end();
}

Logger::Logger() : out_(this), warn_(this), error_(this)
{
    // 初始化输出流并且设置默认输出client为coutLogger
    default_client_ = new CoutLogger();
    register_client(default_client_);
}

Logger::~Logger()
{
    delete default_client_;
    default_client_ = nil;
}

LoggerStream &Logger::out()
{
    // assert instance != nil
    return instance_->out_stream();
}

LoggerStream &Logger::warn()
{
    // assert instance != nil
    return instance_->warn_stream();
}

LoggerStream &Logger::error()
{
    // assert instance != nil
    return instance_->error_stream();
}

void Logger::notify(LoggerStream *s, std::string &message)
{
    if (s == &out_)
    {
        notify_out(message);
    }
    else if (s == &warn_)
    {
        notify_warn(message);
    }
    else if (s == &error_)
    {
        notify_error(message);
    }
    else
    {
        // assert not reached ;
    }
}

LoggerStream &Logger::out_stream()
{
    return out_;
}

LoggerStream &Logger::error_stream()
{
    return error_;
}

LoggerStream &Logger::warn_stream()
{
    return warn_;
}

void Logger::notify_out(const std::string &message)
{
    std::set<LoggerClient *>::iterator it = clients.begin();
    for (; it != clients.end(); it++)
    {
        (*it)->out_message(message);
    }
}

void Logger::notify_warn(const std::string &message)
{
    std::set<LoggerClient *>::iterator it = clients.begin();
    for (; it != clients.end(); it++)
    {
        (*it)->warn_message(message);
    }
}

void Logger::notify_error(const std::string &message)
{
    std::set<LoggerClient *>::iterator it = clients.begin();
    for (; it != clients.end(); it++)
    {
        (*it)->error_message(message);
    }
}