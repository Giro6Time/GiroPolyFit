#pragma once

#include "common.h"
#include "basic.h"
#include "basic_types.h"
#include <iostream>

/// @brief 表示AttributeManager创建的一条记录（record）的唯一id
class GIROPOLYFIT_API RecordID
{
public:
    /// @brief 默认构造创建一个空的Record ID
    RecordID() : data_(~Numeric::uint32(0)) {};
    bool is_nil() const { return data_ == ~Numeric::uint32(0); }
    /// @brief 清空RecordID
    void forget() { data_ = ~Numeric::uint32(0); }
    RecordID(const RecordID &rhs) : data_(rhs.data_) {}
    RecordID &operator=(const RecordID &rhs)
    {
        data_ = rhs.data_;
        return *this;
    }

    /// @brief 比较块内偏移号和块号
    bool operator==(const RecordID &rhs) const;
    ///@brief 比较块内偏移号和块号
    bool operator!=(const RecordID &rhs) const;

protected:
    friend class RAT;
    friend class RawAttributeStore;
    friend class AttributeManager;

    // data_ 为32bit的数据，其中10位用于表示块内偏移（块大小为1024），20位表示块号，剩余2位为标志位
    enum
    {
        OFFSET_BITS = 10, // 块内偏移
        CHUNK_BITS = 20,  // 块号
    };

    enum
    {
        OFFSET_SHIFT = 0,
        MAX_OFFSET = (1 << OFFSET_BITS) - 1,        // 末尾10位为1
        OFFSET_MASK = (MAX_OFFSET << OFFSET_SHIFT), // OFFSET在32位中的位置，即末10位为1
    };

    enum
    {
        CHUNK_SHIFT = OFFSET_BITS,
        MAX_CHUNK = (1 << CHUNK_BITS) - 1,       // 末20位为1
        CHUNK_MASK = (MAX_CHUNK << CHUNK_SHIFT), // CHUNK在32位中的位置，即末10位到末30位
    };

    enum
    {
        FREE_SHIFT = CHUNK_BITS + OFFSET_BITS,
        FREE_MASK = (1 << FREE_SHIFT),
    };

    enum
    {
        MARKED_SHIFT = CHUNK_BITS + OFFSET_BITS + 1,
        MARKED_MASK = (1 << MARKED_SHIFT),
    };

    // 读写data接口

    RecordID(
        unsigned int chunk_in, unsigned int offset_in,
        bool free_in = false, bool marked_in = false)
    {
        my_assert(chunk_in <= MAX_CHUNK);
        my_assert(offset_in <= MAX_OFFSET);
        data_ = (chunk_in << CHUNK_SHIFT) |
                (offset_in << OFFSET_SHIFT);
        if (free_in)
        {
            free();
        }
        if (marked_in)
        {
            mark();
        }
    }

    RecordID(Numeric::uint32 data_in) : data_(data_in)
    {
    }

    unsigned int chunk() const
    {
        return ((data_ & CHUNK_MASK) >> CHUNK_SHIFT);
    }

    unsigned int offset() const
    {
        return ((data_ & OFFSET_MASK) >> OFFSET_SHIFT);
    }

    // 标志位

    bool is_free() const { return ((data_ & FREE_MASK) != 0); }
    void free() { data_ |= FREE_MASK; }
    void unfree() { data_ &= ~FREE_MASK; }

    bool is_marked() const { return ((data_ & MARKED_MASK) != 0); }
    void mark() { data_ |= MARKED_MASK; }
    void unmark() { data_ &= ~MARKED_MASK; }

    // 输出
    friend std::ostream &operator<<(
        std::ostream &out, const RecordID &r);

    std::ostream &print(std::ostream &out = std::cerr) const
    {
        if (is_nil())
        {
            out << "RecordId: nil" << std::endl;
        }
        else
        {
            out << "RecordId: chunk=" << chunk()
                << " offset=" << offset() << " "
                << (is_free() ? "free" : "!free") << " "
                << (is_marked() ? "marked" : "!marked");
        }
        return out;
    }

protected:
    Numeric::uint32 data_;
};

class GIROPOLYFIT_API Record
{
protected:
    const RecordID &record_id() const { return record_id_; }
    RecordID &record_id() { return record_id_; }
    void set_record_id(RecordID id) { record_id_ = id; }

    friend class AttributeManager;
    friend class RawAttributeStore;

private:
    RecordID record_id_;
};

inline bool RecordID::operator==(const RecordID &rhs) const
{
    return (chunk() == rhs.chunk()) &&
           (offset() == rhs.offset());
}

inline bool RecordID::operator!=(const RecordID &rhs) const
{
    return !((*this) == rhs);
}

inline std::ostream &operator<<(std::ostream &out, const RecordID &r)
{
    r.print(out);
    return out;
}
