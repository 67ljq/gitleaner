#pragma once

#include <memory>
#include <initializer_list>

#include <iostream>

namespace htd
{
    template <typename _Ty>
    struct forward_list_node
    {
        using value_type = _Ty;
        struct forward_list_node<_Ty> *next;
        value_type val;
    };

    template <typename _Ty>
    class forward_list_iterator
    {
    public:
        using value_type = _Ty;
        using pointer = value_type *;
        using reference = value_type &;
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        using iterator_category = std::forward_iterator_tag;

        using self = forward_list_iterator<_Ty>;
        using link_node_value = struct forward_list_node<value_type>;
        using link_node = link_node_value *;

        forward_list_iterator() noexcept : node(nullptr) {}
        explicit forward_list_iterator(link_node n) noexcept : node(n) {}

        reference operator*() noexcept { return node->val; }
        pointer operator->() noexcept { return &node->val; }
        self &operator++() noexcept
        {
            node = node->next;
            return *this;
        }
        self operator++(int) noexcept
        {
            auto tmp = *this;
            node = node->next;
            return tmp;
        }
        bool operator==(const self &it) const noexcept { return node == it.node; }
        bool operator!=(const self &it) const noexcept { return node != it.node; }

        link_node node;
    };

    template <typename _Ty>
    class const_forward_list_iterator
    {
    public:
        using value_type = _Ty;
        using pointer = const value_type *;
        using reference = const value_type &;
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        using iterator_category = std::forward_iterator_tag;

        using self = const_forward_list_iterator<_Ty>;
        using link_node_value = struct forward_list_node<value_type>;
        using link_node = const link_node_value *;
        using iterator = forward_list_iterator<_Ty>;

        const_forward_list_iterator() noexcept : node(nullptr) {}
        explicit const_forward_list_iterator(link_node n) noexcept : node(n) {}
        const_forward_list_iterator(const iterator &it) noexcept : node(it.node) {}

        reference operator*() noexcept { return node->val; }
        pointer operator->() noexcept { return &node->val; }
        self &operator++() noexcept
        {
            node = node->next;
            return *this;
        }
        self operator++(int) noexcept
        {
            auto tmp = *this;
            node = node->next;
            return tmp;
        }
        bool operator==(const self &it) const noexcept { return node == it.node; }
        bool operator!=(const self &it) const noexcept { return node != it.node; }

        link_node node;
    };

    template <typename _Ty, typename _Alloc = std::allocator<_Ty>>
    class forward_list
    {
    public:
        using value_type = _Ty;
        using allocator_type = _Alloc;
        using reference = value_type &;
        using const_reference = const value_type &;
        using pointer = typename std::allocator_traits<allocator_type>::pointer;
        using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;
        using iterator = forward_list_iterator<value_type>;
        using const_iterator = const_forward_list_iterator<value_type>;
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;

    public:
        // Constructor
        forward_list();
        explicit forward_list(size_type n);
        forward_list(size_type n, const value_type &val);
        template <typename _Input_itor>
        forward_list(_Input_itor first, _Input_itor last);
        forward_list(const forward_list &list);
        forward_list(forward_list &&list);
        forward_list(std::initializer_list<_Ty> list);
        ~forward_list();

        forward_list &operator=(const forward_list &list);
        forward_list &operator=(forward_list &&list);
        forward_list &operator=(std::initializer_list<_Ty> list);

        // Iterator
        iterator before_begin() noexcept { return iterator(head); }
        const_iterator before_begin() const noexcept { return const_iterator(head); }
        iterator begin() noexcept { return iterator(head->next); }
        const_iterator begin() const noexcept { return const_iterator(head->next); }
        iterator end() noexcept { return iterator(nullptr); }
        const_iterator end() const noexcept { return const_iterator(nullptr); }
        const_iterator cbefore_begin() const noexcept { return const_iterator(head); }
        const_iterator cbegin() const noexcept { return const_iterator(head->next); }
        const_iterator cend() const noexcept { return const_iterator(nullptr); }

