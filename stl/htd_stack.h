#pragma once

#include "htd_deque.h"

namespace htd
{
    template <typename _Ty, typename _Sequence = htd::deque<_Ty>>
    class stack
    {
    public:
        using value_type = typename _Sequence::value_type;
        using reference = typename _Sequence::reference;
        using const_reference = typename _Sequence::const_reference;
        using size_type = typename _Sequence::size_type;
        using container_type = _Sequence;

    public:
        // Construct
        stack() : c() {}
        explicit stack(const _Sequence &s) : c(s) {}
        explicit stack(_Sequence &&s) : c(std::move(s)) {}

        bool empty() const { return c.empty(); }
        size_type size() const { return c.size(); }
        reference top() { return c.back(); }
        const_reference top() const { return c.back(); }
        void push(const value_type &val) { c.push_back(val); }
        void push(value_type &&val) { c.push_back(std::move(val)); }
        template <typename... _Args>
        void emplace(_Args &&... args) { c.emplace_back(std::forward<_Args>(args)...); }
        void pop() { c.pop_back(); }
        void swap(stack &s) { std::swap(c, s.c); }

    private:
        _Sequence c;
    };
} // namespace htd