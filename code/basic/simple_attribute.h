#pragma once
#include "common.h"
#include "basic.h"
#include <string>
#include <unordered_map>
#include <typeindex>
#include <type_traits>
#include <memory>
#include <vector>
#include <any>

class AttributeStore; // 存储属性的核心类

/// @brief 属性访问代理（代理模式）
template <class T>
class AttributeHandle
{
public:
    ~AttributeHandle() = default;

    auto begin() { return store_->begin(name_); }
    auto end() { return store_->end(name_); }

    void bind(std::shared_ptr<AttributeStore> store, const std::string &name)
    {
        store_ = store;
        name_ = name;
    }

    std::shared_ptr<T> get(int id)
    {
        auto store = store_.lock(); // 获取 shared_ptr
        if (!store)
        {
            throw std::runtime_error("AttributeStore is no longer valid");
        }
        // 从 store_ 中获取属性值（std::shared_ptr<std::any>）
        auto value_ptr = store->get<T>(name_, id);
        // 使用 std::any_cast 提取实际类型为 T 的值，并封装到 std::shared_ptr<T> 中
        return std::make_shared<T>(std::any_cast<T>(*value_ptr));
    }

    void set(int id, const T &attr)
    {
        auto store = store_.lock();
        if (!store)
        {
            throw std::runtime_error("AttributeStore is no longer valid");
        }
        store->set(name_, id, std::any(attr));
    }

    // 一次性设置若干个属性
    void set_attributes(const std::unordered_map<int, T> &attributes)
    {
        auto store = store_.lock();
        if (!store)
        {
            throw std::runtime_error("AttributeStore is no longer valid");
        }
        store->set_attributes(name_, attributes);
    }

    // 删除指定属性
    void erase(int id)
    {
        auto store = store_.lock();
        if (!store)
        {
            throw std::runtime_error("AttributeStore is no longer valid");
        }
        store->erase(name_, id);
    }

    bool valid()
    {
        return !store_.expired();
    }

private:
    friend class AttributeStore;

    std::weak_ptr<AttributeStore> store_;
    std::string name_;
    // NOTE: 可以考虑添加一层缓存，暂存该handle已经读写过的属性
    // 如果属性名足够多，则可以尝试实现
};

/// @brief 属性存储仓库（组合模式 + 代理模式）
class AttributeStore : public std::enable_shared_from_this<AttributeStore>
{
public:
    AttributeStore() {}

protected:
    using Iterator = typename std::unordered_map<
        int,
        std::shared_ptr<std::any>>::iterator;
    using ConstIterator = typename std::unordered_map<
        int,
        std::shared_ptr<std::any>>::const_iterator;

    Iterator begin(const std::string &name)
    {
        return attributes_.at(name).begin();
    }

    Iterator end(const std::string &name)
    {
        return attributes_.at(name).end();
    }

    ConstIterator begin(const std::string &name) const
    {
        return attributes_.at(name).begin();
    }

    ConstIterator end(const std::string &name) const
    {
        return attributes_.at(name).end();
    }

    // getter setter
    template <typename T>
    std::shared_ptr<std::any> get(const std::string &name, int id)
    {
        try
        {
            return attributes_.at(name).at(id);
        }
        catch (const std::out_of_range &e)
        {
            attributes_[name][id] = std::make_shared<std::any>(T());
            return attributes_[name][id];
        }
    }

    const std::shared_ptr<std::any> get(const std::string &name, int id) const
    {
        return attributes_.at(name).at(id); // at() 更安全，直接抛出异常
    }

    void set(const std::string &name, int id, const std::any &value)
    {
        attributes_[name][id] = std::make_shared<std::any>(value);
    }

    // 一次性设置若干个属性
    template <class T>
    void set_attributes(const std::string &name, const std::unordered_map<int, T> &attributes)
    {
        for (const auto &[id, attr] : attributes)
        {
            set(name, id, std::any(attr));
        }
    }

    // 删除属性
    void erase(const std::string &name, int id)
    {
        // 由于采用sharedptr管理，所以无需关注IAttribute对象是否被删除。
        attributes_[name].erase(id);
        if (attributes_[name].empty())
        {
            attributes_.erase(name);
        }
    }

public:
    /// @brief 删除指定 ID 的所有属性
    void delete_all_attributes(int id)
    {
        std::vector<std::string> empty_attribute_names;

        // 第一步：遍历所有属性名，删除对应 ID
        for (auto it = attributes_.begin(); it != attributes_.end();)
        {
            auto &name_map = it->second;
            if (name_map.erase(id) > 0)
            { // 如果删除了元素
                if (name_map.empty())
                {
                    empty_attribute_names.push_back(it->first);
                }
            }
            ++it; // 安全递增迭代器
        }

        // 第二步：删除所有空属性名（避免迭代器失效）
        for (const auto &name : empty_attribute_names)
        {
            attributes_.erase(name);
        }
    }

    /// @brief 复制属性
    void copy_attributes(int from_id, int to_id)
    {
        // 遍历所有属性名
        for (auto &[name, id_map] : attributes_)
        {
            // 检查原顶点是否存在该属性
            if (id_map.count(from_id))
            {
                const auto &value = id_map.at(from_id);
                // 类型安全复制到新顶点
                id_map[to_id] = value;
            }
        }
    }

    /// @brief 创建访问attribute的句柄
    /// @tparam T attribute类型，如果与存储的实际类型不匹配将触发断言
    /// @param name 属性名
    template <class T>
    std::shared_ptr<AttributeHandle<T>> create_handle(const std::string &name)
    {
        // 检查该名称是否已关联其他类型
        auto it = name_to_type_.find(name);
#ifdef ATTRIBUTE_CHECK
        if (it != name_to_type_.end())
        {
            using decayed = std::decay_t<T>;
            my_assert(it->second == typeid(decayed));
        }
        else
        {
            // 新增类型映射
            using decayed = std::decay_t<T>;
            name_to_type_.emplace(name, std::type_index(typeid(decayed)));
        }
#endif

        auto handle = std::make_shared<AttributeHandle<T>>();
        handle->bind(shared_from_this(), name);
        return handle;
    }

    void clear()
    {
        attributes_.clear();
#ifdef ATTRIBUTE_CHECK
        name_to_type_.clear();
#endif
    }

    template <typename>
    friend class AttributeHandle;

private:
    std::unordered_map<std::string,
                       std::unordered_map<int, std::shared_ptr<std::any>>>
        attributes_;
#ifdef ATTRIBUTE_CHECK
    std::unordered_map<std::string, std::type_index> name_to_type_;
#endif
};