        // Capacity
        bool empty() const noexcept { return head->next == nullptr; }
        size_type max_size() const noexcept { return node_alloc_traits::max_size(node_allocater); }

        // Element access
        reference front() { return head->next->val; }
        const_reference front() const { return head->next->val; }

        // Modifiers
        template <typename _Input_itor>
        void assign(_Input_itor first, _Input_itor last);
        void assign(size_type n, const value_type &val);
        void assign(std::initializer_list<_Ty> list);
        template <typename... _Args>
        void emplace_front(_Args &&... args) { m_insert_after(this->before_begin(), std::forward<_Args>(args)...); }
        void push_front(const value_type &val) { m_insert_after(this->before_begin(), val); }
        void push_front(value_type &&val) { m_insert_after(this->before_begin(), std::move(val)); }
        void pop_front() { erase_after(before_begin()); }
        template <typename... _Args>
        iterator emplace_after(const_iterator pos, _Args &&... args) { return iterator(m_insert_after(pos, std::forward<_Args>(args)...)); }
        iterator insert_after(const_iterator pos, const value_type &val) { return iterator(m_insert_after(pos, val)); }
        iterator insert_after(const_iterator pos, value_type &&val) { return iterator(m_insert_after(pos, std::move(val))); }
        iterator insert_after(const_iterator pos, size_type n, const value_type &val);
        template <typename _Input_itor>
        iterator insert_after(const_iterator pos, _Input_itor first, _Input_itor last);
        iterator insert_after(const_iterator pos, std::initializer_list<_Ty> list) { return insert_after(pos, list.begin(), list.end()); }
        iterator erase_after(const_iterator pos);
        iterator erase_after(const_iterator first, const_iterator last);
        void swap(forward_list &list) noexcept { std::swap(head, list.head); }
        void resize(size_type sz);
        void resize(size_type sz, const value_type &val);
        void clear() noexcept { erase_after(before_begin(), end()); }

        // Operations
        void splice_after(const_iterator pos, forward_list &list) noexcept { splice_after(pos, std::move(list)); }
        void splice_after(const_iterator pos, forward_list &&list) noexcept;
        void splice_after(const_iterator pos, forward_list &list, const_iterator i) noexcept { splice_after(pos, std::move(list), i); }
        void splice_after(const_iterator pos, forward_list &&list, const_iterator i) noexcept;
        void splice_after(const_iterator pos, forward_list &list, const_iterator first, const_iterator last) noexcept
        {
            splice_after(pos, std::move(list), first, last);
        }
        void splice_after(const_iterator pos, forward_list &&list, const_iterator first, const_iterator last) noexcept;

    private:
        template <typename _Input_itor>
        void initializer_by_range(_Input_itor first, _Input_itor last);
        template <typename... _Args>
        struct forward_list_node<_Ty> *m_insert_after(const_iterator pos, _Args &&... args);

        using link_node_value = struct forward_list_node<value_type>;
        using link_node = link_node_value *;
        using tp_alloc_type = typename __gnu_cxx::__alloc_traits<allocator_type>::template rebind<value_type>::other;
        using alloc_traits = __gnu_cxx::__alloc_traits<tp_alloc_type>;
        using node_alloc_type = typename alloc_traits::template rebind<link_node_value>::other;
        using node_alloc_traits = __gnu_cxx::__alloc_traits<node_alloc_type>;

        link_node head;
        tp_alloc_type value_allocater;
        node_alloc_type node_allocater;
    };

    template <typename _Ty, typename _Alloc>
    forward_list<_Ty, _Alloc>::forward_list()
    {
        head = node_allocater.allocate(1);
        head->next = nullptr;
    }

    template <typename _Ty, typename _Alloc>
    forward_list<_Ty, _Alloc>::forward_list(size_type n)
    {
        head = node_allocater.allocate(1);
        head->next = nullptr;
        link_node tail = head;
        for (auto i = 0; i < n; i++)
        {
            link_node node = node_allocater.allocate(1);
            node->next = nullptr;
            value_allocater.construct(&node->val);
            tail->next = node;
            tail = node;
        }
    }

