#pragma warning(disable : 4355) // loggerStreamBuf中使用了未完成的类指针

#include "logger.h"
#define FLUSH std::flush
#define DEFAULT_FILE_NAME "GiroPolyfitLog"

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
{ // 实现观察者模式
    if (Logger::instance())
        Logger::instance()->unregister_client(this);
}

void CoutLogger::out_message(const std::string &message) { std::cout << "INFO: " << message << FLUSH; }
void CoutLogger::warn_message(const std::string &message) { std::cout << "WARNING: " << message << FLUSH; }
void CoutLogger::error_message(const std::string &message) { std::cout << "ERROR: " << message << FLUSH; }

// FileLogger
FileLogger::FileLogger() : log_file_(nil)
{
    std::string file_name = DEFAULT_FILE_NAME;
    set_file_name(file_name);
}

FileLogger::FileLogger(const std::string &file_name) : log_file_(nil)
{
    set_file_name(file_name);
}

FileLogger::~FileLogger()
{ // 实现观察者模式

    Logger::instance()->unregister_client(this);
    delete log_file_;
    log_file_ = nil;
}

void FileLogger::out_message(const std::string &message)
{
    // 输出日志信息的实现
    if (log_file_)
    {
        *log_file_ << "INFO: " << message << FLUSH;
    }
}

void FileLogger::warn_message(const std::string &message)
{
    // 警告日志信息的实现
    if (log_file_)
    {
        *log_file_ << "WARNING: " << message << FLUSH;
    }
}

void FileLogger::error_message(const std::string &message)
{
    // 错误日志信息的实现
    if (log_file_)
    {
        *log_file_ << "ERROR: " << message << FLUSH;
    }
}

const std::string FileLogger::get_log_file_name()
{
    return log_file_name_;
}

void FileLogger::set_file_name(const std::string &value)
{
    // 设置文件名的实现
    log_file_name_ = value;
    if (log_file_)
    {
        delete log_file_;
        log_file_ = nil;
    }
    if (log_file_name_.length() == 0)
        log_file_name_ = DEFAULT_FILE_NAME;
    log_file_ = new std::ofstream(log_file_name_);
    if (!log_file_)
    {
        abort("Failed to open log file: " + log_file_name_);
    }
}

// Logger
#ifdef GIROPOLYFIT_EXPORTS
Logger *Logger::instance_ = nil;
#endif

void Logger::initialize()
{
    std::lock_guard<std::mutex> lock(mutex_); // 预防一下多线程情况
    if (instance_)
    { // 防止重复初始化
        Logger::warn() << "Logger already initialized. Reinitializing..." << std::endl;
        delete instance_;
    }
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
    clients.erase(c); // 使用erase不会因为c被多次删除而发生崩溃
}

bool Logger::is_client(LoggerClient *c)
{
    return clients.find(c) != clients.end();
}

const std::string Logger::log_file_name()
{
    return file_client_->get_log_file_name();
}

Logger::Logger() : out_(this), warn_(this), error_(this)
{
    // 初始化输出流并且设置默认输出client为coutLogger
    default_client_ = new CoutLogger();
    register_client(default_client_);
    file_client_ = new FileLogger();
    register_client(file_client_);
}

Logger::~Logger()
{
    delete default_client_;
    default_client_ = nil;

    if (file_client_)
    {
        delete file_client_;
        file_client_ = nil;
    }
    // clients由set自动删除
}

bool Logger::rename_log_file(const std::string &file_name)
{
    file_client_ = new FileLogger(file_name);
    register_client(file_client_);
    return true;
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