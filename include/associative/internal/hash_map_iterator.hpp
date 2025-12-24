#pragma once
#include <type_traits>

template <typename Map>
class HashMapIterator {
public:
    using map_type = Map;
    using bucket_type = map_type::bucket_type;
    using value_type = map_type::value_type;
    using size_type = map_type::size_type;
    using bucket_iterator = std::conditional_t<
        std::is_const_v<Map>,
        typename bucket_type::const_iterator,
        typename bucket_type::iterator
    >;

    // Constructor
    HashMapIterator(map_type* map, size_type bucket_index, bucket_iterator it)
        : m_map(map), m_bucket_index(bucket_index), m_it(it)
    {
        if (m_map && m_bucket_index < m_map->get_bucket_count() &&
            m_it == m_map->get_buckets()[m_bucket_index].end()) {
            advance_to_next_bucket();
        }
    }

    // Dereference
    value_type& operator*() const { return *m_it; }

    auto operator->() const -> std::conditional_t<
        std::is_const_v<Map>,
        const value_type*,
        value_type*
    > {
        return &(*m_it);
    }

    // Pre-increment
    HashMapIterator& operator++() {
        ++m_it;
        if (m_map && (m_bucket_index < m_map->get_bucket_count()) &&
            m_it == m_map->get_buckets()[m_bucket_index].end()) {
            advance_to_next_bucket();
        }
        return *this;
    }

    // Post-increment
    HashMapIterator operator++(int) {
        HashMapIterator temp = *this;
        ++(*this);
        return temp;
    }

    // Comparison
    bool operator==(const HashMapIterator& other) const {
        return m_map == other.m_map &&
               m_bucket_index == other.m_bucket_index &&
               (m_bucket_index == end_bucket_index() || m_it == other.m_it);
    }

    bool operator!=(const HashMapIterator& other) const {
        return !(*this == other);
    }

    static constexpr size_type end_bucket_index() { return static_cast<size_type>(-1); }

private:
    map_type* m_map = nullptr;
    size_type m_bucket_index = 0;
    bucket_iterator m_it;

    void advance_to_next_bucket() {
        if (!m_map) return;
        ++m_bucket_index;
        while (m_bucket_index < m_map->get_bucket_count()) {
            auto& bucket = m_map->get_buckets()[m_bucket_index];
            if (!bucket.empty()) {
                m_it = bucket.begin();
                return;
            }
            ++m_bucket_index;
        }
        // Reached end
        m_bucket_index = end_bucket_index();
    }
};
