void M522_FN(M522_VEC *self, M522_T *pos, u32 n, M522_T *xp)
{
    const M522_T *px = xp;
    u32 max;
    M522_T *end;
    M522_T *src;
    M522_T *dst;
    M522_T *oldData;
    M522_T *copyFrom;
    M522_T *copyEnd;
    s32 after;
    s32 index;
    u32 newSize;
    u32 newCap;
    u32 k;

    if (n != 0) {
        max = M522_VEC::maxSize();
        if (n > max || self->m_size > max - n) {
            fn_1_6AD0(M522_STDERR, "vector::insert length error\n");
            abort();
        }
        if (self->m_size + n <= self->capacity()) {
            end = self->end();
            after = end - pos;
            if (end - (after - n) <= px && px < end) {
                px += n;
            }
            if ((u32)after != 0) {
                src = end;
                dst = end + n;
                while (src > pos) {
                    src--;
                    dst--;
                    *dst = *src;
                }
            }
            k = n;
            dst = pos;
            while (k != 0) {
                *dst = *px;
                dst++;
                k--;
            }
            self->m_size += n;
        } else {
            index = pos - self->begin();
            oldData = self->begin();
            newSize = self->m_size + n;
            newCap = self->m_cap != 0 ? self->m_cap : 1;
            while (newSize > newCap) {
                if (newCap < (max >> 1)) {
                    newCap *= 2;
                } else {
                    newCap = max;
                }
            }
            self->m_data = self->allocate(newCap);
            k = n;
            dst = self->begin() + index;
            while (k != 0) {
                *dst = *px;
                dst++;
                k--;
            }
            if (oldData != 0) {
                dst = self->begin();
                copyFrom = oldData;
                while (copyFrom < oldData + index) {
                    *dst = *copyFrom;
                    copyFrom++;
                    dst++;
                }
                dst = self->begin() + index + n;
                copyEnd = oldData + self->m_size;
                copyFrom = oldData + index;
                while (copyFrom < copyEnd) {
                    *dst = *copyFrom;
                    copyFrom++;
                    dst++;
                }
                fn_1_6964(oldData);
            }
            self->m_size = newSize;
            self->m_cap = newCap;
        }
    }
}
