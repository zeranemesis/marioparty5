void M522_FN(M522_VEC *self, u32 n, M522_T *xp)
{
    const M522_T &x = *xp;
    M522_T *p;
    M522_T *e;
    M522_T *tail;
    u32 count;

    if (n <= self->capacity()) {
        count = n < self->m_size ? n : self->m_size;
        p = self->begin();
        e = p + count;
        while (p < e) {
            *p = x;
            p++;
        }
        if (n < self->m_size) {
            tail = self->end();
            while (p < tail) {
                m522Destroy(p);
                p++;
            }
            self->m_size = n;
        }
        if (self->m_size < n) {
            n -= self->m_size;
            while (n != 0) {
                M522_VEC::construct(p, x);
                n--;
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
        while (n != 0) {
            M522_VEC::construct(p, x);
            n--;
            p++;
            self->m_size++;
        }
    }
}
