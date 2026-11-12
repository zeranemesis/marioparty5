void M522_FN(M522_VEC *self, u32 n, M522_T *xp)
{
    const M522_T &x = *xp;
    M522_T *p;

    if (n != 0) {
        if (n > M522_VEC::maxSize()) {
            fn_1_6AD0(M522_STDERR, "vector length error\n");
            abort();
        }
        self->m_data = self->allocate(n);
        self->m_cap = n;
        p = self->m_data;
        while (p < self->m_data + n) {
            M522_VEC::construct(p, x);
            p++;
            self->m_size++;
        }
    }
}
