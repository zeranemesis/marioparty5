void M522_FN(M522_VEC *self, const M522_T *first, const M522_T *last, u8 *tag)
{
    u32 n = last - first;
    M522_T *p;

    if (self->m_size > M522_VEC::maxSize()) {
        fn_1_6AD0(M522_STDERR, "vector length error\n");
        abort();
    }
    if (n != 0) {
        self->m_data = self->allocate(n);
        self->m_cap = n;
        p = self->m_data;
        while (first != last) {
            M522_VEC::construct(p, *first);
            p++;
            first++;
            self->m_size++;
        }
    }
}
