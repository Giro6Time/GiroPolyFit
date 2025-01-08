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
    if (instance_)
    { // 防止重复初始化
        Logger::warn("Logger") << "Logger already initialized. Reinitializing..." << std::endl;
        delete instance_;
    }
    instance_ = new Logger();
    Logger::out("GiroPolyFit") << "Project initialized" << std::endl;
}

void Logger::terminate()
{
    Logger::out("GiroPolyFit") << "terminated... Bye!" << std::endl;
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
    log_everything_ = true;

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

LoggerStream &Logger::out(const std::string &feature)
{
    // assert instance != nil
    return instance_->out_stream(feature);
}

LoggerStream &Logger::warn(const std::string &feature)
{
    // assert instance != nil
    return instance_->warn_stream(feature);
}

LoggerStream &Logger::error(const std::string &feature)
{
    // assert instance != nil
    return instance_->error_stream(feature);
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

LoggerStream &Logger::out_stream(const std::string &feature)
{
    current_feature_ = feature;
    return out_;
}

LoggerStream &Logger::error_stream(const std::string &feature)
{
    current_feature_ = feature;
    return error_;
}

LoggerStream &Logger::warn_stream(const std::string &feature)
{
    current_feature_ = feature;
    return warn_;
}

void Logger::notify_out(const std::string &message)
{
    if ((log_everything_ && log_features_exclude_.find(current_feature_) == log_features_exclude_.end()) || // everything模式下不被排除
        log_features_.find(current_feature_) != log_features_.end())                                        // 或非everything模式下记录在log_features表中
    {

        std::set<LoggerClient *>::iterator it = clients.begin();
        if (current_feature_.empty())
            for (; it != clients.end(); it++)
            {
                (*it)->out_message(message);
            }
        else
            for (; it != clients.end(); it++)
            {
                (*it)->out_message("["+ current_feature_ + "] " + message);
            }
    }
}

void Logger::notify_warn(const std::string &message)
{
    if ((log_everything_ && log_features_exclude_.find(current_feature_) == log_features_exclude_.end()) || // everything模式下不被排除
        log_features_.find(current_feature_) != log_features_.end())                                        // 或非everything模式下记录在log_features表中
    {

        std::set<LoggerClient *>::iterator it = clients.begin();
        if (current_feature_.empty())
            for (; it != clients.end(); it++)
            {
                (*it)->warn_message(message);
            }
        else
            for (; it != clients.end(); it++)
            {
                (*it)->warn_message("["+ current_feature_ + "] " + message);
            }
    }
}

void Logger::notify_error(const std::string &message)
{
    if ((log_everything_ && log_features_exclude_.find(current_feature_) == log_features_exclude_.end()) || // everything模式下不被排除
        log_features_.find(current_feature_) != log_features_.end())                                        // 或非everything模式下记录在log_features表中
    {

        std::set<LoggerClient *>::iterator it = clients.begin();
        if (current_feature_.empty())
            for (; it != clients.end(); it++)
            {
                (*it)->error_message(message);
            }
        else
            for (; it != clients.end(); it++)
            {
                (*it)->error_message("["+ current_feature_ + "] " + message);
            }
    }
}

bool Logger::set_features(const std::string &features, const std::string &features_exclude)
{
    std::vector<std::string> features_tmp;
    String::split(features, ';', features_tmp);
    log_features_.clear();
    for (unsigned i = 0; i < features.size(); i++)
    {
        log_features_.insert(features_tmp[i]);
    }
    log_everything_ = (features_tmp.size() == 1 && ((features_tmp[0] == "*") || (features_tmp[0] == "Everything")));

    log_features_exclude_.clear();
    if (log_everything_ && features.length())
    {
        std::vector<std::string> features_exclude_tmp;
        String::split(features_exclude, ';', features_exclude_tmp);
        for (unsigned i = 0; i < features_exclude_tmp.size(); i++)
        {
            log_features_exclude_.insert(features_exclude_tmp[i]);
        }
    }
    return true;
}

bool Logger::get_features(std::string &features, std::string &features_exclude)
{
    features = features_exclude = "";
    std::set<std::string>::const_iterator it = log_features_.begin();
    for (; it != log_features_.end(); it++)
    {
        if (features.length() != 0)
            features += ';';
        features += *it;
    }
    it = log_features_exclude_.begin();
    for (; it != log_features_exclude_.end(); it++)
    {
        if (features_exclude.length() != 0)
            features_exclude += ';';
        features_exclude += *it;
    }

    return true;
}