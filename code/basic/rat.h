#pragma once
#include "common.h"
#include "raw_attribute_store.h"

/// @brief RAT（Record Allocation Table）通过已使用和空闲Record池（两个容器）管理所有Record
class GIROPOLYFIT_API RAT : public RawAttributeStore
{
public:
    RAT() : RawAttributeStore(sizeof(RecordID)) {}

    bool is_full() const { return free_list_.is_nil(); }

    virtual void clear()
    {
        RawAttributeStore::clear();
        free_list_.forget();
    }
    /// @brief 取出当前的空闲ID
    RecordID new_record_id()
    {
        my_assert(!is_full());
        RecordID result = free_list_;
        // cell(free_list_)里面记录的是上一次delete时回收的record，而回收的record里面记录的是上上次回收的record
        free_list_ = cell(free_list_);
        cell(result).unfree();
        return result;
    }
    /// @brief 回收一条RecordID
    void delete_record_id(RecordID record)
    {
        RecordID &ref = cell(record);
        my_assert(!ref.is_free());
        ref = free_list_;
        ref.free();
        free_list_ = record;
    }
    /// @brief 增加容量
    virtual void grow()
    {
        RawAttributeStore::grow();
        unsigned int chunk = num_chunks() - 1;
        for (unsigned int i = 0; i < CHUNK_SIZE - 1; i++)
        {
            // 初始化一系列RecordID，确保在new id的时候free_list_会正确的前移
            cell(chunk, i) = RecordID(chunk, i + 1, true);
        }
        cell(chunk, CHUNK_SIZE - 1) = free_list_; // 确保当最后一条空闲id被使用后，free_list_被正确设为nil
        free_list_ = RecordID(chunk, 0);
    }

protected:
    RecordID &cell(unsigned int chunk, unsigned int offset)
    {
        return *reinterpret_cast<RecordID *>(
            RawAttributeStore::data(chunk, offset));
    }

    RecordID &cell(RecordID index)
    {
        return cell(index.chunk(), index.offset());
    }

    const RecordID &cell(unsigned int chunk, unsigned int offset) const
    {
        return *reinterpret_cast<RecordID *>(
            RawAttributeStore::data(chunk, offset));
    }

    const RecordID &cell(RecordID index) const
    {
        return cell(index.chunk(), index.offset());
    }

private:
    RecordID free_list_;

    friend class AttributeManager;
};