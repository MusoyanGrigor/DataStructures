#pragma once

#include "sequence/vector.hpp"
#include "sequence/forward_list.hpp"
#include "utils/pair.hpp"

template <typename Key, typename Value>
class Hash_map {
public:
    using key_type = Key;
    using mapped_type = Value;
    using value_type = Pair<Key, Value>;
    using size_type = size_t;
    using bucket_type = Forward_list<Pair<Key, Value>>;

    // Constructors
    Hash_map() : m_buckets(4) ,m_bucket_count(4) {}

    Hash_map(const Hash_map &other) : m_buckets(other.m_buckets),m_bucket_count(other.m_bucket_count) {}

    Hash_map(Hash_map &&other) noexcept : m_buckets(std::move(other.m_buckets)),m_bucket_count(other.m_bucket_count) {}

    explicit Hash_map(size_type num_buckets) : m_buckets(num_buckets), m_bucket_count(num_buckets) {}

    Hash_map(std::initializer_list<value_type> i_list, const size_type bucket_count = 4)
        : m_buckets(bucket_count), m_bucket_count(bucket_count) {
        for (const auto &i : i_list) {
            insert(i.first, i.second);
        }
    }

    // Assignment operator
    Hash_map &operator=(const Hash_map &other) {
        if (this != &other) {
            m_buckets = other.m_buckets;
            m_bucket_count = other.m_bucket_count;
        }
        return *this;
    }

    Hash_map &operator=(Hash_map &&other) noexcept {
        if (this != &other) {
            m_buckets = std::move(other.m_buckets);
            m_bucket_count = other.m_bucket_count;
        }
        return *this;
    }

    Hash_map &operator=(std::initializer_list<std::pair<key_type, mapped_type>> i_list) {
        for (auto &bucket : m_buckets) bucket.clear();
        for (const auto &i : i_list) insert(i.first, i.second);

        return *this;
    }

    // Element access
    mapped_type& operator[](const key_type &key) {
        mapped_type* value = find(key);
        if (value) return *value;
        insert(key, mapped_type{});
        return *find(key);
    }

    mapped_type& operator[](key_type &&key) {
        mapped_type* value = find(key);
        if (value) return *value;
        insert(key, mapped_type{});
        return *find(key);
    }

    mapped_type& at(const key_type &key) {
        mapped_type* value = find(key);
        if (!value) throw std::out_of_range("Key not found");
        return *value;
    }

    const mapped_type& at(const key_type &key) const {
        const mapped_type* value = find(key);
        if (!value) throw std::out_of_range("Key not found");
        return *value;
    }

    // Capacity
    [[nodiscard]] size_type size() const {
        size_type count = 0;
        for (auto &bucket : m_buckets) {
            count += bucket.size();
        }

        return count;
    }

    [[nodiscard]] bool empty() const {
        return size() == 0;
    }

    [[nodiscard]] size_type max_size() const {
        return std::numeric_limits<size_type>::max();
    }

    void set_max_load_factor(const double factor) {
        m_max_load_factor = factor;
    }

    double max_load_factor() const {
        return m_max_load_factor;
    }

    double load_factor() const {
        return static_cast<double>(size()) / m_bucket_count;
    }

    // Modifiers and lookup
    void insert(const key_type& key, const mapped_type& value) {
        if (size() + 1 > m_bucket_count * m_max_load_factor) {
            rehash(m_bucket_count * 2);
        }

        const size_type index = std::hash<key_type>{}(key) % m_bucket_count;
        for (auto &kv : m_buckets[index]) {
            if (kv.first == key) {
                kv.second = value;
                return;
            }
        }
        m_buckets[index].emplace_front({key, value});
    }

    mapped_type* find(const key_type &key) {
        const size_type index = std::hash<key_type>{}(key) % m_bucket_count;
        for (auto &kv : m_buckets[index]) {
            if (kv.first == key) {
                return &kv.second;
            }
        }
        return nullptr;
    }

    const mapped_type* find(const key_type &key) const {
        const size_type index = std::hash<key_type>{}(key) % m_bucket_count;
        for (auto &kv : m_buckets[index]) {
            if (kv.first == key) {
                return &kv.second;
            }
        }
        return nullptr;
    }

    bool contains(const key_type &key) const {
        return find(key) != nullptr;
    }

    bool remove(const key_type &key) {
        const size_type index = std::hash<key_type>{}(key) % m_bucket_count;
        auto &bucket = m_buckets[index];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->first == key) {
                bucket.erase(it);
                return true;
            }
        }
        return false;
    }

    void clear() {
        for (auto &bucket : m_buckets) {
            bucket.clear();
        }
    }

    void swap(Hash_map &other) noexcept {
        using std::swap;
        swap(m_buckets, other.m_buckets);
        swap(m_bucket_count, other.m_bucket_count);
        swap(m_max_load_factor, other.m_max_load_factor);
    }

private:
    Vector<bucket_type> m_buckets;
    size_type m_bucket_count;
    double m_max_load_factor = 0.75;

    void rehash(size_type new_bucket_count) {
        Vector<bucket_type> new_buckets(new_bucket_count);

        for (auto &bucket : m_buckets) {
            for (auto &kv : bucket) {
                size_type new_index = std::hash<key_type>{}(kv.first) % new_bucket_count;
                new_buckets[new_index].emplace_front(kv);
            }
        }
        m_buckets = std::move(new_buckets);
        m_bucket_count = new_bucket_count;
    }
};

template <typename Key, typename Value>
void swap(Hash_map<Key, Value> &lhs, Hash_map<Key, Value> &rhs) noexcept {
    lhs.swap(rhs);
}