    template <typename _Ty, typename _Alloc>
    forward_list<_Ty, _Alloc>::forward_list(size_type n, const value_type &val)
    {
        head = node_allocater.allocate(1);
        head->next = nullptr;
        link_node tail = head;
        for (auto i = 0; i < n; i++)
        {
            link_node node = node_allocater.allocate(1);
            node->next = nullptr;
            value_allocater.construct(&node->val, val);
            tail->next = node;
            tail = node;
        }
    }

    template <typename _Ty, typename _Alloc>
    template <typename _Input_itor>
    forward_list<_Ty, _Alloc>::forward_list(_Input_itor first, _Input_itor last)
    {
        initializer_by_range(first, last);
    }

    template <typename _Ty, typename _Alloc>
    forward_list<_Ty, _Alloc>::forward_list(const forward_list &list)
    {
        head = node_allocater.allocate(1);
        head->next = nullptr;
        link_node tail = head;
        link_node copy_node = list.head->next;
        while (copy_node != nullptr)
        {
            link_node node = node_allocater.allocate(1);
            node->next = nullptr;
            value_allocater.construct(&node->val, copy_node->val);
            tail->next = node;
            tail = node;
            copy_node = copy_node->next;
        }
    }

    template <typename _Ty, typename _Alloc>
    forward_list<_Ty, _Alloc>::forward_list(forward_list &&list)
    {
        head = nullptr;
        std::swap(head, list.head);
    }

    template <typename _Ty, typename _Alloc>
    forward_list<_Ty, _Alloc>::forward_list(std::initializer_list<_Ty> list)
    {
        initializer_by_range(list.begin(), list.end());
    }

    template <typename _Ty, typename _Alloc>
    forward_list<_Ty, _Alloc>::~forward_list()
    {
        link_node node = head ? head->next : nullptr;
        link_node tmp = node;
        while (node != nullptr)
        {
            tmp = node->next;
            value_allocater.destroy(&node->val);
            node_allocater.deallocate(node, 1);
            node = tmp;
        }
        if (head)
            node_allocater.deallocate(head, 1);
    }

    template <typename _Ty, typename _Alloc>
    forward_list<_Ty, _Alloc> &forward_list<_Ty, _Alloc>::operator=(const forward_list &list)
    {
        link_node node = head->next;
        link_node tail = head;
        link_node copy_node = list.head->next;
        while (copy_node != nullptr)
        {
            if (node == nullptr)
            {
                node = node_allocater.allocate(1);
                node->next = nullptr;
                value_allocater.construct(&node->val, copy_node->val);
                tail->next = node;
            }
            else
            {
                node->val = copy_node->val;
            }
            tail = node;
            node = node->next;
            copy_node = copy_node->next;
        }
        tail->next = nullptr;
        while (node != nullptr)
        {
            tail = node->next;
            value_allocater.destroy(&node->val);
            node_allocater.deallocate(node, 1);
            node = tail;
        }

        return *this;
    }

    template <typename _Ty, typename _Alloc>
    forward_list<_Ty, _Alloc> &forward_list<_Ty, _Alloc>::operator=(forward_list &&list)
    {
        link_node node = head ? head->next : nullptr;
        link_node tmp = node;
        while (node != nullptr)
        {
            tmp = node->next;
            value_allocater.destroy(&node->val);
            node_allocater.deallocate(node, 1);
            node = tmp;
        }
        head->next = nullptr;
        std::swap(head, list.head);

        return *this;
    }

    template <typename _Ty, typename _Alloc>
    forward_list<_Ty, _Alloc> &forward_list<_Ty, _Alloc>::operator=(std::initializer_list<_Ty> list)
    {
        link_node node = head->next;
        link_node tail = head;
        for (auto it = list.begin(); it != list.end(); it++)
        {
            if (node == nullptr)
            {
                node = node_allocater.allocate(1);
                node->next = nullptr;
                value_allocater.construct(&node->val, *it);
                tail->next = node;
            }
            else
            {
                node->val = *it;
            }
            tail = node;
            node = node->next;
        }
        tail->next = nullptr;
        while (node != nullptr)
        {
            tail = node->next;
            value_allocater.destroy(&node->val);
            node_allocater.deallocate(node, 1);
            node = tail;
        }

        return *this;
    }

