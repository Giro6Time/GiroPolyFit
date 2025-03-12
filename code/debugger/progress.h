#pragma once

#include <set>
#include <memory>

#include "common.h"
#include "basic_types.h"
#include "basic.h"

class ProgressClient;

class GIROPOLYFIT_API Progress
{
public:
    Progress();
    virtual ~Progress();
    static std::shared_ptr<Progress> instance();

    virtual void notify(std::size_t new_val);

    inline void register_client(std::shared_ptr<ProgressClient> client);
    inline void unregister_client(std::shared_ptr<ProgressClient> client);

    void cancel() { canceled_ = true; }
    void clear_canceled() { canceled_ = false; }
    bool is_canceled() const { return canceled_; }

private:
    static std::shared_ptr<Progress> instance_;
    std::set<std::shared_ptr<ProgressClient>> clients_;
    bool canceled_;
};

/// @brief 仅用于被重载。每个client同时只能控制一个进度条
class GIROPOLYFIT_API ProgressClient
{
public:
    /// @brief 重载此函数以实现进度的具体表现形式
    /// @param new_val 增量
    virtual void notify(std::size_t new_val) = 0;
    virtual ~ProgressClient() {}
};

class GIROPOLYFIT_API ProgressLogger
{
public:
    ProgressLogger(std::size_t max_val = 100, const std::string &task_name = "", bool quiet = false);
    virtual ~ProgressLogger();

    /// @brief 更新进度
    virtual void notify(std::size_t new_val);
    /// @brief 进度加一
    virtual void next();
    /// @brief 进度加val
    virtual void next_val(std::size_t val);
    bool is_canceled() const
    {
        return Progress::instance()->is_canceled();
    }
    void reset() { notify(0); }
    void reset(std::size_t max_val);

protected:
    virtual void update();

private:
    std::size_t max_val_;
    std::string task_name_;
    std::size_t cur_val_;
    std::size_t cur_percent_;
    bool quiet_;
};
