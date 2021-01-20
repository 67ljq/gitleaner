#pragma once

#include <memory>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <algorithm>

namespace ljq
{

    template <typename _Ty, typename _Alloc = std::allocator<_Ty>>
    class vector
    {
    public:
        using value_type = _Ty;
        using allocator_type = _Alloc;
        using pointer = typename std::allocator_traits<_Alloc>::pointer;
        using const_pointer = typename std::allocator_traits<_Alloc>::const_pointer;
        using iterator = __gnu_cxx::__normal_iterator<pointer, vector>;
        using const_iterator = __gnu_cxx::__normal_iterator<const_pointer, vector>;
        using reserse_iterator = std::reverse_iterator<iterator>;
        using const_reserse_iterator = std::reverse_iterator<const_iterator>;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using reference = _Ty &;
        using const_reference = const _Ty &;

    public:
        vector() noexcept;
        explicit vector(size_type _count);
        vector(size_type _count, const _Ty &val);
        template <typename _Input_iter>
        vector(_Input_iter _first, _Input_iter _last);
        vector(const vector &_x);
        vector(std::initializer_list<value_type> _list);
        ~vector();

        vector &operator=(const vector &_x);
        vector &operator=(vector &&_x);
        vector &operator=(std::initializer_list<value_type> _list);

        // Iterators
        iterator begin() noexcept { return iterator(start); }
        iterator end() noexcept { return iterator(finish); }
        const_iterator begin() const noexcept { return const_iterator(start); }
        const_iterator end() const noexcept { return const_iterator(finish); }
        reserse_iterator rbegin() noexcept { return reserse_iterator(end()); }
        reserse_iterator rend() noexcept { return reserse_iterator(begin()); }
        const_reserse_iterator rbegin() const noexcept { return const_reserse_iterator(end()); }
        const_reserse_iterator rend() const noexcept { return const_reserse_iterator(begin()); }
        const_iterator cbegin() const noexcept { return const_iterator(start); }
        const_iterator cend() const noexcept { return const_iterator(finish); }
        const_reserse_iterator crbegin() const noexcept { return const_reserse_iterator(end()); }
        const_reserse_iterator crend() const noexcept { return const_reserse_iterator(begin()); }

        // Capacity
        size_type size() const { return static_cast<size_type>(finish - start); }
        // size_type max_size() const { return 0; } //TODO
        void resize(size_type _new_size);
        void resize(size_type _new_size, const value_type &_x);
        size_type capacity() const { return static_cast<size_type>(end_of_storage - start); }
        void reserve(size_type _new_capacity);
        bool empty() { return begin() == end(); }
        void shrink_to_fit();

        // Element access:
        reference operator[](size_type _n) { return *(start + _n); }
        reference at(size_type _n)
        {
            if (_n >= size())
                throw std::out_of_range("vector.at out of range _n > size");
            return *(start + _n);
        }
        const_reference at(size_type _n) const
        {
            if (_n >= size())
                throw std::out_of_range("vector.at out of range _n > size");
            return *(start + _n);
        }
        reference front() { return *begin(); }
        const_reference front() const { return *begin(); }
        reference back() { return *(end() - 1); }
        const_reference back() const { return *(end() - 1); }
        pointer data() { return start; }
        const_pointer data() const { return start; }

        // Modifiers
        void assign(size_type _n, const value_type &_x);
        void assign(std::initializer_list<value_type> _list);
        template <typename _Input_iter>
        void assign(_Input_iter _first, _Input_iter _last);
        void push_back(const value_type &val);
        void pop_back();
        iterator insert(const_iterator _position, const value_type &_x);
        iterator insert(const_iterator _position, size_type _n, const value_type &_x);
        template <typename _Input_iter>
        iterator insert(const_iterator _position, _Input_iter _first, _Input_iter _last);
        iterator insert(const_iterator _position, std::initializer_list<value_type> _list);
        iterator erase(const_iterator _position);
        iterator erase(const_iterator _first, const_iterator _last);
        void swap(vector &_x)
        {
            std::swap(start, _x.start);
            std::swap(finish, _x.finish);
            std::swap(end_of_storage, _x.end_of_storage);
        }
        void clear()
        {
            allocator_type alloc;
            for (pointer i = start; i < finish; i++)
            {
                alloc.destroy(i);
            }
            finish = start;
        }
        template <typename... _ARGS>
        iterator emplace(const_iterator _position, _ARGS &&... _args);
        template <typename... _ARGS>
        void emplace_back(_ARGS &&... _args);