    template <typename _Ty, typename _Alloc>
    template <typename _Input_itor>
    void forward_list<_Ty, _Alloc>::assign(_Input_itor first, _Input_itor last)
    {
        link_node node = head->next;
        link_node tail = head;
        for (auto it = first; it != last; it++)
        {
            if (node == nullptr)
            {
                node = node_allocater.allocate(1);
                node->next = nullptr;
                value_allocater.construct(&node->val, *it);
                tail->next = node;
            }
            else
            {
                node->val = *it;
            }
            tail = node;
            node = node->next;
        }
        tail->next = nullptr;
        while (node != nullptr)
        {
            tail = node->next;
            value_allocater.destroy(&node->val);
            node_allocater.deallocate(node, 1);
            node = tail;
        }
    }

    template <typename _Ty, typename _Alloc>
    void forward_list<_Ty, _Alloc>::assign(size_type n, const value_type &val)
    {
        link_node node = head->next;
        link_node tail = head;
        while (n-- > 0)
        {
            if (node == nullptr)
            {
                node = node_allocater.allocate(1);
                node->next = nullptr;
                value_allocater.construct(&node->val, val);
                tail->next = node;
            }
            else
            {
                node->val = val;
            }
            tail = node;
            node = node->next;
        }
        tail->next = nullptr;
        while (node != nullptr)
        {
            tail = node->next;
            value_allocater.destroy(&node->val);
            node_allocater.deallocate(node, 1);
            node = tail;
        }
    }

    template <typename _Ty, typename _Alloc>
    void forward_list<_Ty, _Alloc>::assign(std::initializer_list<_Ty> list)
    {
        link_node node = head->next;
        link_node tail = head;
        for (auto it = list.begin(); it != list.end(); it++)
        {
            if (node == nullptr)
            {
                node = node_allocater.allocate(1);
                node->next = nullptr;
                value_allocater.construct(&node->val, *it);
                tail->next = node;
            }
            else
            {
                node->val = *it;
            }
            tail = node;
            node = node->next;
        }
        tail->next = nullptr;
        while (node != nullptr)
        {
            tail = node->next;
            value_allocater.destroy(&node->val);
            node_allocater.deallocate(node, 1);
            node = tail;
        }
    }

    template <typename _Ty, typename _Alloc>
    typename forward_list<_Ty, _Alloc>::iterator forward_list<_Ty, _Alloc>::insert_after(const_iterator pos, size_type n, const value_type &val)
    {
        link_node node = const_cast<link_node>(pos.node);
        link_node tmp = node;
        for (auto i = 0; i < n; i++)
        {
            node = node_allocater.allocate(1);
            value_allocater.construct(&node->val, val);
            node->next = tmp->next;
            tmp->next = node;
            tmp = node;
        }
        return iterator(node);
    }

    template <typename _Ty, typename _Alloc>
    template <typename _Input_itor>
    typename forward_list<_Ty, _Alloc>::iterator forward_list<_Ty, _Alloc>::insert_after(const_iterator pos, _Input_itor first, _Input_itor last)
    {
        link_node node = const_cast<link_node>(pos.node);
        link_node tmp = node;
        for (auto it = first; it != last; it++)
        {
            node = node_allocater.allocate(1);
            value_allocater.construct(&node->val, *it);
            node->next = tmp->next;
            tmp->next = node;
            tmp = node;
        }
        return iterator(node);
    }

    template <typename _Ty, typename _Alloc>
    typename forward_list<_Ty, _Alloc>::iterator forward_list<_Ty, _Alloc>::erase_after(const_iterator pos)
    {
        link_node tmp = const_cast<link_node>(pos.node);
        link_node node = tmp->next;
        tmp->next = node->next;
        value_allocater.destroy(&node->val);
        node_allocater.deallocate(node, 1);
        return iterator(tmp->next);
    }

