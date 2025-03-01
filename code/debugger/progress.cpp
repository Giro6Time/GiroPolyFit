#include "progress.h"

// Progress
std::shared_ptr<Progress> Progress::instance_ = nil;

Progress::Progress() : canceled_(false) {}

Progress::~Progress() { clients_.clear(); }

std::shared_ptr<Progress> Progress::instance()
{
    if (!instance_)
    {
        instance_ = std::make_shared<Progress>();
    }
    return instance_;
}

void Progress::notify(std::size_t new_val)
{
    for (auto it = clients_.begin(); it != clients_.end(); it++)
    {
        (*it)->notify(new_val);
    }
}

inline void Progress::register_client(std::shared_ptr<ProgressClient> client)
{
    clients_.insert(client);
}

inline void Progress::unregister_client(std::shared_ptr<ProgressClient> client)
{
    clients_.erase(client);
}

// ProgressLogger

ProgressLogger::ProgressLogger(std::size_t max_val, const std::string &task_name, bool quiet)
    : max_val_(max_val), task_name_(task_name), quiet_(quiet), cur_val_(0), cur_percent_(0)
{
    if (!quiet)
    {
        Progress::instance()->notify(0);
    }
}

ProgressLogger::~ProgressLogger()
{
    Progress::instance()->notify(100);
    Progress::instance()->notify(0);
}

void ProgressLogger::notify(std::size_t new_val)
{
    cur_val_ = new_val;
    update();
}

void ProgressLogger::next()
{
    cur_val_++;
    update();
}

void ProgressLogger::next_val(std::size_t val)
{
    cur_val_ += val;
    update();
}

void ProgressLogger::update()
{
    std::size_t percent = cur_val_ * 100 / Numeric::max<std::size_t>(1, max_val_ - 1);

    if (percent != cur_percent_)
    {
        cur_percent_ = percent;

        if (!quiet_)
        {
            Progress::instance()->notify(Numeric::min<std::size_t>(cur_percent_, 100));
        }
    }
}

void ProgressLogger::reset(std::size_t max_val)
{
    max_val_ = max_val;
    reset();
}