    protected:
        pointer start;
        pointer finish;
        pointer end_of_storage;
    };

    template <class _Ty, typename _Alloc>
    vector<_Ty, _Alloc>::vector() noexcept
    {
        start = nullptr;
        finish = nullptr;
        end_of_storage = nullptr;
    }

    template <class _Ty, typename _Alloc>
    vector<_Ty, _Alloc>::vector(size_type _count)
    {
        allocator_type alloc;
        size_type cnt = _count ? _count : 1;
        start = alloc.allocate(cnt);
        finish = std::__uninitialized_default_n_a(start, cnt, alloc);
        end_of_storage = finish;
    }

    template <class _Ty, typename _Alloc>
    vector<_Ty, _Alloc>::vector(size_type _count, const _Ty &val)
    {
        allocator_type alloc;
        size_type cnt = _count ? _count : 1;
        start = alloc.allocate(cnt);
        finish = std::__uninitialized_fill_n_a(start, cnt, val, alloc);
        end_of_storage = finish;
    }

    template <class _Ty, typename _Alloc>
    template <typename _Input_iter>
    vector<_Ty, _Alloc>::vector(_Input_iter _first, _Input_iter _last)
    {
        size_type cnt = _last > _first ? (_last - _first) : 0;
        if (cnt == 0)
        {
            start = nullptr;
            finish = nullptr;
            end_of_storage = nullptr;
        }
        else
        {
            allocator_type alloc;
            start = alloc.allocate(cnt);
            finish = std::__uninitialized_copy_a(_first, _last, start, alloc);
            end_of_storage = finish;
        }
    }

    template <class _Ty, typename _Alloc>
    vector<_Ty, _Alloc>::vector(const vector &_x)
    {
        allocator_type alloc;
        start = alloc.allocate(_x.capacity());
        finish = std::__uninitialized_copy_a(_x.begin(), _x.end(), start, alloc);
        end_of_storage = finish;
    }

    template <class _Ty, typename _Alloc>
    vector<_Ty, _Alloc>::vector(std::initializer_list<value_type> _list)
    {
        allocator_type alloc;
        start = alloc.allocate(_list.size());
        finish = std::__uninitialized_copy_a(_list.begin(), _list.end(), start, alloc);
        end_of_storage = finish;
    }

    template <class _Ty, typename _Alloc>
    vector<_Ty, _Alloc>::~vector()
    {
        allocator_type alloc;
        for (pointer i = start; i < finish; i++)
        {
            alloc.destroy(i);
        }
        alloc.deallocate(start, end_of_storage - start);
    }

    template <class _Ty, typename _Alloc>
    vector<_Ty, _Alloc> &vector<_Ty, _Alloc>::operator=(const vector &_x)
    {
        allocator_type alloc;

        for (pointer i = start; i < finish; i++)
        {
            alloc.destroy(i);
        }

        if (_x.size() > capacity() || size() == 0)
        {
            alloc.deallocate(start, end_of_storage - start);
            start = alloc.allocate(_x.size());
            finish = std::__uninitialized_copy_a(_x.begin(), _x.end(), start, alloc);
            end_of_storage = finish;
        }
        else
        {
            finish = std::__uninitialized_copy_a(_x.begin(), _x.end(), start, alloc);
        }
        return *this;
    }

