void M522_FN(M522_VEC *self, M522_T *pos, u32 n, M522_T *xp)
{
    const M522_T &x = *xp;
    u32 max;
    M522_T *oldEnd;
    M522_T *p;
    M522_T *q;
    M522_T *newData;
    M522_T *oldData;
    M522_T *dst;
    const M522_T *px;
    s32 after;
    u32 newSize;
    u32 newCap;
    u32 k;
    M522_VEC tmp;

    if (n != 0) {
        max = M522_VEC::maxSize();
        if (n > max || self->m_size > max - n) {
            fn_1_6AD0(M522_STDERR, "vector::insert length error\n");
            abort();
        }
        if (self->m_size + n <= self->capacity()) {
            oldEnd = self->end();
            after = oldEnd - pos;
            px = &x;
            if (n > (u32)after) {
                p = oldEnd;
                while (n > (u32)after) {
                    M522_VEC::construct(p, x);
                    p++;
                    n--;
                    self->m_size++;
                }
                q = pos;
                while (q < oldEnd) {
                    M522_VEC::construct(p, *q);
                    q++;
                    p++;
                    self->m_size++;
                }
            } else {
                p = oldEnd;
                q = oldEnd - n;
                while (q < oldEnd) {
                    M522_VEC::construct(p, *q);
                    q++;
                    p++;
                    self->m_size++;
                }
                if (oldEnd - (after - n) <= px && px < oldEnd) {
                    px += n;
                }
                p = oldEnd;
                q = pos + (after - n);
                while (q > pos) {
                    q--;
                    p--;
                    *p = *q;
                }
            }
            k = n;
            dst = pos;
            while (k != 0) {
                *dst = *px;
                dst++;
                k--;
            }
        } else {
            newSize = self->m_size + n;
            newCap = self->m_cap != 0 ? self->m_cap : 1;
            while (newSize > newCap) {
                if (newCap < (max >> 1)) {
                    newCap *= 2;
                } else {
                    newCap = max;
                }
            }
            tmp.m_cap = 0;
            tmp.m_size = 0;
            tmp.m_data = 0;
            tmp.m_data = tmp.allocate(newCap);
            tmp.m_cap = newCap;
            dst = tmp.m_data;
            oldData = self->begin();
            oldEnd = oldData + self->m_size;
            p = oldData;
            while (p < pos) {
                M522_VEC::construct(dst, *p);
                p++;
                dst++;
                tmp.m_size++;
            }
            while (n != 0) {
                M522_VEC::construct(dst, x);
                dst++;
                n--;
                tmp.m_size++;
            }
            while (p < oldEnd) {
                M522_VEC::construct(dst, *p);
                p++;
                dst++;
                tmp.m_size++;
            }
            if (&tmp != self) {
                u32 t0 = tmp.m_cap;
                tmp.m_cap = self->m_cap;
                self->m_cap = t0;
                M522_T *t1 = tmp.m_data;
                tmp.m_data = self->m_data;
                self->m_data = t1;
                u32 t2 = tmp.m_size;
                tmp.m_size = self->m_size;
                self->m_size = t2;
            }
            M522_TMP_DTOR(tmp);
        }
    }
}
