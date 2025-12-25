#pragma once

#include <functional>
#include "sequence/vector.hpp"
#include "sequence/forward_list.hpp"
#include "internal/hash_set_iterator.hpp"

template <typename Key>
class Hash_set {
public:
    using key_type = Key;
    using size_type = size_t;
    using bucket_type = Forward_list<Key>;
    using iterator = HashSetIterator<Hash_set>;
    using const_iterator = HashSetIterator<const Hash_set>;

    // Constructors
    Hash_set() : m_buckets(4), m_bucket_count(4) {}

    Hash_set(const Hash_set &other) : m_buckets(other.m_buckets), m_bucket_count(other.m_bucket_count) {}

    Hash_set(Hash_set &&other) noexcept : m_buckets(std::move(other.m_buckets)), m_bucket_count(other.m_bucket_count) {}

    explicit Hash_set(size_type num_buckets) : m_buckets(num_buckets), m_bucket_count(num_buckets) {}

    Hash_set(std::initializer_list<key_type> i_list, const size_type bucket_count = 4)
        : m_buckets(bucket_count), m_bucket_count(bucket_count) {
        for (const auto &key : i_list) {
            insert(key);
        }
    }

    // Assignment
    Hash_set &operator=(const Hash_set &other) {
        if (this != &other) {
            m_buckets = other.m_buckets;
            m_bucket_count = other.m_bucket_count;
        }
        return *this;
    }

    Hash_set &operator=(Hash_set &&other) noexcept {
        if (this != &other) {
            m_buckets = std::move(other.m_buckets);
            m_bucket_count = other.m_bucket_count;
        }
        return *this;
    }

    Hash_set &operator=(std::initializer_list<key_type> i_list) {
        clear();
        for (const auto &key : i_list) insert(key);
        return *this;
    }

    // Capacity
    [[nodiscard]] size_type size() const {
        size_type count = 0;
        for (auto &bucket : m_buckets) count += bucket.size();
        return count;
    }

    [[nodiscard]] bool empty() const { return size() == 0; }

    void set_max_load_factor(double factor) { m_max_load_factor = factor; }
    [[nodiscard]] double max_load_factor() const { return m_max_load_factor; }
    [[nodiscard]] double load_factor() const { return static_cast<double>(size()) / m_bucket_count; }

    Vector<bucket_type>& get_buckets() {
        return m_buckets;
    }

    const Vector<bucket_type>& get_buckets() const {
        return m_buckets;
    }

    [[nodiscard]] const size_type& get_bucket_count() const {
        return m_bucket_count;
    }


    // Modifiers
    void insert(const key_type &key) {
        if (size() + 1 > m_bucket_count * m_max_load_factor) {
            rehash(m_bucket_count * 2);
        }

        size_type index = std::hash<key_type>{}(key) % m_bucket_count;
        for (const auto &k : m_buckets[index]) {
            if (k == key) return; // already exists
        }
        m_buckets[index].push_front(key);
    }

    bool contains(const key_type &key) const {
        size_type index = std::hash<key_type>{}(key) % m_bucket_count;
        for (const auto &k : m_buckets[index]) if (k == key) return true;
        return false;
    }

    bool remove(const key_type &key) {
        size_type index = std::hash<key_type>{}(key) % m_bucket_count;
        auto &bucket = m_buckets[index];
        auto prev = bucket.before_begin();
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (*it == key) {
                bucket.erase_after(prev);
                return true;
            }
            prev = it;
        }
        return false;
    }

    void clear() {
        for (auto &bucket : m_buckets) bucket.clear();
    }

    void swap(Hash_set &other) noexcept {
        using std::swap;
        swap(m_buckets, other.m_buckets);
        swap(m_bucket_count, other.m_bucket_count);
        swap(m_max_load_factor, other.m_max_load_factor);
    }

    // Iterators
    iterator begin() {
        for (size_type i = 0; i < m_bucket_count; ++i) {
            if (!m_buckets[i].empty()) return iterator(this, i, m_buckets[i].begin());
        }
        return end();
    }

    iterator end() {
        return iterator(this, iterator::end_bucket_index(), typename bucket_type::iterator{});
    }

    const_iterator begin() const {
        for (size_type i = 0; i < m_bucket_count; ++i) {
            if (!m_buckets[i].empty()) return const_iterator(this, i, m_buckets[i].begin());
        }
        return end();
    }

    const_iterator end() const {
        return const_iterator(
            this,
            iterator::end_bucket_index(),
            typename bucket_type::const_iterator{}
        );
    }


private:
    Vector<bucket_type> m_buckets;
    size_type m_bucket_count;
    double m_max_load_factor = 0.75;

    void rehash(size_type new_bucket_count) {
        Vector<bucket_type> new_buckets(new_bucket_count);
        for (auto &bucket : m_buckets) {
            for (auto &key : bucket) {
                size_type new_index = std::hash<key_type>{}(key) % new_bucket_count;
                new_buckets[new_index].push_front(key);
            }
        }
        m_buckets = std::move(new_buckets);
        m_bucket_count = new_bucket_count;
    }
};

// Swap utility
template <typename Key>
void swap(Hash_set<Key> &lhs, Hash_set<Key> &rhs) noexcept {
    lhs.swap(rhs);
}