
#pragma ocne
#include "common.h"
#include "basic_types.h"
#include "record_id.h"
#include "basic.h"

#include <vector>
#include <set>

class GIROPOLYFIT_API RawAttributeStore
{
public:
    enum
    {
        CHUNK_SIZE = RecordID::MAX_OFFSET + 1
    };

    RawAttributeStore(unsigned int item_size) : item_size_(item_size) {}
    virtual void clear() { data_.clear(); } // data_使用智能指针管理，无需手动delete
    virtual ~RawAttributeStore() { clear(); }
    unsigned int item_size() const { return item_size_; }
    unsigned int num_chunks() const { return (unsigned int)data_.size(); }
    unsigned int capacity() const
    {
        return (unsigned int)data_.size() * CHUNK_SIZE;
    }

    /// @brief 根据块号和块内偏移计算出目标数据的位置
    /// @return 目标数据的起始地址
    /// @note 勿对data进行delete操作
    Memory::byte *data(
        unsigned int chunk, unsigned int offset) const
    {
        my_assert(chunk < data_.size());
        my_assert(offset < CHUNK_SIZE);
        return &(data_[chunk].get()[offset * item_size_]);
    }

    /// @brief 根据记录计算出目标数据的位置
    /// @return 目标数据的起始位置
    /// @note 勿对data进行delete操作
    Memory::byte *data(const Record &r) const
    {
        return data(r.record_id().chunk(), r.record_id().offset());
    }

    virtual void grow()
    {
        std::shared_ptr<Memory::byte> chunk(new Memory::byte(CHUNK_SIZE * item_size_));
        data_.push_back(chunk);
    }

private:
    unsigned int item_size_;
    std::vector<std::shared_ptr<Memory::byte>> data_;
};
