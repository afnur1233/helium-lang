#ifndef _INCLUDE_DA_C
#define _INCLUDE_DA_C

#include <mem.c>

#define da_reserve(_allocator,_da,_size)((bool)({\
    struct allocator *m_da_reserve_allocator = (_allocator);\
    __typeof__(_da) m_da_reserve_da = (_da);\
    size_t m_da_reserve_size = _size;\
    size_t m_da_reserve_new_cap = 1;\
    while (m_da_reserve_new_cap < m_da_reserve_size) m_da_reserve_new_cap *= 2;\
    bool m_da_reserve_should_alloc = m_da_reserve_size > m_da_reserve_da->cap;\
    \
    if (m_da_reserve_da->items == NULL && m_da_reserve_should_alloc) {\
        m_da_reserve_da->items = m_da_reserve_allocator->alloc(\
            m_da_reserve_allocator,\
            sizeof(*m_da_reserve_da->items) * m_da_reserve_new_cap\
        );\
    } else if (m_da_reserve_should_alloc) {\
        m_da_reserve_da->items = m_da_reserve_allocator->realloc(\
            m_da_reserve_allocator,\
            m_da_reserve_da->items,\
            sizeof(*m_da_reserve_da->items) * m_da_reserve_da->cap,\
            sizeof(*m_da_reserve_da->items) * m_da_reserve_new_cap\
        );\
    }\
    \
    if (!(m_da_reserve_should_alloc && m_da_reserve_da->items == NULL))\
        m_da_reserve_da->cap = m_da_reserve_new_cap;\
    \
    !(m_da_reserve_should_alloc && m_da_reserve_da->items == NULL);\
}))

#define da_push(_allocator,_da,...)({\
    struct allocator *m_da_push_allocator = (_allocator);\
    __typeof__(_da) m_da_push_da = (_da);\
    __typeof__(__VA_ARGS__) m_da_push_item = (__VA_ARGS__);\
    bool result = da_reserve(m_da_push_allocator,m_da_push_da,m_da_push_da->len + 1);\
    \
    if (result) {\
        m_da_push_da->items[m_da_push_da->len++] = m_da_push_item;\
    }\
    result;\
})

#define da_remove_unordered(_da,_i)(()({\
    __typeof__(_da) m_da_remove_unordered_da = (_da);\
    size_t m_da_remove_unordered_i = (_i);\
    \
    if (m_da_remove_unordered_da->len != 0 && m_da_remove_unordered_i < m_da_remove_unordered_da->len) {\
        m_da_remove_unordered_da->items[m_da_remove_unordered_i] = m_da_remove_unordered_da->items[--m_da_remove_unordered_da->len];\
    }\
    \
    true;\
}))

#define da_free(_allocator,_da)((void *)({\
    struct allocator *m_da_push_allocator = (_allocator);\
    __typeof__(_da) m_da_remove_unordered_da = (_da);\
    \
    if (m_da_remove_unordered_da->items != NULL) {\
        m_da_remove_unordered_da->items = m_da_push_allocator->free(\
            m_da_push_allocator,\
            m_da_remove_unordered_da->items,\
            sizeof(*m_da_remove_unordered_da->items) * m_da_remove_unordered_da->cap\
        );\
        m_da_remove_unordered_da->cap = 0;\
        m_da_remove_unordered_da->len = 0;\
    }\
    \
    NULL;\
}))

#endif // _INCLUDE_DA_C