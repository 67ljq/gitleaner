#pragma once

#include <memory>
#include <initializer_list>

namespace ljq
{
    // list node
    template <typename _Ty>
    struct list_node
    {
        using pointer = struct list_node<_Ty> *;
        pointer prev;
        pointer next;
        _Ty data;
    };

    // list iterator
    template <typename _Ty>
    class list_iterator
    {
    public:
        using pointer = _Ty *;
        using reference = _Ty &;
        using value_type = _Ty;
        using difference_type = ptrdiff_t;
        using size_type = size_t;
        using iterator_category = std::bidirectional_iterator_tag;

        using node = list_node<_Ty> *;
        using self = list_iterator<_Ty>;

    public:
        list_iterator() : _node(nullptr) {}
        explicit list_iterator(node __node) : _node(__node) {}

        reference operator*() const { return _node->data; }
        pointer operator->() const { return &(_node->data); }
        self &operator++()
        {
            _node = _node->next;
            return *this;
        }
        self operator++(int)
        {
            self _tmp = *this;
            _node = _node->next;
            return _tmp;
        }
        self &operator--()
        {
            _node = _node->prev;
            return *this;
        }
        self operator--(int)
        {
            self _tmp = *this;
            _node = _node->prev;
            return _tmp;
        }
        bool operator==(const self &_x) const { return _node == _x._node; }
        bool operator!=(const self &_x) const { return _node != _x._node; }

        // private:
        // friend class list<_Ty>;

        node _node;
    };

    // list const_iterator
    template <typename _Ty>
    class list_const_iterator
    {
    public:
        using pointer = const _Ty *;
        using reference = const _Ty &;
        using value_type = _Ty;
        using difference_type = ptrdiff_t;
        using size_type = size_t;
        using iterator_category = std::bidirectional_iterator_tag;

        using node = const list_node<_Ty> *;
        using self = list_const_iterator<_Ty>;

    public:
        list_const_iterator() : _node(nullptr) {}
        list_const_iterator(const list_iterator<_Ty> &__x) : _node(__x._node) {}
        explicit list_const_iterator(const node __node) : _node(__node) {}

        reference operator*() const { return _node->data; }
        pointer operator->() const { return &(_node->data); }
        self &operator++()
        {
            _node = _node->next;
            return *this;
        }
        self operator++(int)
        {
            self _tmp = *this;
            _node = _node->next;
            return _tmp;
        }
        self &operator--()
        {
            _node = _node->prev;
            return *this;
        }
        self operator--(int)
        {
            self _tmp = *this;
            _node = _node->prev;
            return _tmp;
        }
        bool operator==(const self &_x) const { return _node == _x._node; }
        bool operator!=(const self &_x) const { return _node != _x._node; }

        // private:
        // friend class list<_Ty>;

        node _node;
    };

    template <typename _Ty, typename _Alloc = std::allocator<_Ty>>
    class list
    {
    public:
        using value_type = _Ty;
        using allocator_type = _Alloc;
        using reference = value_type &;
        using const_reference = const value_type &;
        using pointer = typename std::allocator_traits<_Alloc>::pointer;
        using const_pointer = typename std::allocator_traits<_Alloc>::const_pointer;
        using iterator = list_iterator<_Ty>;
        using const_iterator = list_const_iterator<_Ty>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reserse_iterator = std::reverse_iterator<const_iterator>;
        using difference_type = ptrdiff_t;
        using size_type = size_t;

    public:
        // constructor destructor assign
        list();
        explicit list(size_type _n);
        list(size_type _n, const value_type &_x);
        template <typename _Input_iter>
        list(_Input_iter _first, _Input_iter _last);
        list(const list &_x);
        list(list &&_x);
        list(std::initializer_list<value_type> _list);
        ~list();

        list &operator=(const list &_x);
        list &operator=(list &&_x);
        list &operator=(std::initializer_list<value_type> _list);

        // iterator
        iterator begin() noexcept { return iterator(head->next); }
        const_iterator begin() const noexcept { return const_iterator(head->next); }
        iterator end() noexcept { return iterator(head); }
        const_iterator end() const noexcept { return const_iterator(head); }
        reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
        const_reserse_iterator rbegin() const noexcept { return const_reserse_iterator(end()); }
        reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
        const_reserse_iterator rend() const noexcept { return const_reserse_iterator(begin()); }
        const_iterator cbegin() const noexcept { return const_iterator(head->next); }
        const_iterator cend() const noexcept { return const_iterator(head); }
        const_reserse_iterator crbegin() const noexcept { return const_reserse_iterator(end()); }
        const_reserse_iterator crend() const noexcept { return const_reserse_iterator(begin()); }