    template <class _Ty, typename _Alloc>
    vector<_Ty, _Alloc> &vector<_Ty, _Alloc>::operator=(vector &&_x)
    {
        allocator_type alloc;

        for (pointer i = start; i < finish; i++)
        {
            alloc.destroy(i);
        }
        alloc.deallocate(start, end_of_storage - start);

        start = _x.start;
        finish = _x.finish;
        end_of_storage = _x.end_of_storage;

        _x.start = nullptr;
        _x.finish = nullptr;
        _x.end_of_storage = nullptr;

        return *this;
    }

    template <class _Ty, typename _Alloc>
    vector<_Ty, _Alloc> &vector<_Ty, _Alloc>::operator=(std::initializer_list<value_type> _list)
    {
        allocator_type alloc;

        for (pointer i = start; i < finish; i++)
        {
            alloc.destroy(i);
        }

        if (_list.size() > capacity())
        {
            alloc.deallocate(start, end_of_storage - start);

            start = alloc.allocate(_list.size());
            finish = std::__uninitialized_copy_a(_list.begin(), _list.end(), start, alloc);
            end_of_storage = finish;
        }
        else
        {
            finish = std::__uninitialized_copy_a(_list.begin(), _list.end(), start, alloc);
        }
    }

    template <class _Ty, typename _Alloc>
    void vector<_Ty, _Alloc>::resize(size_type _new_size)
    {
        allocator_type alloc;
        if (_new_size < size())
        {
            for (pointer i = start + _new_size; i < finish; i++)
            {
                alloc.destroy(i);
            }
            finish = start + _new_size;
        }
        else if (_new_size > size())
        {
            if (_new_size <= capacity())
            {
                finish = std::__uninitialized_default_n_a(finish, _new_size - size(), alloc);
            }
            else
            {
                pointer _start = alloc.allocate(_new_size);
                pointer _finish = std::__uninitialized_move_if_noexcept_a(start, finish, _start, alloc);
                for (pointer i = start; i < finish; i++)
                {
                    alloc.destroy(i);
                }
                alloc.deallocate(start, end_of_storage - start);
                start = _start;
                finish = std::__uninitialized_default_n_a(_finish, _new_size - static_cast<size_type>(_finish - _start), alloc);
                end_of_storage = finish;
            }
        }
    }

    template <class _Ty, typename _Alloc>
    void vector<_Ty, _Alloc>::resize(size_type _new_size, const value_type &_x)
    {
        allocator_type alloc;
        if (_new_size < size())
        {
            for (pointer i = start + _new_size; i < finish; i++)
            {
                alloc.destroy(i);
            }
            finish = start + _new_size;
        }
        else if (_new_size > size())
        {
            if (_new_size <= capacity())
            {
                finish = std::__uninitialized_fill_n_a(finish, _new_size - size(), _x, alloc);
            }
            else
            {
                pointer _start = alloc.allocate(_new_size);
                pointer _finish = std::__uninitialized_move_if_noexcept_a(start, finish, _start, alloc);
                for (pointer i = start; i < finish; i++)
                {
                    alloc.destroy(i);
                }
                alloc.deallocate(start, end_of_storage - start);
                start = _start;
                finish = std::__uninitialized_fill_n_a(_finish, _new_size - static_cast<size_type>(_finish - _start), _x, alloc);
                end_of_storage = finish;
            }
        }
    }

    template <class _Ty, typename _Alloc>
    void vector<_Ty, _Alloc>::reserve(size_type _new_capacity)
    {
        if (_new_capacity > capacity())
        {
            allocator_type alloc;
            pointer _start = alloc.allocate(_new_capacity);
            pointer _finish = std::__uninitialized_move_if_noexcept_a(start, finish, _start, alloc);
            for (pointer i = start; i < finish; i++)
            {
                alloc.destroy(i);
            }
            alloc.deallocate(start, end_of_storage - start);
            start = _start;
            finish = _finish;
            end_of_storage = _start + _new_capacity;
        }
    }