    template <typename _Ty, typename _Alloc>
    typename forward_list<_Ty, _Alloc>::iterator forward_list<_Ty, _Alloc>::erase_after(const_iterator first, const_iterator last)
    {
        link_node first_node = const_cast<link_node>(first.node);
        link_node last_node = const_cast<link_node>(last.node);
        for (auto node = first_node->next; node != last_node;)
        {
            auto tmp = node->next;
            value_allocater.destroy(&node->val);
            node_allocater.deallocate(node, 1);
            node = tmp;
        }
        first_node->next = last_node;
        return iterator(last_node);
    }

    template <typename _Ty, typename _Alloc>
    void forward_list<_Ty, _Alloc>::resize(size_type sz)
    {
        iterator it = before_begin();
        size_type len = 0;
        while (it.node->next != nullptr && len < sz)
        {
            it++;
            len++;
        }
        if (len == sz)
            erase_after(it, end());
        else
        {
            link_node node = it.node;
            link_node tmp = node;
            for (auto i = 0; i < sz - len; i++)
            {
                node = node_allocater.allocate(1);
                value_allocater.construct(&node->val);
                node->next = tmp->next;
                tmp->next = node;
                tmp = node;
            }
        }
    }

    template <typename _Ty, typename _Alloc>
    void forward_list<_Ty, _Alloc>::resize(size_type sz, const value_type &val)
    {
        iterator it = before_begin();
        size_type len = 0;
        while (it.node->next != nullptr && len < sz)
        {
            it++;
            len++;
        }
        if (len == sz)
            erase_after(it, end());
        else
            insert_after(it, sz - len, val);
    }

    template <typename _Ty, typename _Alloc>
    void forward_list<_Ty, _Alloc>::splice_after(const_iterator pos, forward_list &&list) noexcept
    {
        link_node node = const_cast<link_node>(pos.node);
        link_node next = node->next;
        node->next = list.head->next;
        list.head->next = nullptr;
        while (node->next != nullptr)
            node = node->next;
        node->next = next;
    }

    template <typename _Ty, typename _Alloc>
    void forward_list<_Ty, _Alloc>::splice_after(const_iterator pos, forward_list &&list, const_iterator i) noexcept
    {
        link_node node = const_cast<link_node>(pos.node);
        link_node tmp = const_cast<link_node>(i.node);
        link_node j = tmp->next;
        if (tmp == nullptr || j == nullptr)
            return;
        tmp->next = j->next;
        j->next = node->next;
        node->next = j;
    }

    template <typename _Ty, typename _Alloc>
    void forward_list<_Ty, _Alloc>::splice_after(const_iterator pos, forward_list &&list, const_iterator first, const_iterator last) noexcept
    {
        link_node node = const_cast<link_node>(pos.node);
        link_node start = const_cast<link_node>(first.node);
        link_node finish = const_cast<link_node>(last.node);
        link_node tmp = start;
        link_node keep = start->next;
        while (tmp->next != nullptr && tmp->next != finish)
            tmp = tmp->next;
        start->next = tmp->next;
        tmp->next = node->next;
        node->next = keep;
    }

    template <typename _Ty, typename _Alloc>
    template <typename _Input_itor>
    void forward_list<_Ty, _Alloc>::initializer_by_range(_Input_itor first, _Input_itor last)
    {
        head = node_allocater.allocate(1);
        head->next = nullptr;
        link_node tail = head;
        for (auto it = first; it != last; it++)
        {
            link_node node = node_allocater.allocate(1);
            node->next = nullptr;
            value_allocater.construct(&node->val, *it);
            tail->next = node;
            tail = node;
        }
    }

    // typename forward_list<_Ty, _Alloc>::link_node
    template <typename _Ty, typename _Alloc>
    template <typename... _Args>
    struct forward_list_node<_Ty> *forward_list<_Ty, _Alloc>::m_insert_after(const_iterator pos, _Args &&... args)
    {
        link_node tmp = const_cast<link_node>(pos.node);
        link_node node = node_allocater.allocate(1);
        value_allocater.construct(&node->val, std::forward<_Args>(args)...);
        node->next = tmp->next;
        tmp->next = node;
        return node;
    }
} // namespace htd