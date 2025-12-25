#pragma once

template <typename Set>
class HashSetIterator {
public:
    using set_type = Set;
    using bucket_type = set_type::bucket_type;
    using value_type = set_type::key_type;
    using size_type = set_type::size_type;

    using reference = std::conditional_t<
        std::is_const_v<set_type>,
        const value_type &,
        value_type &>;

    using pointer =
    std::conditional_t<
        std::is_const_v<set_type>,
        const value_type *,
        value_type *>;

    using bucket_iterator =
    std::conditional_t<
        std::is_const_v<set_type>,
        typename bucket_type::const_iterator,
        typename bucket_type::iterator
    >;

    // Constructor
    HashSetIterator(set_type *set, size_type bucket_index, bucket_iterator it)
        : m_set(set), m_bucket_index(bucket_index), m_it(it) {
        if (m_set &&
            m_bucket_index < m_set->get_bucket_count() &&
            m_it == m_set->get_buckets()[m_bucket_index].end()) {
            advance_to_next_bucket();
        }
    }

    // Dereference
    reference operator*() const { return *m_it; }
    pointer operator->() const { return &(*m_it); }


    // Pre-increment
    HashSetIterator &operator++() {
        ++m_it;
        if (m_set &&
            m_bucket_index < m_set->get_bucket_count() &&
            m_it == m_set->get_buckets()[m_bucket_index].end()) {
            advance_to_next_bucket();
        }
        return *this;
    }

    // Post-increment
    HashSetIterator operator++(int) {
        HashSetIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    // Comparison
    bool operator==(const HashSetIterator &other) const {
        return m_set == other.m_set &&
               m_bucket_index == other.m_bucket_index &&
               (m_bucket_index == end_bucket_index() || m_it == other.m_it);
    }

    bool operator!=(const HashSetIterator &other) const {
        return !(*this == other);
    }

    static constexpr size_type end_bucket_index() {
        return static_cast<size_type>(-1);
    }

private:
    set_type *m_set = nullptr;
    size_type m_bucket_index = 0;
    bucket_iterator m_it;

    void advance_to_next_bucket() {
        if (!m_set) return;

        ++m_bucket_index;
        while (m_bucket_index < m_set->get_bucket_count()) {
            auto &bucket = m_set->get_buckets()[m_bucket_index];
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