        // Capacity
        bool empty() const noexcept { return head->next == head; }
        size_type size() const noexcept
        {
            size_type len = 0;
            for (auto _node = head->next; _node != head; _node = _node->next, len++)
                ;
            return len;
        }
        // size_type max_size() const noexcept

        // Element access
        reference front() noexcept { return *begin(); }
        const_reference front() const noexcept { return *begin(); }
        reference back() noexcept
        {
            iterator it = end();
            it--;
            return *it;
        }
        const_reference back() const noexcept
        {
            iterator it = end();
            it--;
            return *it;
        }

        // Modifiers
        template <typename... ARGS>
        void emplace_front(ARGS &&... args);
        void push_front(const value_type &_x);
        void push_front(value_type &&_x);
        void pop_front() { erase(begin()); }
        template <typename... ARGS>
        void emplace_back(ARGS &&... args);
        void push_back(const value_type &_x);
        void push_back(value_type &&_x);
        void pop_back() { erase(--end()); }
        template <typename... ARGS>
        iterator emplace(const_iterator _pos, ARGS &&... args);
        iterator insert(const_iterator _pos, const value_type &_x);
        iterator insert(const_iterator _pos, value_type &&_x);
        iterator insert(const_iterator _pos, size_type _n, const value_type &_x);
        template <typename _Input_iter>
        iterator insert(const_iterator _pos, _Input_iter _first, _Input_iter _last);
        iterator insert(const_iterator _pos, std::initializer_list<value_type> _list);
        iterator erase(const_iterator _pos);
        iterator erase(const_iterator _first, const_iterator _last);
        void swap(list &_x)
        {
            std::swap(head, _x.head);
            std::swap(ty_alloc, _x.ty_alloc);
            std::swap(node_alloc, _x.node_alloc);
        }
        void resize(size_type _n);
        void resize(size_type _n, const value_type &_x);
        void clear() noexcept;

        // Operations
        void remove(const value_type &val);
        template <class Predicate>
        void remove_if(Predicate pred);
        void reverse() noexcept;

    private:
        using link_type = list_node<_Ty> *;
        using _Tp_alloc_type = typename __gnu_cxx::__alloc_traits<_Alloc>::template rebind<_Ty>::other;
        using _Tp_alloc_traits = __gnu_cxx::__alloc_traits<_Tp_alloc_type>;
        using _Node_alloc_type = typename _Tp_alloc_traits::template rebind<list_node<_Ty>>::other;
        using _Node_alloc_traits = __gnu_cxx::__alloc_traits<_Node_alloc_type>;

        link_type head;
        _Tp_alloc_type ty_alloc;
        _Node_alloc_type node_alloc;
    };

    // constructor destructor assign
    template <class _Ty, typename _Alloc>
    list<_Ty, _Alloc>::list()
    {
        head = node_alloc.allocate(1);
        head->prev = head;
        head->next = head;
    }

    template <class _Ty, typename _Alloc>
    list<_Ty, _Alloc>::list(size_type _n)
    {
        head = node_alloc.allocate(1);
        head->prev = head;
        head->next = head;
        for (auto i = 0; i < _n; i++)
        {
            emplace_back();
        }
    }

    template <class _Ty, typename _Alloc>
    list<_Ty, _Alloc>::list(size_type _n, const value_type &_x)
    {
        head = node_alloc.allocate(1);
        head->prev = head;
        head->next = head;
        for (auto i = 0; i < _n; i++)
        {
            push_back(_x);
        }
    }

    template <class _Ty, typename _Alloc>
    template <typename _Input_iter>
    list<_Ty, _Alloc>::list(_Input_iter _first, _Input_iter _last)
    {
        head = node_alloc.allocate(1);
        head->prev = head;
        head->next = head;
        for (auto it = _first; it != _last; it++)
        {
            push_back(*it);
        }
    }

    template <class _Ty, typename _Alloc>
    list<_Ty, _Alloc>::list(const list &_x)
    {
        head = node_alloc.allocate(1);
        head->prev = head;
        head->next = head;
        for (auto it = _x.begin(); it != _x.end(); it++)
        {
            push_back(*it);
        }
    }

