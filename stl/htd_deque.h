#pragma once

#include <memory>
#include <algorithm>
#include <iterator>
#include <exception>
#include <initializer_list>

namespace htd
{
    const int deque_buf_size = 12;

    template <typename _Ty>
    class const_deque_iterator;

    template <typename _Ty>
    class deque_iterator
    {
    public:
        using value_type = _Ty;
        using reference = value_type &;
        using pointer = value_type *;
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = ptrdiff_t;
        using size_type = size_t;

        using _self = deque_iterator;
        using _elem_pointer = value_type *;
        using _map_pointer = _elem_pointer *;

        _elem_pointer _cur;
        _elem_pointer _start;
        _elem_pointer _end;
        _map_pointer _node;

        deque_iterator() noexcept : _cur(nullptr), _start(nullptr), _end(nullptr), _node(nullptr) {}
        deque_iterator(_elem_pointer start, _elem_pointer end, _map_pointer node) noexcept : _cur(start), _start(start), _end(end), _node(node) {}
        deque_iterator(const _self &_x) noexcept : _cur(_x._cur), _start(_x._start), _end(_x._end), _node(_x._node) {}
        deque_iterator(const const_deque_iterator<_Ty> &_x) noexcept : _cur(_x._cur), _start(_x._start), _end(_x._end), _node(_x._node) {}

        reference operator*() const noexcept { return *_cur; }
        pointer operator->() const noexcept { return _cur; }
        bool operator==(const deque_iterator &_x) const noexcept { return _x._cur == _cur; }
        bool operator!=(const deque_iterator &_x) const noexcept { return _x._cur != _cur; }
        _self &operator++() noexcept
        {
            _cur++;
            if (_cur == _end)
            {
                difference_type diff = _end - _start;
                _node++;
                _start = *_node;
                _end = _start + diff;
                _cur = _start;
            }
            return *this;
        }
        _self operator++(int) noexcept
        {
            _self tmp = *this;
            ++*this;
            return tmp;
        }
        _self &operator--() noexcept
        {
            if (_cur == _start)
            {
                difference_type diff = _end - _start;
                _node--;
                _start = *_node;
                _end = _start + diff;
                _cur = _end;
            }
            _cur--;

            return *this;
        }
        _self operator--(int) noexcept
        {
            _self tmp = *this;
            --*this;
            return tmp;
        }
        _self &operator+=(difference_type _n) noexcept
        {
            const difference_type diff = _end - _start;
            const difference_type _offset = _n + (_cur - _start);
            if (_offset >= 0 && _offset < diff)
                _cur += _n;
            else if (_offset >= diff)
            {
                difference_type _node_offset = _offset / diff;
                _node += _node_offset;
                _start = *_node;
                _end = _start + diff;
                _cur = _start + (_offset - _node_offset * diff);
            }
            else if (_offset < 0)
            {
                difference_type _node_offset = (_offset / diff);
                _node_offset = _offset % diff == 0 ? _node_offset : _node_offset - 1;
                _node += _node_offset;
                _start = *_node;
                _end = _start + diff;
                _cur = _start + (_offset - _node_offset * diff);
            }

            return *this;
        }
        _self operator+(difference_type _n) const noexcept
        {
            _self tmp = *this;
            return tmp += _n;
        }
        _self &operator-=(difference_type _n) noexcept { return *this += -_n; }
        _self operator-(difference_type _n) const noexcept
        {
            _self tmp = *this;
            return tmp -= _n;
        }
        difference_type operator-(const _self &_x) const noexcept
        {
            const difference_type diff = _end - _start;
            return diff * (_node - _x._node - 1) + (_cur - _start) + (_x._end - _x._cur);
        }
        reference operator[](difference_type _n) noexcept { return *(*this + _n); }
        bool operator<(_self &_x) { return (_node == _x._node) ? (_cur < _x._cur) : (_node < _x._node); }
        bool operator>(_self &_x) { return _x < (*this); }
        bool operator<=(_self &_x) { return !((*this) > _x); }
        bool operator>=(_self &_x) { return !((*this) < _x); }
    };

    template <typename _Ty>
    class const_deque_iterator
    {
    public:
        using value_type = _Ty;
        using reference = const value_type &;
        using pointer = const value_type *;
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = ptrdiff_t;
        using size_type = size_t;

        using _self = const_deque_iterator;
        using _elem_pointer = value_type *;
        using _map_pointer = _elem_pointer *;

        _elem_pointer _cur;
        _elem_pointer _start;
        _elem_pointer _end;
        _map_pointer _node;

