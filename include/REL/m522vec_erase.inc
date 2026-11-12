void M522_FN(M522_VEC *self, M522_T *pos)
{
    M522_T *last = self->end();
    M522_T *dst;
    M522_T *src;

    if ((u32)((last - pos) - 1) != 0) {
        dst = pos;
        src = pos + 1;
        while (src < last) {
            *dst = *src;
            src++;
            dst++;
        }
    }
    self->m_size--;
    m522Destroy(self->m_data + self->m_size);
}
