#ifndef _M522VEC_H
#define _M522VEC_H

/* The CodeWarrior MSL std::vector as compiled in m522Dll: {capacity, size,
 * data}, memory from fn_1_689C / fn_1_6964.  MWCC never inlines a function
 * that contains a loop, so the loop-free helpers live in the class and every
 * operation with loops is spelled out per instantiation by one of the
 * m522vec_*.inc files, which are included with M522_FN (the function name),
 * M522_VEC (the vector type) and M522_T (the element type) defined.  Elements
 * that own memory provide a m522Destroy(T *) overload before the vector is
 * instantiated. */

#include "REL/m522Dll.h"

extern "C" {
void abort(void);
void *fn_1_689C(u32 size);
void fn_1_6964(void *ptr);
s32 fn_1_6AD0(void *file, const char *msg);
}

#ifdef TARGET_PC
#include <new>
#include <stdio.h>
#define M522_STDERR ((void *)stderr)
#else
extern "C" u8 __files[];
#define M522_STDERR ((void *)(__files + 0xA0))
inline void *operator new(unsigned long, void *p) { return p; }
#endif

template <class T>
inline void m522Destroy(T *p)
{
}

template <class T>
class M522Vec {
public:
    u32 m_cap;
    u32 m_size;
    T *m_data;

    static u32 maxSizeA()
    {
        u32 v = 0xFFFFFFFFu / sizeof(T);
        return v;
    }
    static u32 maxSizeB()
    {
        u32 v = maxSizeA();
        return v;
    }
    static u32 maxSize()
    {
        u32 v = maxSizeB();
        return v;
    }
    u32 capacity()
    {
        u32 *pc = &m_cap;
        return *pc;
    }
    T *begin()
    {
        T **pp = &m_data;
        return *pp;
    }
    T *end()
    {
        T **pp = &m_data;
        return *pp + m_size;
    }

    static void construct(T *p, const T &x)
    {
        if (p != 0) {
            new (p) T(x);
        }
    }

    T *allocate(u32 n)
    {
        M522Vec *owner = this;
        T *p = (T *)fn_1_689C(n * sizeof(T));
        if (p == 0) {
            fn_1_6AD0(M522_STDERR, "Memory allocation failure");
            abort();
        }
        return p;
    }
};

/* Destroy [m_data, end) back to front and empty the vector. */
#define M522_VEC_CLEAR(VECP, T)                     \
    do {                                            \
        T *clearFirst = (VECP)->begin();            \
        T *clearLast = clearFirst + (VECP)->m_size; \
        while (clearLast > clearFirst) {            \
            clearLast--;                            \
            m522Destroy(clearLast);                 \
        }                                           \
        (VECP)->m_size = 0;                         \
    } while (0)

#endif
