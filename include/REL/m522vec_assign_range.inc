void M522_FN(M522_VEC *self, const M522_T *first, const M522_T *last)
{
    u32 n = last - first;
    M522_T *p;
    M522_T *e;
    M522_T *tail;
    u32 count;

    if (n <= self->capacity()) {
        count = n < self->m_size ? n : self->m_size;
        p = self->begin();
        e = p + count;
        while (p < e) {
            *p = *first;
            p++;
            first++;
        }
        if (n < self->m_size) {
            tail = self->end();
            while (p < tail) {
                m522Destroy(p);
                p++;
            }
            self->m_size = n;
        } else if (self->m_size < n) {
            while (first != last) {
                M522_VEC::construct(p, *first);
                first++;
                p++;
                self->m_size++;
            }
        }
    } else {
        if (n > M522_VEC::maxSize()) {
            fn_1_6AD0(M522_STDERR, "vector length error\n");
            abort();
        }
        M522_VEC_CLEAR(self, M522_T);
        if (self->m_data != 0) {
            fn_1_6964(self->m_data);
            self->m_data = 0;
            self->m_cap = 0;
        }
        self->m_data = self->allocate(n);
        self->m_cap = n;
        p = self->m_data;
        while (first != last) {
            M522_VEC::construct(p, *first);
            first++;
            p++;
            self->m_size++;
        }
    }
}