    template <class _Ty, typename _Alloc>
    void vector<_Ty, _Alloc>::shrink_to_fit()
    {
        if (size() != capacity())
        {
            allocator_type alloc;
            pointer _start = alloc.allocate(size());
            pointer _finish = std::__uninitialized_move_if_noexcept_a(start, finish, _start, alloc);
            for (pointer i = start; i < finish; i++)
            {
                alloc.destroy(i);
            }
            alloc.deallocate(start, end_of_storage - start);
            start = _start;
            finish = _finish;
            end_of_storage = finish;
        }
    }

    template <class _Ty, typename _Alloc>
    void vector<_Ty, _Alloc>::assign(size_type _n, const value_type &_x)
    {
        allocator_type alloc;
        if (_n > capacity())
        {
            for (pointer i = start; i < finish; i++)
            {
                alloc.destroy(i);
            }
            alloc.deallocate(start, end_of_storage - start);

            start = alloc.allocate(_n);
            finish = std::__uninitialized_fill_n_a(start, _n, _x, alloc);
            end_of_storage = finish;
        }
        else
        {
            for (pointer i = start; i < finish; i++)
            {
                alloc.destroy(i);
            }
            finish = std::__uninitialized_fill_n_a(start, _n, _x, alloc);
        }
    }

    template <class _Ty, typename _Alloc>
    void vector<_Ty, _Alloc>::assign(std::initializer_list<value_type> _list)
    {
        allocator_type alloc;
        if (_list.size() > capacity())
        {
            for (pointer i = start; i < finish; i++)
            {
                alloc.destroy(i);
            }
            alloc.deallocate(start, end_of_storage - start);

            start = alloc.allocate(_list.size());
            finish = std::__uninitialized_copy_a(_list.begin(), _list.end(), start, alloc);
            end_of_storage = finish;
        }
        else
        {
            for (pointer i = start; i < finish; i++)
            {
                alloc.destroy(i);
            }
            finish = std::__uninitialized_copy_a(_list.begin(), _list.end(), start, alloc);
        }
    }

    template <class _Ty, typename _Alloc>
    template <typename _Input_iter>
    void vector<_Ty, _Alloc>::assign(_Input_iter _first, _Input_iter _last)
    {
        allocator_type alloc;

        size_type cnt = _last > _first ? (_last - _first) : 0;
        if (cnt > capacity())
        {
            for (pointer i = start; i < finish; i++)
            {
                alloc.destroy(i);
            }
            alloc.deallocate(start, end_of_storage - start);

            start = alloc.allocate(cnt);
            finish = std::__uninitialized_copy_a(_first, _last, start, alloc);
            end_of_storage = finish;
        }
        else
        {
            for (pointer i = start; i < finish; i++)
            {
                alloc.destroy(i);
            }
            finish = std::__uninitialized_copy_a(_first, _last, start, alloc);
        }
    }

    template <class _Ty, typename _Alloc>
    void vector<_Ty, _Alloc>::push_back(const value_type &val)
    {
        allocator_type alloc;
        if (end_of_storage == finish)
        {
            size_type cnt = capacity() ? 2 * capacity() : 1;
            pointer _start = alloc.allocate(cnt);
            pointer _finish = std::__uninitialized_move_if_noexcept_a(start, finish, _start, alloc);
            for (pointer i = start; i < finish; i++)
            {
                alloc.destroy(i);
            }
            alloc.deallocate(start, end_of_storage - start);
            start = _start;
            finish = _finish;
            end_of_storage = _start + cnt;
        }

        alloc.construct(finish, std::move(val));
        ++finish;
    }

    template <class _Ty, typename _Alloc>
    void vector<_Ty, _Alloc>::pop_back()
    {
        if (start == finish)
            return;
        else
        {
            allocator_type alloc;
            alloc.destroy(finish - 1);
            --finish;
        }
    }

