#pragma once

#include <algorithm>
#include "htd_deque.h"

namespace htd
{
    template <typename _Ty, typename _Sequenece = htd::deque<_Ty>>
    class queue
    {
    public:
        using value_type = typename _Sequenece::value_type;
        using reference = typename _Sequenece::reference;
        using const_reference = typename _Sequenece::const_reference;
        using size_type = typename _Sequenece::size_type;
        using container_type = _Sequenece;

    public:
        // Constructs
        queue() : c() {}
        explicit queue(const _Sequenece &s) : c(s) {}
        explicit queue(_Sequenece &&s) : c(std::move(s)) {}
        queue(const queue &q) : c(q.c) {}
        queue(queue &&q) : c(std::move(q.c)) {}

        // Member function
        bool empty() const { return c.empty(); }
        size_type size() const { return c.size(); }
        reference front() { return c.front(); }
        const_reference front() const { return c.front(); }
        reference back() { return c.back(); }
        const_reference back() const { return c.back(); }
        void push(const value_type &val) { c.push_back(val); }
        void push(value_type &&val) { c.push_back(std::move(val)); }
        template <typename... _Args>
        void emplace(_Args &&... args) { return c.emplace_back(std::forward<_Args>(args)...); }
        void pop() { c.pop_front(); }
        void swap(queue &que) { std::swap(c, que.c); }

    private:
        _Sequenece c;
    };
} // namespace htd