        const_deque_iterator() noexcept : _cur(nullptr), _start(nullptr), _end(nullptr), _node(nullptr) {}
        const_deque_iterator(_elem_pointer start, _elem_pointer end, _map_pointer node) noexcept : _cur(start), _start(start), _end(end), _node(node) {}
        const_deque_iterator(const _self &_x) noexcept : _cur(_x._cur), _start(_x._start), _end(_x._end), _node(_x._node) {}
        const_deque_iterator(const deque_iterator<_Ty> &_x) noexcept : _cur(_x._cur), _start(_x._start), _end(_x._end), _node(_x._node) {}

        reference operator*() const noexcept { return *_cur; }
        pointer operator->() const noexcept { return _cur; }
        bool operator==(const _self &_x) const noexcept { return _x._cur == _cur; }
        bool operator!=(const _self &_x) const noexcept { return _x._cur != _cur; }
        _self &operator++() noexcept
        {
            _cur++;
            if (_cur == _end)
            {
                difference_type diff = _end - _start;
                _node++;
                _start = *_node;
                _end = _start + diff;
                _cur = _start;
            }
            return *this;
        }
        _self operator++(int) noexcept
        {
            _self tmp = *this;
            ++*this;
            return tmp;
        }
        _self &operator--() noexcept
        {
            if (_cur == _start)
            {
                difference_type diff = _end - _start;
                _node--;
                _start = *_node;
                _end = _start + diff;
                _cur = _end;
            }
            _cur--;

            return *this;
        }
        _self operator--(int) noexcept
        {
            _self tmp = *this;
            --*this;
            return tmp;
        }
        _self &operator+=(difference_type _n) noexcept
        {
            const difference_type diff = _end - _start;
            const difference_type _offset = _n + (_cur - _start);
            if (_offset >= 0 && _offset < diff)
                _cur += _n;
            else if (_offset >= diff)
            {
                difference_type _node_offset = _offset / diff;
                _node += _node_offset;
                _start = *_node;
                _end = _start + diff;
                _cur = _start + (_offset - _node_offset * diff);
            }
            else if (_offset < 0)
            {
                difference_type _node_offset = (_offset / diff) - 1;
                _node_offset = _offset % diff == 0 ? _node_offset : _node_offset - 1;
                _node += _node_offset;
                _start = *_node;
                _end = _start + diff;
                _cur = _start + (_offset - _node_offset * diff);
            }

            return *this;
        }
        _self operator+(difference_type _n) const noexcept
        {
            _self tmp = *this;
            return tmp += _n;
        }
        _self &operator-=(difference_type _n) noexcept { return *this += -_n; }
        _self operator-(difference_type _n) const noexcept
        {
            _self tmp = *this;
            return tmp -= _n;
        }
        difference_type operator-(const _self &_x) const noexcept
        {
            const difference_type diff = _end - _start;
            return diff * (_node - _x._node - 1) + (_cur - _start) + (_x._end - _x._cur);
        }
        reference operator[](difference_type _n) noexcept { return *(*this + _n); }
        bool operator<(_self &_x) { return (_node == _x._node) ? (_cur < _x._cur) : (_node < _x._node); }
        bool operator>(_self &_x) { return _x < (*this); }
        bool operator<=(_self &_x) { return !((*this) > _x); }
        bool operator>=(_self &_x) { return !((*this) < _x); }
    };

    template <typename _Ty, typename _Alloc = std::allocator<_Ty>>
    class deque
    {
    public:
        using value_type = _Ty;
        using allocator_type = _Alloc;
        using reference = value_type &;
        using const_reference = const value_type &;
        using pointer = typename std::allocator_traits<allocator_type>::pointer;
        using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;
        using iterator = deque_iterator<value_type>;
        using const_iterator = const_deque_iterator<value_type>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
        using difference_type = ptrdiff_t;
        using size_type = size_t;

    public:
        deque();
        deque(size_type _n);
        deque(size_type _n, const value_type &_value);
        template <typename _Input_iter, typename = std::_RequireInputIter<_Input_iter>>
        deque(_Input_iter first, _Input_iter last);
        deque(std::initializer_list<value_type> _l);
        deque(const deque &_x);
        deque(deque &&_x);
        ~deque();

        deque &operator=(const deque &_x);
        deque &operator=(deque &&_x);
        deque &operator=(std::initializer_list<value_type> _l);