    template <class _Ty, typename _Alloc>
    list<_Ty, _Alloc>::list(list &&_x)
    {
        head = node_alloc.allocate(1);
        head->prev = head;
        head->next = head;
        swap(_x);
    }

    template <class _Ty, typename _Alloc>
    list<_Ty, _Alloc>::list(std::initializer_list<value_type> _list)
    {
        head = node_alloc.allocate(1);
        head->prev = head;
        head->next = head;

        for (auto it = _list.begin(); it != _list.end(); it++)
        {
            push_back(*it);
        }
    }

    template <class _Ty, typename _Alloc>
    list<_Ty, _Alloc>::~list()
    {
        for (auto it = begin(); it != end();)
            it = erase(it);
        node_alloc.deallocate(head, 1);
    }

    template <class _Ty, typename _Alloc>
    list<_Ty, _Alloc> &list<_Ty, _Alloc>::operator=(const list &_x)
    {
        clear();

        for (auto it = _x.begin(); it != _x.end(); it++)
        {
            push_back(*it);
        }
    }

    template <class _Ty, typename _Alloc>
    list<_Ty, _Alloc> &list<_Ty, _Alloc>::operator=(list &&_x)
    {
        clear();
        swap(_x);
    }

    template <class _Ty, typename _Alloc>
    list<_Ty, _Alloc> &list<_Ty, _Alloc>::operator=(std::initializer_list<value_type> _list)
    {
        clear();
        for (auto it = _list.begin(); it != _list.end(); it++)
        {
            push_back(*it);
        }
    }

    template <class _Ty, typename _Alloc>
    template <typename... ARGS>
    void list<_Ty, _Alloc>::emplace_front(ARGS &&... args)
    {
        emplace(begin(), std::forward<ARGS>(args)...);
    }

    template <class _Ty, typename _Alloc>
    void list<_Ty, _Alloc>::push_front(const value_type &_x)
    {
        link_type node = node_alloc.allocate(1);
        ty_alloc.construct(&(node->data), _x);
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }

    template <class _Ty, typename _Alloc>
    void list<_Ty, _Alloc>::push_front(value_type &&_x)
    {
        link_type node = node_alloc.allocate(1);
        ty_alloc.construct(&(node->data), std::move(_x));
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }

    // template <class _Ty, typename _Alloc>
    // void list<_Ty, _Alloc>::pop_front()
    // {

    // }

    template <class _Ty, typename _Alloc>
    template <typename... ARGS>
    void list<_Ty, _Alloc>::emplace_back(ARGS &&... args)
    {
        emplace(end(), std::forward<ARGS>(args)...);
    }

    template <class _Ty, typename _Alloc>
    void list<_Ty, _Alloc>::push_back(const value_type &_x)
    {
        link_type node = node_alloc.allocate(1);
        ty_alloc.construct(&(node->data), _x);
        node->next = head;
        node->prev = head->prev;
        head->prev->next = node;
        head->prev = node;
    }

    template <class _Ty, typename _Alloc>
    void list<_Ty, _Alloc>::push_back(value_type &&_x)
    {
        link_type node = node_alloc.allocate(1);
        ty_alloc.construct(&(node->data), std::move(_x));
        node->next = head;
        node->prev = head->prev;
        head->prev->next = node;
        head->prev = node;
    }

    template <class _Ty, typename _Alloc>
    template <typename... ARGS>
    typename list<_Ty, _Alloc>::iterator list<_Ty, _Alloc>::emplace(const_iterator _pos, ARGS &&... args)
    {
        link_type pos = const_cast<link_type>(_pos._node);
        link_type node = node_alloc.allocate(1);
        ty_alloc.construct(&(node->data), std::forward<ARGS>(args)...);
        node->next = pos;
        node->prev = pos->prev;
        pos->prev->next = node;
        pos->prev = node;
        return iterator(node);
    }

    template <class _Ty, typename _Alloc>
    typename list<_Ty, _Alloc>::iterator list<_Ty, _Alloc>::insert(const_iterator _pos, const value_type &_x)
    {
        link_type pos = const_cast<link_type>(_pos._node);
        link_type node = node_alloc.allocate(1);
        ty_alloc.construct(&(node->data), _x);
        node->next = pos;
        node->prev = pos->prev;
        pos->prev->next = node;
        pos->prev = node;
        return iterator(node);
    }