    template <class _Ty, typename _Alloc>
    typename vector<_Ty, _Alloc>::iterator vector<_Ty, _Alloc>::insert(const_iterator _position, const value_type &_x)
    {
        auto _n = _position - cbegin();
        allocator_type alloc;

        if (end_of_storage == finish)
        {
            size_type cnt = capacity() ? 2 * capacity() : 1;
            pointer _start = alloc.allocate(cnt);
            pointer _finish = std::__uninitialized_move_if_noexcept_a(start, start + _n, _start, alloc);
            alloc.construct(_finish, _x);
            ++_finish;
            _finish = std::__uninitialized_move_if_noexcept_a(start + _n, finish, _finish, alloc);
            for (pointer i = start; i < finish; i++)
            {
                alloc.destroy(i);
            }
            alloc.deallocate(start, end_of_storage - start);
            start = _start;
            finish = _finish;
            end_of_storage = _start + cnt;
        }
        else
        {
            std::__uninitialized_move_if_noexcept_a(start + _n, finish, start + _n + 1, alloc);
            alloc.construct(start + _n, std::move(_x));
            ++finish;
        }

        return iterator(start + _n);
    }

    template <class _Ty, typename _Alloc>
    typename vector<_Ty, _Alloc>::iterator vector<_Ty, _Alloc>::insert(const_iterator _position, size_type _n, const value_type &_x)
    {
        auto _diff = _position - cbegin();
        allocator_type alloc;

        if (end_of_storage < finish + _n)
        {
            size_type cnt = size() + _n;
            pointer _start = alloc.allocate(cnt);
            pointer _finish = std::__uninitialized_move_if_noexcept_a(start, start + _diff, _start, alloc);
            _finish = std::__uninitialized_fill_n_a(_start + _diff, _n, _x, alloc);
            _finish = std::__uninitialized_move_if_noexcept_a(start + _diff, finish, _finish, alloc);
            for (pointer i = start; i < finish; i++)
            {
                alloc.destroy(i);
            }
            alloc.deallocate(start, end_of_storage - start);
            start = _start;
            finish = _finish;
            end_of_storage = _start + cnt;
        }
        else
        {
            std::__uninitialized_move_if_noexcept_a(start + _diff, finish, start + _diff + _n, alloc);
            std::__uninitialized_fill_n_a(start + _diff, _n, std::move(_x), alloc);
            finish += _n;
        }

        return iterator(start + _diff);
    }

    template <class _Ty, typename _Alloc>
    template <typename _Input_iter>
    typename vector<_Ty, _Alloc>::iterator vector<_Ty, _Alloc>::insert(const_iterator _position, _Input_iter _first, _Input_iter _last)
    {
        auto _diff = _position - cbegin();
        size_type _n = _last - _first;
        allocator_type alloc;

        if (end_of_storage < finish + _n)
        {
            size_type cnt = size() + _n;
            pointer _start = alloc.allocate(cnt);
            pointer _finish = std::__uninitialized_move_if_noexcept_a(start, start + _diff, _start, alloc);
            _finish = std::__uninitialized_copy_a(_first, _last, _start + _diff, alloc);
            _finish = std::__uninitialized_move_if_noexcept_a(start + _diff, finish, _finish, alloc);
            for (pointer i = start; i < finish; i++)
            {
                alloc.destroy(i);
            }
            alloc.deallocate(start, end_of_storage - start);
            start = _start;
            finish = _finish;
            end_of_storage = _start + cnt;
        }
        else
        {
            std::__uninitialized_move_if_noexcept_a(start + _diff, finish, start + _diff + _n, alloc);
            finish = std::__uninitialized_copy_a(_first, _last, start + _diff, alloc);
            finish += _n;
        }

        return iterator(start + _diff);
    }