        // Iterator
        iterator begin() noexcept { return start; }
        const_iterator begin() const noexcept { return start; }
        iterator end() noexcept { return finish; }
        const_iterator end() const noexcept { return finish; }
        reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
        const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
        reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
        const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
        const_iterator cbegin() const noexcept { return start; }
        const_iterator cend() const noexcept { return finish; }
        const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }
        const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }

        // Capacity
        size_type size() const noexcept { return end() - begin(); }
        // size_type max_size() const noexcept { return 0; }
        void resize(size_type new_size);
        void resize(size_type new_size, const value_type &val);
        bool empty() const noexcept { return start == finish; }
        void shrink_to_fit();

        // Element access
        reference
        operator[](difference_type n)
        {
            return *(start + n);
        }
        const_reference operator[](difference_type n) const { return *(start + n); }
        reference at(difference_type n)
        {
            if (n >= size())
                throw std::out_of_range("deque out of range");
            return *(start + n);
        }
        const_reference at(difference_type n) const
        {
            if (n >= size())
                throw std::out_of_range("deque out of range");
            return *(start + n);
        }
        reference front() noexcept { return *begin(); }
        const_reference front() const noexcept { return *begin(); }
        reference back() noexcept { return *(end() - 1); }
        const_reference back() const noexcept { return *(end() - 1); }

        // Modifiers
        void assign(size_type n, const value_type &val);
        template <typename _Input_iter>
        void assign(_Input_iter first, _Input_iter last);
        void assign(std::initializer_list<value_type> _l);
        void push_back(const value_type &val);
        void push_back(value_type &&val);
        void push_front(const value_type &val);
        void push_front(value_type &&val);
        void pop_back();
        void pop_front();
        iterator insert(const_iterator pos, const value_type &val);
        iterator insert(const_iterator pos, value_type &&val);
        iterator insert(const_iterator pos, size_type n, const value_type &val);
        template <typename _Input_iter>
        iterator insert(const_iterator pos, _Input_iter first, _Input_iter last);
        iterator insert(const_iterator pos, std::initializer_list<value_type> _l);
        iterator erase(const_iterator pos);
        iterator erase(const_iterator first, const_iterator last);
        void swap(deque &x) noexcept;
        void clear() noexcept;
        template <typename... _Args>
        iterator emplace(const_iterator pos, _Args &&...args);
        template <typename... _Args>
        void emplace_back(_Args &&...args);
        template <typename... _Args>
        void emplace_front(_Args &&...args);

    private:
        inline size_type per_buf_size() { return (sizeof(value_type) < deque_buf_size ? (deque_buf_size / sizeof(value_type)) : (1)); }
        void _M_initialize(size_type _n);
        void _M_reserve_map_back(size_type _map_size);
        void _M_reserve_map_front(size_type _map_size);

        using _elem_pointer = typename iterator::_elem_pointer;
        using _map_pointer = typename iterator::_map_pointer;
        using _Tp_alloc_type = typename __gnu_cxx::__alloc_traits<_Alloc>::template rebind<value_type>::other;
        using _Alloc_traits = __gnu_cxx::__alloc_traits<_Tp_alloc_type>;
        using _Map_alloc_type = typename _Alloc_traits::template rebind<_elem_pointer>::other;
        using _Map_alloc_traits = __gnu_cxx::__alloc_traits<_Map_alloc_type>;

        _map_pointer map;
        size_type map_size;
        iterator start;
        iterator finish;
        _Tp_alloc_type value_allocater;
        _Map_alloc_type map_allocater;
    };

    template <typename _Ty, typename _Alloc>
    deque<_Ty, _Alloc>::deque()
    {
        _M_initialize(0);
    }

    template <typename _Ty, typename _Alloc>
    deque<_Ty, _Alloc>::deque(size_type _n)
    {
        _M_initialize(_n);
        for (auto ptr = start._node; ptr < finish._node; ++ptr)
            std::__uninitialized_default_a(*ptr, *ptr + per_buf_size(), value_allocater);
        std::__uninitialized_default_a(finish._start, finish._cur, value_allocater);
    }

    template <typename _Ty, typename _Alloc>
    deque<_Ty, _Alloc>::deque(size_type _n, const value_type &_value)
    {
        _M_initialize(_n);
        for (auto ptr = start._node; ptr < finish._node; ++ptr)
            std::__uninitialized_fill_a(*ptr, *ptr + per_buf_size(), _value, value_allocater);
        std::__uninitialized_fill_a(finish._start, finish._cur, _value, value_allocater);
    }

    template <typename _Ty, typename _Alloc>
    template <typename _Input_iter, typename>
    deque<_Ty, _Alloc>::deque(_Input_iter first, _Input_iter last)
    {
        _M_initialize(last - first);
        std::__uninitialized_copy_a(first, last, start, value_allocater);
    }

    template <typename _Ty, typename _Alloc>
    deque<_Ty, _Alloc>::deque(std::initializer_list<value_type> _l)
    {
        _M_initialize(_l.size());
        std::__uninitialized_copy_a(_l.begin(), _l.end(), start, value_allocater);
    }

    template <typename _Ty, typename _Alloc>
    deque<_Ty, _Alloc>::deque(const deque &_x)
    {
        _M_initialize(_x.size());
        std::__uninitialized_copy_a(_x.begin(), _x.end(), begin(), value_allocater);
    }

    template <typename _Ty, typename _Alloc>
    deque<_Ty, _Alloc>::deque(deque &&_x)
    {
        map_size = _x.map_size;
        map = _x.map;
        start = _x.start;
        finish = _x.finish;

        _x.map_size = 0;
        _x.map = nullptr;
        _x.start = iterator();
        _x.finish = iterator();
    }

    template <typename _Ty, typename _Alloc>
    deque<_Ty, _Alloc>::~deque()
    {
        if (map == nullptr)
            return;

        for (auto it = start; it < finish;)
        {
            value_allocater.destroy((it++)._cur);
        }
        for (auto i = 0; i < map_size; i++)
            value_allocater.deallocate(*(map + i), per_buf_size());
        map_allocater.deallocate(map, map_size);
    }

    template <typename _Ty, typename _Alloc>
    deque<_Ty, _Alloc> &deque<_Ty, _Alloc>::operator=(const deque &_x)
    {
        if (size() >= _x.size())
        {
            auto pos = std::copy(_x.begin(), _x.end(), start);
            for (auto it = pos; it < finish; it++)
                value_allocater.destroy(it._cur);
            finish = pos;
        }
        else
        {
            auto left = per_buf_size() * (map + map_size - finish._node - 1) + (finish._end - finish._cur);
            std::copy(_x.begin(), _x.begin() + size(), start);
            if (_x.size() - size() <= left)
                finish = std::__uninitialized_copy_a(_x.begin() + size(), _x.end(), end(), value_allocater);
            else
            {
                _M_reserve_map_back(_x.map_size);
                finish = std::__uninitialized_copy_a(_x.begin() + size(), _x.end(), end(), value_allocater);
            }
        }

        return *this;
    }

    template <typename _Ty, typename _Alloc>
    deque<_Ty, _Alloc> &deque<_Ty, _Alloc>::operator=(deque &&_x)
    {
        this->~deque();

        map_size = _x.map_size;
        map = _x.map;
        start = _x.start;
        finish = _x.finish;

        _x.map_size = 0;
        _x.map = nullptr;
        _x.start = iterator();
        _x.finish = iterator();

        return *this;
    }

    template <typename _Ty, typename _Alloc>
    deque<_Ty, _Alloc> &deque<_Ty, _Alloc>::operator=(std::initializer_list<value_type> _l)
    {
        if (size() >= _l.size())
        {
            auto pos = std::copy(_l.begin(), _l.end(), start);
            for (auto it = pos; it < finish;)
                value_allocater.destroy((it++)._cur);
            finish = pos;
        }
        else
        {
            auto left = per_buf_size() * (map + map_size - finish._node - 1) + (finish._end - finish._cur);
            std::copy(_l.begin(), _l.begin() + size(), start);
            if (_l.size() - size() <= left)
                finish = std::__uninitialized_copy_a(_l.begin() + size(), _l.end(), end(), value_allocater);
            else
            {
                _M_reserve_map_back((_l.size() / per_buf_size()) + 1);
                finish = std::__uninitialized_copy_a(_l.begin() + size(), _l.end(), end(), value_allocater);
            }
        }

        return *this;
    }

    template <typename _Ty, typename _Alloc>
    void deque<_Ty, _Alloc>::resize(size_type new_size)
    {
        if (size() < new_size)
        {
            iterator prefinish = finish - 1;
            auto end_left = (((map + map_size) - prefinish._node) - 1) * per_buf_size() + prefinish._end - prefinish._cur;
            if (new_size - size() > end_left)
                _M_reserve_map_back(map_size + (new_size - size() - end_left + per_buf_size() - 1) % per_buf_size());
            std::__uninitialized_default_a(finish, start + new_size, value_allocater);
            finish = start + new_size;
        }
        else
        {
            iterator newfinish = start + new_size;
            for (auto it = newfinish; it < finish; it++)
                value_allocater.destroy(it._cur);
            finish = newfinish;
        }
    }

    template <typename _Ty, typename _Alloc>
    void deque<_Ty, _Alloc>::resize(size_type new_size, const value_type &val)
    {
        if (size() < new_size)
        {
            iterator prefinish = finish - 1;
            auto end_left = (((map + map_size) - prefinish._node) - 1) * per_buf_size() + prefinish._end - prefinish._cur;
            if (new_size - size() > end_left)
                _M_reserve_map_back(map_size + (new_size - size() - end_left + per_buf_size() - 1) % per_buf_size());
            std::__uninitialized_fill_a(finish, start + new_size, val, value_allocater);
            finish = start + new_size;
        }
        else
        {
            iterator newfinish = start + new_size;
            for (auto it = newfinish; it < finish; it++)
                value_allocater.destroy(it._cur);
            finish = newfinish;
        }
    }

    template <typename _Ty, typename _Alloc>
    void deque<_Ty, _Alloc>::shrink_to_fit()
    {
        iterator perfinish = finish - 1;
        if (start._node == map && perfinish._node == (map + map_size - 1))
            return;

        auto _map_size = perfinish._node - start._node + 1;
        _map_pointer new_map = map_allocater.allocate(_map_size);
        for (auto i = 0; i < _map_size; i++)
            *(new_map + i) = *(start._node + i);

        auto diff = start._cur - start._start;
        auto node_diff = finish._node - start._node;
        start._node = new_map;
        start._start = *start._node;
        start._end = start._start + per_buf_size();
        start._cur = start._start + diff;

        diff = finish._cur - finish._start;
        finish._node = start._node + node_diff;
        finish._start = *finish._node;
        finish._end = finish._start + per_buf_size();
        finish._cur = finish._start + diff;

        map_allocater.deallocate(map, map_size);
        map = new_map;
        map_size = _map_size;
    }

    template <typename _Ty, typename _Alloc>
    void deque<_Ty, _Alloc>::assign(size_type n, const value_type &val)
    {
        if (map_size * per_buf_size() >= n)
        {
            if (size() >= n)
            {
                auto pos = std::fill_n(begin(), n, val);
                for (auto it = pos; it < finish;)
                    value_allocater.destroy((it++)._cur);
                finish = pos;
            }
            else
            {
                std::fill(begin(), end(), val);
                std::__uninitialized_fill_a(end(), begin() + n, val, value_allocater);
                finish = start + n;
            }
        }
        else
        {
            std::fill(begin(), end(), val);
            _M_reserve_map_back((n / per_buf_size()) + 1);
            std::__uninitialized_fill_a(end(), begin() + n, val, value_allocater);
            finish = start + n;
        }
    }

    template <typename _Ty, typename _Alloc>
    template <typename _Input_iter>
    void deque<_Ty, _Alloc>::assign(_Input_iter first, _Input_iter last)
    {
        auto n = last - first;
        if (map_size * per_buf_size() >= n)
        {
            if (size() >= n)
            {
                auto pos = std::copy(first, last, start);
                for (auto it = pos; it < finish;)
                    value_allocater.destroy((it++)._cur);
                finish = pos;
            }
            else
            {
                auto _n = size();
                std::copy(first, first + _n, start);
                finish = std::__uninitialized_copy_a(first + _n, last, end(), value_allocater);
            }
        }
        else
        {
            std::copy(first, first + size(), start);
            _M_reserve_map_back((n / per_buf_size()) + 1);
            finish = std::__uninitialized_copy_a(first + size(), last, end(), value_allocater);
        }
    }

    template <typename _Ty, typename _Alloc>
    void deque<_Ty, _Alloc>::assign(std::initializer_list<value_type> _l)
    {
        if (size() >= _l.size())
        {
            auto pos = std::copy(_l.begin(), _l.end(), begin());
            for (auto it = pos; it < finish;)
                value_allocater.destroy((it++)._cur);
            finish = pos;
        }
        else
        {
            auto left = per_buf_size() * (map + map_size - finish._node - 1) + (finish._end - finish._cur);
            std::copy(_l.begin(), _l.begin() + size(), begin());
            if (_l.size() - size() <= left)
                finish = std::__uninitialized_copy_a(_l.begin() + size(), _l.end(), end(), value_allocater);
            else
            {
                _M_reserve_map_back((_l.size() / per_buf_size()) + 1);
                finish = std::__uninitialized_copy_a(_l.begin() + size(), _l.end(), end(), value_allocater);
            }
        }
    }

    template <typename _Ty, typename _Alloc>
    void deque<_Ty, _Alloc>::push_back(const value_type &val)
    {
        if (finish._node == map + map_size)
        {
            _M_reserve_map_back(map_size + 1);
        }

        std::__uninitialized_fill_a(end(), end() + 1, val, value_allocater);
        finish += 1;
    }

    template <typename _Ty, typename _Alloc>
    void deque<_Ty, _Alloc>::push_back(value_type &&val)
    {
        if (finish._node == map + map_size)
        {
            _M_reserve_map_back(map_size + 1);
        }

        value_allocater.construct(end()._cur, std::move(val));
        finish += 1;
    }

    template <typename _Ty, typename _Alloc>
    void deque<_Ty, _Alloc>::push_front(const value_type &val)
    {
        if (start._cur == start._start)
        {
            _M_reserve_map_front(map_size + 1);
        }

        start--;
        value_allocater.construct(begin()._cur, val);
    }

    template <typename _Ty, typename _Alloc>
    void deque<_Ty, _Alloc>::push_front(value_type &&val)
    {
        if (start._cur == start._start)
        {
            _M_reserve_map_front(map_size + 1);
        }

        start--;
        value_allocater.construct(begin()._cur, std::move(val));
    }

    template <typename _Ty, typename _Alloc>
    void deque<_Ty, _Alloc>::pop_back()
    {
        --finish;
        value_allocater.destroy(finish._cur);
    }

    template <typename _Ty, typename _Alloc>
    void deque<_Ty, _Alloc>::pop_front()
    {
        auto it = start++;
        value_allocater.destroy(it._cur);
    }

    template <typename _Ty, typename _Alloc>
    typename deque<_Ty, _Alloc>::iterator deque<_Ty, _Alloc>::insert(const_iterator pos, const value_type &val)
    {
        if (pos._cur == finish._cur)
        {
            push_back(val);
            auto tmp = finish;
            return --tmp;
        }
        else if (pos._cur == start._cur)
        {
            push_front(val);
            return start;
        }
        else
        {
            auto diff = pos - start;
            if (diff <= size() / 2)
            {
                iterator _pos = pos - 1;
                push_front(front());
                std::move(begin() + 2, _pos + 1, begin() + 1);
                *_pos = val;
                return _pos;
            }
            else
            {
                iterator _pos = pos;
                push_back(back());
                std::move_backward(_pos, end() - 2, _pos + 1);
                *_pos = val;
                return _pos;
            }
        }
    }

    template <typename _Ty, typename _Alloc>
    typename deque<_Ty, _Alloc>::iterator deque<_Ty, _Alloc>::insert(const_iterator pos, value_type &&val)
    {
        return emplace(pos, std::forward<value_type>(val));
    }

    template <typename _Ty, typename _Alloc>
    typename deque<_Ty, _Alloc>::iterator deque<_Ty, _Alloc>::insert(const_iterator pos, size_type n, const value_type &val)
    {
        auto diff = pos - start;

        if (diff <= size() / 2)
        {
            if ((start._cur - start._start) < n)
            {
                auto map_size_reverse = (n - (start._cur - start._start)) / per_buf_size();
                map_size_reverse = (n - (start._cur - start._start)) % per_buf_size() == 0 ? map_size_reverse : map_size_reverse + 1;
                _M_reserve_map_front(map_size_reverse + map_size);
            }
            if (diff >= n)
            {
                std::__uninitialized_move_a(start, start + n, start - n, value_allocater);
                std::move(start + n, start + diff, start);
                std::fill(start + diff - n, start + diff, val);
            }
            else
            {
                std::__uninitialized_move_a(start, start + diff, start - n, value_allocater);
                std::__uninitialized_fill_a(start - n + diff, start, val, value_allocater);
                std::fill(start, start + diff, val);
            }
            start = start - n;
            return start + diff;
        }
        else
        {
            auto reverse_num = (finish._node - map == map_size) ? 0 : finish._end - finish._cur;
            auto diff_back = size() - diff;
            if (reverse_num < n)
            {
                auto map_size_reverse = (n - reverse_num) / per_buf_size();
                map_size_reverse = (n - reverse_num) % per_buf_size() == 0 ? map_size_reverse : map_size_reverse + 1;
                _M_reserve_map_back(map_size_reverse + map_size);
            }
            if (diff_back >= n)
            {
                std::__uninitialized_move_a(finish - n, finish, finish, value_allocater);
                std::move_backward(finish - diff_back, finish - n, finish);
                std::fill(finish - diff_back, finish - (diff_back - n), val);
            }
            else
            {
                std::__uninitialized_move_a(finish - diff_back, finish, finish + n - diff_back, value_allocater);
                std::__uninitialized_fill_a(finish, finish + n - diff_back, val, value_allocater);
                std::fill(finish - diff_back, finish, val);
            }
            finish = finish + n;
            return finish - diff_back - n;
        }
    }

    template <typename _Ty, typename _Alloc>
    template <typename _Input_iter>
    typename deque<_Ty, _Alloc>::iterator deque<_Ty, _Alloc>::insert(const_iterator pos, _Input_iter first, _Input_iter last)
    {
        auto diff = pos - start;
        auto n = last - first;

        if (diff <= size() / 2)
        {
            if ((start._cur - start._start) < n)
            {
                auto map_size_reverse = (n - (start._cur - start._start) + per_buf_size() - 1) / per_buf_size();
                _M_reserve_map_front(map_size_reverse + map_size);
            }
            if (diff >= n)
            {
                std::__uninitialized_move_a(start, start + n, start - n, value_allocater);
                std::move(start + n, start + diff, start);
                std::copy(first, last, start + diff - n);
            }
            else
            {
                std::__uninitialized_move_a(start, start + diff, start - n, value_allocater);
                std::__uninitialized_copy_a(first, first + n - diff, start - n + diff, value_allocater);
                std::copy(first + n - diff, last, start);
            }
            start = start - n;
            return start + diff;
        }
        else
        {
            auto reverse_num = (finish._node - map == map_size) ? 0 : finish._end - finish._cur;
            auto diff_back = size() - diff;
            if (reverse_num < n)
            {
                auto map_size_reverse = (n - reverse_num + per_buf_size() - 1) / per_buf_size();
                _M_reserve_map_back(map_size_reverse + map_size);
            }
            if (diff_back >= n)
            {
                std::__uninitialized_move_a(finish - n, finish, finish, value_allocater);
                std::move_backward(finish - diff_back, finish - n, finish);
                std::copy(first, last, finish - diff_back);
            }
            else
            {
                std::__uninitialized_move_a(finish - diff_back, finish, finish + n - diff_back, value_allocater);
                std::__uninitialized_copy_a(first + diff_back, last, finish, value_allocater);
                std::copy(first, first + diff_back, finish - diff_back);
            }
            finish = finish + n;
            return finish - diff_back - n;
        }
    }

    template <typename _Ty, typename _Alloc>
    typename deque<_Ty, _Alloc>::iterator deque<_Ty, _Alloc>::insert(const_iterator pos, std::initializer_list<value_type> _l)
    {
        auto diff = pos - start;
        auto n = _l.size();

        if (diff <= size() / 2)
        {
            if ((start._cur - start._start) < n)
            {
                auto map_size_reverse = (n - (start._cur - start._start) + per_buf_size() - 1) / per_buf_size();
                _M_reserve_map_front(map_size_reverse + map_size);
            }
            if (diff >= n)
            {
                std::__uninitialized_move_a(start, start + n, start - n, value_allocater);
                std::move(start + n, start + diff, start);
                std::copy(_l.begin(), _l.end(), start + diff - n);
            }
            else
            {
                std::__uninitialized_move_a(start, start + diff, start - n, value_allocater);
                std::__uninitialized_copy_a(_l.begin(), _l.begin() + n - diff, start - n + diff, value_allocater);
                std::copy(_l.begin() + n - diff, _l.end(), start);
            }
            start = start - n;
            return start + diff;
        }
        else
        {
            auto reverse_num = (finish._node - map == map_size) ? 0 : finish._end - finish._cur;
            auto diff_back = size() - diff;
            if (reverse_num < n)
            {
                auto map_size_reverse = (n - reverse_num + per_buf_size() - 1) / per_buf_size();
                _M_reserve_map_back(map_size_reverse + map_size);
            }
            if (diff_back >= n)
            {
                std::__uninitialized_move_a(finish - n, finish, finish, value_allocater);
                std::move_backward(finish - diff_back, finish - n, finish);
                std::copy(_l.begin(), _l.end(), finish - diff_back);
            }
            else
            {
                std::__uninitialized_move_a(finish - diff_back, finish, finish + n - diff_back, value_allocater);
                std::__uninitialized_copy_a(_l.begin() + diff_back, _l.end(), finish, value_allocater);
                std::copy(_l.begin(), _l.begin() + diff_back, finish - diff_back);
            }
            finish = finish + n;
            return finish - diff_back - n;
        }
    }

    template <typename _Ty, typename _Alloc>
    typename deque<_Ty, _Alloc>::iterator deque<_Ty, _Alloc>::erase(const_iterator pos)
    {
        iterator _pos = pos;
        auto diff = _pos - start;

        if (diff <= (size() >> 1))
        {
            if (_pos != start)
                std::move_backward(start, _pos, _pos + 1);
            pop_front();
        }
        else
        {
            if (_pos != finish - 1)
                std::move(_pos + 1, finish, _pos);
            pop_back();
        }
        return start + diff;
    }

    template <typename _Ty, typename _Alloc>
    typename deque<_Ty, _Alloc>::iterator deque<_Ty, _Alloc>::erase(const_iterator first, const_iterator last)
    {
        iterator _first = first;
        iterator _last = last;
        auto n = _last - _first;
        auto diff = _first - start;
        if (n <= 0 || diff < 0)
            return _first;

        if (_first - start <= finish - _last)
        {
            std::move_backward(start, _first, _last);
            auto node = _last - diff;
            for (auto it = start; it != node;)
                value_allocater.destroy((it++)._cur);
            start = node;
            return start + diff;
        }
        else
        {
            std::move(_last, finish, _first);
            auto node = finish - n;
            for (auto it = node; it != finish;)
                value_allocater.destroy((it++)._cur);
            finish = node;
            return _first;
        }
    }

    template <typename _Ty, typename _Alloc>
    void deque<_Ty, _Alloc>::swap(deque &x) noexcept
    {
        std::swap(map_size, x.map_size);
        std::swap(map, x.map);
        std::swap(start, x.start);
        std::swap(finish, x.finish);
    }

    template <typename _Ty, typename _Alloc>
    void deque<_Ty, _Alloc>::clear() noexcept
    {
        for (auto it = start; it != finish;)
            value_allocater.destroy((it++)._cur);
        start._node = map;
        start._start = *start._node;
        start._end = start._start + per_buf_size();
        start._cur = start._start;

        finish = start;
    }

    template <typename _Ty, typename _Alloc>
    template <typename... _Args>
    typename deque<_Ty, _Alloc>::iterator deque<_Ty, _Alloc>::emplace(const_iterator pos, _Args &&...args)
    {
        auto diff = pos - start;

        if (pos._cur == start._cur)
        {
            emplace_front(std::forward<_Args>(args)...);
            return start;
        }
        else if (pos._cur == finish._cur)
        {
            emplace_back(std::forward<_Args>(args)...);
            return start + diff;
        }
        else
        {
            value_type val(std::forward<_Args>(args)...);

            if (diff <= size() / 2)
            {
                push_front(std::move(front()));
                std::move(start + 2, start + diff + 1, start + 1);
                *(start + diff) = std::move(val);
                return start + diff;
            }
            else
            {
                push_back(std::move(back()));
                std::move_backward(start + diff, finish - 2, finish - 1);
                *(start + diff) = std::move(val);
                return (start + diff);
            }
        }
    }

    template <typename _Ty, typename _Alloc>
    template <typename... _Args>
    void deque<_Ty, _Alloc>::emplace_back(_Args &&...args)
    {
        if (finish._node == map + map_size)
        {
            _M_reserve_map_back(map_size + 1);
        }

        value_allocater.construct(finish._cur, std::forward<_Args>(args)...);
        finish += 1;
    }

    template <typename _Ty, typename _Alloc>
    template <typename... _Args>
    void deque<_Ty, _Alloc>::emplace_front(_Args &&...args)
    {
        if (start._cur == start._start && start._node == map)
        {
            _M_reserve_map_front(map_size + 1);
        }

        start--;
        value_allocater.construct(start._cur, std::forward<_Args>(args)...);
    }

    template <typename _Ty, typename _Alloc>
    void deque<_Ty, _Alloc>::_M_initialize(size_type _n)
    {
        map_size = _n / per_buf_size();
        map_size = _n % per_buf_size() == 0 ? map_size : map_size + 1;
        map = map_allocater.allocate(map_size);

        for (auto i = 0; i < map_size; i++)
            *(map + i) = value_allocater.allocate(per_buf_size());

        start._node = map;
        start._start = *start._node;
        start._end = start._start + per_buf_size();
        start._cur = start._start;

        finish._node = start._node + _n / per_buf_size();
        finish._start = *finish._node;
        finish._end = finish._start + per_buf_size();
        finish._cur = finish._start + (_n % per_buf_size());
    }

    template <typename _Ty, typename _Alloc>
    void deque<_Ty, _Alloc>::_M_reserve_map_back(size_type _map_size)
    {
        if (_map_size <= map_size)
            return;

        auto _n = size();
        _map_pointer new_map = map_allocater.allocate(_map_size);
        for (auto i = 0; i < map_size; i++)
            *(new_map + i) = *(map + i);
        for (auto i = map_size; i < _map_size; i++)
            *(new_map + i) = value_allocater.allocate(per_buf_size());

        auto diff = start._cur - start._start;
        auto node_diff = finish._node - start._node;
        start._node = new_map + (start._node - map);
        start._start = *start._node;
        start._end = start._start + per_buf_size();
        start._cur = start._start + diff;

        diff = finish._cur - finish._start;
        finish._node = start._node + node_diff;
        finish._start = *finish._node;
        finish._end = finish._start + per_buf_size();
        finish._cur = finish._start + diff;

        map_allocater.deallocate(map, map_size);
        map = new_map;
        map_size = _map_size;
    }

    template <typename _Ty, typename _Alloc>
    void deque<_Ty, _Alloc>::_M_reserve_map_front(size_type _map_size)
    {
        if (_map_size <= map_size)
            return;

        auto _n = size();
        _map_pointer new_map = map_allocater.allocate(_map_size);
        for (auto i = 0; i < map_size; i++)
            *(new_map + i + _map_size - map_size) = *(map + i);
        for (auto i = 0; i < _map_size - map_size; i++)
            *(new_map + i) = value_allocater.allocate(per_buf_size());

        auto diff = start._cur - start._start;
        auto node_diff = finish._node - start._node;
        start._node = new_map + _map_size - map_size + (start._node - map);
        start._start = *start._node;
        start._end = start._start + per_buf_size();
        start._cur = start._start + diff;

        diff = finish._cur - finish._start;
        finish._node = start._node + node_diff;
        finish._start = *finish._node;
        finish._end = finish._start + per_buf_size();
        finish._cur = finish._start + diff;

        map_allocater.deallocate(map, map_size);
        map = new_map;
        map_size = _map_size;
    }
} // namespace htd