    template <class _Ty, typename _Alloc>
    typename list<_Ty, _Alloc>::iterator list<_Ty, _Alloc>::insert(const_iterator _pos, value_type &&_x)
    {
        return emplace(_pos, std::move(_x));
    }

    template <class _Ty, typename _Alloc>
    typename list<_Ty, _Alloc>::iterator list<_Ty, _Alloc>::insert(const_iterator _pos, size_type _n, const value_type &_x)
    {
        iterator it(const_cast<link_type>(_pos._node));
        for (auto i = 0; i < _n; i++)
        {
            it = insert(it, _x);
        }
        return it;
    }

    template <class _Ty, typename _Alloc>
    template <typename _Input_iter>
    typename list<_Ty, _Alloc>::iterator list<_Ty, _Alloc>::insert(const_iterator _pos, _Input_iter _first, _Input_iter _last)
    {
        iterator pos(const_cast<link_type>(_pos._node));
        // for (auto it = _first; it != _last; it++)
        // {
        //     pos = insert(pos, *it);
        // }
        return pos;
    }

    template <class _Ty, typename _Alloc>
    typename list<_Ty, _Alloc>::iterator list<_Ty, _Alloc>::insert(const_iterator _pos, std::initializer_list<value_type> _list)
    {
        iterator pos(const_cast<link_type>(_pos._node));
        for (auto it = _list.end() - 1;;)
        {
            pos = insert(pos, *it);
            it--;
            if (it == _list.begin())
            {
                pos = insert(pos, *it);
                break;
            }
        }
        return pos;
    }

    template <class _Ty, typename _Alloc>
    typename list<_Ty, _Alloc>::iterator list<_Ty, _Alloc>::erase(const_iterator _pos)
    {
        link_type node = const_cast<link_type>(_pos._node);
        link_type node_ret = node->next;
        node->next->prev = node->prev;
        node->prev->next = node->next;
        ty_alloc.destroy(&node->data);
        node_alloc.deallocate(node, 1);
        return iterator(node_ret);
    }

    template <class _Ty, typename _Alloc>
    typename list<_Ty, _Alloc>::iterator list<_Ty, _Alloc>::erase(const_iterator _first, const_iterator _last)
    {
        iterator first(const_cast<link_type>(_first._node));
        iterator last(const_cast<link_type>(_last._node));
        auto it = first;
        for (; it != last;)
        {
            it = erase(it);
        }
        return it;
    }

    template <class _Ty, typename _Alloc>
    void list<_Ty, _Alloc>::resize(size_type _n)
    {
        size_type size = 0;
        for (auto it = begin(); it != end();)
        {
            size++;
            if (size > _n)
                it = erase(it);
            else
                it++;
        }
        for (; size < _n; size++)
        {
            emplace_back();
        }
    }

    template <class _Ty, typename _Alloc>
    void list<_Ty, _Alloc>::resize(size_type _n, const value_type &_x)
    {
        size_type size = 0;
        for (auto it = begin(); it != end();)
        {
            size++;
            if (size > _n)
                it = erase(it);
            else
                it++;
        }
        for (; size < _n; size++)
        {
            push_back(_x);
        }
    }

    template <class _Ty, typename _Alloc>
    void list<_Ty, _Alloc>::clear() noexcept
    {
        for (auto it = begin(); it != end();)
            it = erase(it);
    }

    // Operations
    template <class _Ty, typename _Alloc>
    void list<_Ty, _Alloc>::remove(const value_type &val)
    {
        for (auto it = begin(); it != end();)
        {
            if (*it == val)
                it = erase(it);
            else
                it++;
        }
    }

    template <class _Ty, typename _Alloc>
    template <class Predicate>
    void list<_Ty, _Alloc>::remove_if(Predicate pred)
    {
        for (auto it = begin(); it != end();)
        {
            if (pred(*it))
                it = erase(it);
            else
                it++;
        }
    }

    template <class _Ty, typename _Alloc>
    void list<_Ty, _Alloc>::reverse() noexcept
    {
        if (empty())
            return;

        link_type pnode = head;
        link_type pnext;
        while (true)
        {
            pnext = pnode->next;
            pnode->next = pnode->prev;
            pnode->prev = pnext;

            if (pnext == head)
                break;

            pnode = pnext;
        }
    }
} // namespace ljq
