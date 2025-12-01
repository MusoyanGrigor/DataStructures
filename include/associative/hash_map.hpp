#pragma once

#include "sequence/vector.hpp"
#include "sequence/forward_list.hpp"
#include "utils/pair.hpp"

template <typename Key, typename Value>
class Hash_map {
public:
    // Constructors
    Hash_map() : m_buckets(4) ,m_bucket_count(4) {}

    Hash_map(const Hash_map &other) : m_buckets(other.m_buckets),m_bucket_count(other.m_bucket_count) {}

    Hash_map(Hash_map &&other) noexcept : m_buckets(std::move(other.m_buckets)),m_bucket_count(std::move(other.m_bucket_count)) {}

    explicit Hash_map(std::size_t num_buckets) : m_buckets(num_buckets), m_bucket_count(num_buckets) {}

    Hash_map(std::initializer_list<std::pair<Key, Value>> i_list, const std::size_t bucket_count = 4)
        : m_buckets(bucket_count), m_bucket_count(bucket_count) {
        for (const auto &i : i_list) {
            insert(i.first, i.second);
        }
    }


    // Assignment operator
    Hash_map & operator=(const Hash_map &other) {
        if (this != &other) {
            m_buckets = other.m_buckets;
            m_bucket_count = other.m_bucket_count;
        }
        return *this;
    }

    Hash_map &operator=(Hash_map &&other) noexcept {
        if (this != &other) {
            m_buckets = std::move(other.m_buckets);
            m_bucket_count = std::move(other.m_bucket_count);
        }
        return *this;
    }


    void insert(const Key& key, const Value& value) {
        std::size_t index = std::hash<Key>{}(key) % m_bucket_count;
        for (auto &kv : m_buckets[index]) {
            if (kv.first == key) {
                kv.second = value;
                return;
            }
        }
        m_buckets[index].emplace_front({key, value});
    }

    Value* find(const Key &key) {
        const std::size_t index = std::hash<Key>{}(key) % m_bucket_count;
        for (auto &kv : m_buckets[index]) {
            if (kv.first == key) {
                return &kv.second;
            }
        }
        return nullptr;
    }

    bool remove(const Key &key) {
        std::size_t index = std::hash<Key>{}(key) % m_bucket_count;
        auto &bucket = m_buckets[index];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->first == key) {
                bucket.erase(it);
                return true;
            }
        }
        return false;
    }

private:
    using Bucket = Forward_list<Pair<Key, Value>>;

    Vector<Bucket> m_buckets;
    std::size_t m_bucket_count;
};