    template <class _Ty, typename _Alloc>
    typename vector<_Ty, _Alloc>::iterator vector<_Ty, _Alloc>::insert(const_iterator _position, std::initializer_list<value_type> _list)
    {
        auto _diff = _position - cbegin();
        size_type _n = _list.size();
        allocator_type alloc;

        if (end_of_storage < finish + _n)
        {
            size_type cnt = size() + _n;
            pointer _start = alloc.allocate(cnt);
            pointer _finish = std::__uninitialized_move_if_noexcept_a(start, start + _diff, _start, alloc);
            _finish = std::__uninitialized_copy_a(_list.begin(), _list.end(), _start + _diff, alloc);
            _finish = std::__uninitialized_move_if_noexcept_a(start + _diff, finish, _finish, alloc);
            for (pointer i = start; i < finish; i++)
            {
                alloc.destroy(i);
            }
            alloc.deallocate(start, end_of_storage - start);
            start = _start;
            finish = _finish;
            end_of_storage = _start + cnt;
        }
        else
        {
            std::__uninitialized_move_if_noexcept_a(start + _diff, finish, start + _diff + _n, alloc);
            finish = std::__uninitialized_copy_a(_list.begin(), _list.end(), start + _diff, alloc);
        }

        return iterator(start + _diff);
    }

    template <class _Ty, typename _Alloc>
    typename vector<_Ty, _Alloc>::iterator vector<_Ty, _Alloc>::erase(const_iterator _position)
    {
        auto _diff = _position - cbegin();
        allocator_type alloc;

        alloc.destroy(start + _diff);
        finish = std::__uninitialized_move_if_noexcept_a(start + _diff + 1, finish, start + _diff, alloc);

        return iterator(start + _diff);
    }

    template <class _Ty, typename _Alloc>
    typename vector<_Ty, _Alloc>::iterator vector<_Ty, _Alloc>::erase(const_iterator _first, const_iterator _last)
    {
        auto _pos = start + (_first - cbegin());
        size_type _n = _last - _first;
        allocator_type alloc;

        for (auto i = 0; i < _n; i++)
        {
            alloc.destroy(_pos + i);
        }
        finish = std::__uninitialized_move_if_noexcept_a(_pos + _n, finish, _pos, alloc);

        return iterator(_pos);
    }

    template <class _Ty, typename _Alloc>
    template <typename... _ARGS>
    typename vector<_Ty, _Alloc>::iterator vector<_Ty, _Alloc>::emplace(const_iterator _position, _ARGS &&... _args)
    {
        auto _n = _position - cbegin();
        allocator_type alloc;

        if (end_of_storage == finish)
        {
            size_type cnt = capacity() ? 2 * capacity() : 1;
            pointer _start = alloc.allocate(cnt);
            pointer _finish = std::__uninitialized_move_if_noexcept_a(start, start + _n, _start, alloc);
            alloc.construct(_finish, std::forward<_ARGS>(_args)...);
            ++_finish;
            _finish = std::__uninitialized_move_if_noexcept_a(start + _n, finish, _finish, alloc);
            for (pointer i = start; i < finish; i++)
            {
                alloc.destroy(i);
            }
            alloc.deallocate(start, end_of_storage - start);
            start = _start;
            finish = _finish;
            end_of_storage = _start + cnt;
        }
        else
        {
            std::__uninitialized_move_if_noexcept_a(start + _n, finish, start + _n + 1, alloc);
            alloc.construct(start + _n, std::forward<_ARGS>(_args)...);
            ++finish;
        }

        return iterator(start + _n);
    }

    template <class _Ty, typename _Alloc>
    template <typename... _ARGS>
    void vector<_Ty, _Alloc>::emplace_back(_ARGS &&... _args)
    {
        allocator_type alloc;
        if (end_of_storage == finish)
        {
            size_type cnt = capacity() ? 2 * capacity() : 1;
            pointer _start = alloc.allocate(cnt);
            pointer _finish = std::__uninitialized_move_if_noexcept_a(start, finish, _start, alloc);
            for (pointer i = start; i < finish; i++)
            {
                alloc.destroy(i);
            }
            alloc.deallocate(start, end_of_storage - start);
            start = _start;
            finish = _finish;
            end_of_storage = _start + cnt;
        }

        alloc.construct(finish, std::forward<_ARGS>(_args)...);
        ++finish;
    }
} // namespace ljq