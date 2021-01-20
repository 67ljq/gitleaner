#pragma once

#include <iterator>
#include <stdexcept>

namespace ljq
{
    template <typename _Ty, std::size_t _Nm>
    struct array
    {
    public:
        using value_type = _Ty;
        using reference = value_type &;
        using const_reference = const value_type &;
        using pointer = value_type *;
        using const_pointer = const value_type *;
        using iterator = value_type *;
        using const_iterator = const value_type *;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
        using size_type = std::size_t;
        using difference_type = ptrdiff_t;

    public:
        // Iterators
        iterator begin() noexcept { return iterator(_Em); }
        constexpr const_iterator begin() const noexcept { return const_iterator(_Em); }
        iterator end() noexcept { return iterator(_Em + _Nm); }
        constexpr const_iterator end() const noexcept { return const_iterator(_Em + _Nm); }
        reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
        constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
        reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
        constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
        constexpr const_iterator cbegin() const noexcept { return const_iterator(_Em); }
        constexpr const_iterator cend() const noexcept { return const_iterator(_Em + _Nm); }
        constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }
        constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }

        // Capacity
        constexpr size_type size() const noexcept { return _Nm; }
        constexpr size_type max_size() const noexcept { return _Nm; }
        constexpr bool empty() const noexcept { return _Nm == 0; }

        // Element access
        reference operator[](size_type _n) noexcept { return _Em[_n]; }
        constexpr const_reference operator[](size_type _n) const noexcept { return _Em[_n]; }
        reference at(size_type _n)
        {
            if (_n >= _Nm)
                throw std::out_of_range("arr.at out of range _n > size");
            return _Em[_n];
        }
        constexpr const_reference at(size_type _n) const
        {
            if (_n >= _Nm)
                throw std::out_of_range("arr.at out of range _n > size");
            return _Em[_n];
        }
        reference front() noexcept { return _Em[0]; }
        constexpr const_reference front() const noexcept { return _Em[0]; }
        reference back() noexcept { return _Em[_Nm - 1]; }
        constexpr const_reference back() const noexcept { return _Em[_Nm - 1]; }
        pointer data() noexcept { return _Em; }
        constexpr const_pointer data() const noexcept { return _Nm; }

        // Modifiers
        void fill(const value_type &_x) { std::fill_n(begin(), size(), _x); }
        void swap(array &_a) { std::swap_ranges(begin(), end(), _a.begin()); }

    public:
        value_type _Em[_Nm];
    };

} // namespace ljq