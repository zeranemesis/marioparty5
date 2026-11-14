/* Second half of player.cpp (included at its end): the MSL deque<unsigned char> the beat
 * recorder uses (deque = blocks of bytes, cdeque = circular buffer of block pointers),
 * vector<u16>::reserve, the single precision wrap-range helpers and the static init. */

#ifndef M537_STDERR
#ifdef TARGET_PC
#define M537_STDERR ((void *)stderr)
#else
extern "C" u8 __files[];
#define M537_STDERR ((void *)(__files + 0xA0))
#endif
#endif

#define M537Q_ALLOC_FAIL()                                  \
    do {                                                    \
        fn_1_234(M537_STDERR, "Memory allocation failure"); \
        abort();                                            \
    } while (0)

/* map iterator (3 words) and deque iterator (6 words) */
struct M537QMapIt {
    M537_DEQUEMAP *map;
    u32 idx;
    u8 **slot;
};

struct M537QDequeIt {
    M537_DEQUEMAP *map;
    u32 idx;
    u8 **slot;
    u8 *cur;
    u8 *end;
    u32 pos;
};

extern "C" {

void abort(void);

u32 lbl_1_data_598 = 1; /* cdeque::push_back minimum capacity */
u32 lbl_1_data_5DC = 1; /* cdeque::insert minimum capacity */
M537_RODATA const u32 lbl_1_rodata_520 = 0x200; /* default block size */

s32 lbl_1_bss_A28;
s32 lbl_1_bss_A2C;
s32 lbl_1_bss_A30;
#ifndef TARGET_PC
u8 lbl_1_bss_A34[12];
#endif
u8 lbl_1_bss_A4C;

void fn_1_E004(M537_DEQUE *self, M537QDequeIt *first, M537QDequeIt *last);
void fn_1_DD6C(M537QDequeIt *out, const M537_DEQUE *self, u32 idx);
void fn_1_DEB8(M537QDequeIt *out, M537_DEQUE *self, u32 idx);
u8 fn_1_D6D4(M537_DEQUE *self, s32 front, M537_DEQUEMAP *spare, u32 *count, s32 n);
void fn_1_D224(M537_DEQUEMAP *self, u32 n);
void fn_1_D448(M537_DEQUEMAP *self, u32 n, u8 *val);
void fn_1_E538(M537_DEQUEMAP *self, M537QMapIt *first, M537QMapIt *last, const u8 *tag);
void fn_1_E8D0(M537_DEQUEMAP *self, M537_DEQUEMAP *other);
void fn_1_E944(M537QMapIt *out, M537_DEQUEMAP *self, M537QMapIt *first, M537QMapIt *last);
void fn_1_EE38(M537_DEQUEMAP *self, M537QMapIt *pos, u32 n, u8 *const *val);
}

/* ---- inline helpers of the template ---- */

static inline M537QMapIt m537qBegin(M537_DEQUEMAP *m)
{
    M537QMapIt it;
    it.map = m;
    it.idx = 0;
    it.slot = m->data + m->start;
    return it;
}

static inline M537QMapIt m537qEnd(M537_DEQUEMAP *m)
{
    M537QMapIt it;
    u32 t = m->start + m->count;
    if (t >= m->cap) {
        t -= m->cap;
    }
    it.map = m;
    it.idx = m->count;
    it.slot = m->data + t;
    return it;
}

static inline M537QMapIt m537qAt(M537_DEQUEMAP *m, u32 n)
{
    M537QMapIt it = m537qBegin(m);
    u32 t;
    it.idx += n;
    t = m->start + it.idx;
    if (t >= m->cap) {
        t -= m->cap;
    }
    it.slot = m->data + t;
    return it;
}

static inline void m537qNext(M537QMapIt *it)
{
    it->idx++;
    it->slot++;
    if (it->slot == it->map->data + it->map->cap) {
        it->slot = it->map->data;
    }
}

static inline void m537qPrev(M537QMapIt *it)
{
    it->idx--;
    if (it->slot == it->map->data) {
        it->slot = it->map->data + it->map->cap;
    }
    it->slot--;
}

/* copy [first, last) to dst */
static inline u8 **m537qCopy(M537QMapIt first, M537QMapIt last, u8 **dst)
{
    while (first.idx != last.idx) {
        *dst = *first.slot;
        m537qNext(&first);
        dst++;
    }
    return dst;
}

static inline void m537qFill(M537QMapIt it, u32 n, u8 *const *val)
{
    while (n != 0) {
        *it.slot = *val;
        m537qNext(&it);
        n--;
    }
}

static inline u32 m537qMaxLen(void)
{
    return 0x3FFFFFFF; /* min(0x3FFFFFFF, 0x7FFFFFFF) */
}

/* cdeque::reserve as the template inlines it */
static inline void m537qReserve(M537_DEQUEMAP *self, u32 n)
{
    u8 **nd;

    if (n > m537qMaxLen()) {
        fn_1_234(M537_STDERR, "cdeque::reserve length error\n");
        abort();
    }
    if (n > self->cap) {
        nd = (u8 **)fn_1_0(n * sizeof(u8 *));
        if (nd == 0) {
            M537Q_ALLOC_FAIL();
        }
        if (self->data != 0) {
            m537qCopy(m537qBegin(self), m537qEnd(self), nd);
            if (self->data != 0) {
                fn_1_C8(self->data);
            }
        }
        self->cap = n;
        self->data = nd;
        self->start = 0;
    }
}

/* step of the deque iterator, moving on to the next block when the current one ends */
static inline void m537qDqNext(M537QDequeIt *it)
{
    it->pos++;
    it->cur++;
    if (it->cur == it->end) {
        u32 n = it->end - *it->slot;
        it->idx++;
        it->slot++;
        if (it->slot == it->map->data + it->map->cap) {
            it->slot = it->map->data;
        }
        it->cur = *it->slot;
        it->end = it->cur + n;
    }
}

/* wraps x into [lo, hi) */
static inline f32 m537qWrap(M537_WRAP *c, f32 x)
{
    if (x < c->lo) {
        return c->hi - (f32)fmod(c->lo - x, c->range);
    }
    if (x >= c->hi) {
        return c->lo + (f32)fmod(x - c->hi, c->range);
    }
    return x;
}

extern "C" {

u8 **fn_1_C084(M537_DEQUEMAP *map, u32 idx)
{
    idx += map->start;
    if (idx >= map->cap) {
        idx -= map->cap;
    }
    return &map->data[idx];
}

/* list::for_each over the circular node list */
void fn_1_C0B0(M537_NODE *head, void (*fn)(M537Player *))
{
    fn_1_C110(head->next, head, fn);
}

void fn_1_C110(M537_NODE *first, M537_NODE *end, void (*fn)(M537Player *))
{
    while (first != end) {
        fn((M537Player *)first);
        first = first->next;
    }
}

/* ~deque body: the bytes are trivial, the blocks (all but the trailing empty slot) are freed */
void fn_1_C170(M537_DEQUE *self)
{
    M537QDequeIt last;
    M537QDequeIt first;
    M537QMapIt it;
    M537QMapIt stop;

    if (self->map.count != 0) {
        fn_1_DEB8(&last, self, self->offset + self->size);
        fn_1_DEB8(&first, self, self->offset);
        fn_1_E004(self, &first, &last);
        stop = m537qEnd(&self->map);
        stop.idx--;
        it = m537qBegin(&self->map);
        while (it.idx != stop.idx) {
            fn_1_C8(*it.slot);
            m537qNext(&it);
        }
    }
}

/* deque::pop_front */
void fn_1_C420(M537_DEQUE *self)
{
    u32 q;
    u32 n;
    u8 **slot;

    self->offset++;
    self->size--;
    if (self->size == 0) {
        n = self->map.count - 2;
        while (n != 0) {
            self->map.count--;
            q = self->map.count - 1 + self->map.start;
            if (q >= self->map.cap) {
                q -= self->map.cap;
            }
            slot = &self->map.data[q];
            fn_1_C8(*slot);
            *slot = 0;
            n--;
        }
        self->offset = self->chunk >> 1;
    }
}

/* deque::push_back */
void fn_1_C54C(M537_DEQUE *self, u8 *val)
{
    u32 added;
    M537_DEQUEMAP spare;
    u32 at;
    u32 q;
    u8 *p;

    if (self->size > (u32)-2) {
        fn_1_234(M537_STDERR, "deque::push_back length error\n");
        abort();
    }
    if (self->map.count == 0) {
        fn_1_D224(&self->map, 2);
        fn_1_D448(&self->map, 1, 0);
    }
    added = 0;
    spare.cap = 0;
    spare.count = 0;
    spare.start = 0;
    spare.data = 0;
    if (self->offset + self->size == self->chunk * (self->map.count - 1)) {
        fn_1_D6D4(self, 0, &spare, &added, 1);
    }
    at = self->offset + self->size;
    if (at < self->chunk) {
        p = self->map.data[self->map.start] + at;
    } else {
        q = at / self->chunk + self->map.start;
        if (q >= self->map.cap) {
            q -= self->map.cap;
        }
        p = self->map.data[q] + at % self->chunk;
    }
    if (p != 0) {
        *p = *val;
    }
    self->size++;
    if (spare.data != 0) {
        fn_1_C8(spare.data);
    }
    while (self->offset >= (self->chunk * 3) >> 1) {
        fn_1_C8(self->map.data[self->map.start]);
        fn_1_CEEC(&self->map);
        self->offset -= self->chunk;
    }
}

/* ~cdeque (this one tests self twice) */
void *fn_1_C7C4(M537_DEQUEMAP *self, s16 freeIt)
{
    if (self != 0) {
        if (self != 0 && self->data != 0) {
            fn_1_C8(self->data);
        }
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

/* ~cdeque */
void *fn_1_C838(M537_DEQUEMAP *self, s16 freeIt)
{
    if (self != 0) {
        if (self->data != 0) {
            fn_1_C8(self->data);
        }
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

/* deque copy constructor */
M537_DEQUE *fn_1_C8A4(M537_DEQUE *self, M537_DEQUE *src)
{
    u8 *blk;
    u8 *nul;
    u8 *dst;
    M537QDequeIt it;
    M537QDequeIt end;

    self->chunk = lbl_1_rodata_520;
    self->map.cap = 0;
    self->map.count = 0;
    self->map.start = 0;
    self->map.data = 0;
    self->offset = 0;
    self->size = src->size;
    if (self->size != 0) {
        fn_1_D224(&self->map, 2);
        fn_1_D448(&self->map, 1, 0);
        if (self->chunk < self->size) {
            self->chunk = self->size;
        }
        blk = (u8 *)fn_1_0(self->chunk * sizeof(u8));
        if (blk == 0) {
            M537Q_ALLOC_FAIL();
        }
        self->map.data[self->map.start] = blk;
        nul = 0;
        fn_1_CF28(&self->map, &nul);
        self->offset = (self->chunk - self->size) >> 1;
        dst = blk + self->offset;
        fn_1_DD6C(&end, src, src->offset + src->size);
        fn_1_DD6C(&it, src, src->offset);
        while (it.cur != end.cur) {
            if (dst != 0) {
                *dst = *it.cur;
            }
            dst++;
            m537qDqNext(&it);
        }
    }
    return self;
}

/* default deque constructor */
void fn_1_CBCC(M537_DEQUE *self, void *unused)
{
    self->chunk = lbl_1_rodata_520;
    self->map.cap = 0;
    self->map.count = 0;
    self->map.start = 0;
    self->map.data = 0;
    self->offset = 0;
    self->size = 0;
}

/* vector<u16>::reserve */
void fn_1_CC24(M537_VECS16 *self, u32 n)
{
    s16 *old;
    s16 *nd;

    if (n > self->cap) {
        if (n > 0x7FFFFFFF) {
            fn_1_234(M537_STDERR, "vector::reserve length error\n");
            abort();
        }
        old = self->data;
        nd = (s16 *)fn_1_0(n * sizeof(s16));
        if (nd == 0) {
            M537Q_ALLOC_FAIL();
        }
        self->data = nd;
        if (old != 0) {
            memcpy(self->data, old, self->size * sizeof(s16));
            fn_1_C8(old);
        }
        self->cap = n;
    }
}

M537_WRAP *fn_1_CD4C(M537_WRAP *self, f32 a, f32 b, f32 x)
{
    self->lo = (a < b) ? a : b;
    self->hi = (a > b) ? a : b;
    self->range = self->hi - self->lo;
    self->value = m537qWrap(self, x);
    return self;
}

f32 fn_1_CEB0(M537_WRAP *self, f32 to)
{
    return fn_1_E320(self, self->value, to);
}

/* cdeque::pop_front */
void fn_1_CEEC(M537_DEQUEMAP *self)
{
    self->count--;
    self->start++;
    if (self->start >= self->cap) {
        self->start -= self->cap;
    }
}

/* cdeque::push_back */
void fn_1_CF28(M537_DEQUEMAP *self, u8 *const *val)
{
    const u32 *keep;
    u32 newCap;
    u32 t;
    u8 **nd;

    if (self->count == fn_1_FE0C(self)) {
        fn_1_234(M537_STDERR, "cdeque::push_back length error\n");
        abort();
    }
    if (self->count < self->cap) {
        t = self->count + self->start;
        if (t >= self->cap) {
            t -= self->cap;
        }
        self->data[t] = *val;
        self->count++;
        return;
    }
    if (self->cap < lbl_1_data_598) {
        keep = &lbl_1_data_598;
    } else {
        keep = &self->cap;
    }
    newCap = *keep;
    if (self->count + 1 > newCap) {
        newCap *= 2;
    }
    nd = (u8 **)fn_1_0(newCap * sizeof(u8 *));
    if (nd == 0) {
        M537Q_ALLOC_FAIL();
    }
    nd[self->count] = *val;
    if (self->count != 0) {
        m537qCopy(m537qBegin(self), m537qEnd(self), nd);
    }
    if (self->data != 0) {
        fn_1_C8(self->data);
    }
    self->data = nd;
    self->count++;
    self->cap = newCap;
    self->start = 0;
}

/* cdeque::reserve */
void fn_1_D224(M537_DEQUEMAP *self, u32 n)
{
    m537qReserve(self, n);
}

/* cdeque::resize */
void fn_1_D448(M537_DEQUEMAP *self, u32 n, u8 *val)
{
    M537QMapIt pos;
    M537QMapIt first;
    M537QMapIt last;
    M537QMapIt res;

    if (n > self->count) {
        pos = m537qEnd(self);
        fn_1_EE38(self, &pos, n - self->count, &val);
    } else if (n < self->count) {
        first = m537qAt(self, n);
        last = m537qEnd(self);
        fn_1_E944(&res, self, &first, &last);
    }
}

/* deque: append blocks at the back of the map until n more bytes fit */
u8 fn_1_D6D4(M537_DEQUE *self, s32 front, M537_DEQUEMAP *spare, u32 *count, s32 n)
{
    u8 *blk;
    u8 *nul;
    u32 t;
    u32 left;
    u32 cap2;
    u8 grew;
    u8 tag;
    M537QMapIt first;
    M537QMapIt last;

    grew = 0;
    left = n - front;
    do {
        blk = (u8 *)fn_1_0(self->chunk * sizeof(u8));
        if (blk == 0) {
            M537Q_ALLOC_FAIL();
        }
        if (grew == 0 && self->map.count == self->map.cap) {
            fn_1_E8D0(&self->map, spare);
            grew = 1;
            cap2 = spare->cap * 2;
            m537qReserve(&self->map, cap2);
            tag = lbl_1_bss_A4C;
            first = m537qBegin(spare);
            last = m537qEnd(spare);
            fn_1_E538(&self->map, &first, &last, &tag);
        }
        t = self->map.count - 1 + self->map.start;
        if (t >= self->map.cap) {
            t -= self->map.cap;
        }
        self->map.data[t] = blk;
        nul = 0;
        fn_1_DD3C(&self->map, &nul);
        *count = *count + 1;
        if (left > self->chunk) {
            left -= self->chunk;
            continue;
        }
        break;
    } while (1);
    return grew;
}

void fn_1_DD3C(M537_DEQUEMAP *self, u8 *const *val)
{
    fn_1_CF28(self, val);
}

/* deque iterator at index idx (const / mutable) */
void fn_1_DD6C(M537QDequeIt *out, const M537_DEQUE *self, u32 idx)
{
    M537QMapIt it;
    u8 *base;
    u8 *cur;
    u32 t;

    it = m537qBegin((M537_DEQUEMAP *)&self->map);
    if (idx < self->chunk) {
        base = (self->map.count != 0) ? *it.slot : 0;
        cur = base + idx;
    } else {
        it.idx += idx / self->chunk;
        t = it.map->start + it.idx;
        if (t >= it.map->cap) {
            t -= it.map->cap;
        }
        it.slot = it.map->data + t;
        base = *it.slot;
        cur = base + idx % self->chunk;
    }
    out->map = it.map;
    out->idx = it.idx;
    out->slot = it.slot;
    out->cur = cur;
    out->end = base + self->chunk;
    out->pos = idx;
}

void fn_1_DEB8(M537QDequeIt *out, M537_DEQUE *self, u32 idx)
{
    fn_1_DD6C(out, self, idx);
}

/* destroy [first, last): the bytes are trivial, nothing is left to do but the walk */
void fn_1_E004(M537_DEQUE *self, M537QDequeIt *first, M537QDequeIt *last)
{
}

/* single precision shortest signed distance on the circle, from -> to */
f32 fn_1_E320(M537_WRAP *self, f32 from, f32 to)
{
    f32 a = m537qWrap(self, to);
    f32 b = m537qWrap(self, from);
    f32 d = a - b;
    f32 ad = (d < 0.0f) ? -d : d;
    s32 sign;

    if (ad < self->range * 0.5f) {
        return d;
    }
    sign = (d < 0.0f) ? 1 : -1;
    return (self->range - ad) * (f32)sign;
}

/* cdeque::assign(first, last) */
void fn_1_E538(M537_DEQUEMAP *self, M537QMapIt *first, M537QMapIt *last, const u8 *tag)
{
    u32 n;
    u8 **old;

    n = last->idx - first->idx;
    if (n > m537qMaxLen()) {
        fn_1_234(M537_STDERR, "cdeque::assign length error\n");
        abort();
    }
    if (n <= self->cap) {
        m537qCopy(*first, *last, self->data);
        self->start = 0;
        self->count = n;
        return;
    }
    old = self->data;
    self->data = (u8 **)fn_1_0(n * sizeof(u8 *));
    if (self->data == 0) {
        M537Q_ALLOC_FAIL();
    }
    m537qCopy(*first, *last, self->data);
    if (old != 0) {
        fn_1_C8(old);
    }
    self->cap = n;
    self->count = n;
    self->start = 0;
}

/* cdeque::swap */
void fn_1_E8D0(M537_DEQUEMAP *self, M537_DEQUEMAP *other)
{
    u32 t;
    u8 **d;

    if (self != other) {
        t = self->cap;
        self->cap = other->cap;
        other->cap = t;
        t = self->count;
        self->count = other->count;
        other->count = t;
        t = self->start;
        self->start = other->start;
        other->start = t;
        d = self->data;
        self->data = other->data;
        other->data = d;
    }
}

/* cdeque::erase(first, last) */
void fn_1_E944(M537QMapIt *out, M537_DEQUEMAP *self, M537QMapIt *first, M537QMapIt *last)
{
    M537QMapIt d;
    M537QMapIt s;
    M537QMapIt stop;
    u32 before;
    u32 after;
    u32 n;

    if (first->idx == last->idx) {
        *out = *first;
        return;
    }
    before = first->idx - m537qBegin(self).idx;
    after = m537qEnd(self).idx - last->idx;
    n = last->idx - first->idx;
    if (before < after) {
        if (before != 0) {
            d = *last;
            s = *first;
            stop = m537qBegin(self);
            while (s.idx != stop.idx) {
                m537qPrev(&d);
                m537qPrev(&s);
                *d.slot = *s.slot;
            }
        }
        self->start += n;
        if (self->start >= self->cap) {
            self->start -= self->cap;
        }
    } else if (after != 0) {
        d = *first;
        stop = m537qEnd(self);
        s = *last;
        while (s.idx != stop.idx) {
            *d.slot = *s.slot;
            m537qNext(&s);
            m537qNext(&d);
        }
    }
    self->count -= n;
    *out = *first;
}

/* cdeque::insert(pos, n, val) */
void fn_1_EE38(M537_DEQUEMAP *self, M537QMapIt *pos, u32 n, u8 *const *val)
{
    u32 before;
    u32 after;
    u32 newSize;
    u32 newCap;
    u32 i;
    const u32 *keep;
    M537QMapIt d;
    M537QMapIt s;
    M537QMapIt se;
    u8 **nd;
    u8 **p;

    if (n == 0) {
        return;
    }
    if (n > m537qMaxLen() || self->count > m537qMaxLen() - n) {
        fn_1_234(M537_STDERR, "cdeque::insert length error\n");
        abort();
    }
    if (self->count + n <= self->cap) {
        before = pos->idx - m537qBegin(self).idx;
        after = m537qEnd(self).idx - pos->idx;
        if (before < after) {
            if (self->start >= n) {
                self->start -= n;
            } else {
                self->start = self->cap - (n - self->start);
            }
            if (before == 0) {
                m537qFill(m537qBegin(self), n, val);
            } else {
                /* move the first "before" slots n slots down, then fill the gap */
                d = m537qBegin(self);
                s = m537qAt(self, n);
                se = m537qAt(self, n + before);
                while (s.idx != se.idx) {
                    *d.slot = *s.slot;
                    m537qNext(&s);
                    m537qNext(&d);
                }
                m537qFill(m537qAt(self, before), n, val);
            }
        } else if (after == 0) {
            m537qFill(m537qEnd(self), n, val);
        } else {
            /* move the tail n slots up, then fill the gap */
            d = m537qAt(self, self->count + n);
            s = m537qEnd(self);
            while (s.idx != pos->idx) {
                m537qPrev(&d);
                m537qPrev(&s);
                *d.slot = *s.slot;
            }
            m537qFill(*pos, n, val);
        }
        self->count += n;
        return;
    }
    newSize = self->count + n;
    if (self->cap < lbl_1_data_5DC) {
        keep = &lbl_1_data_5DC;
    } else {
        keep = &self->cap;
    }
    newCap = *keep;
    while (newSize > newCap) {
        newCap *= 2;
    }
    before = pos->idx - m537qBegin(self).idx;
    nd = (u8 **)fn_1_0(newCap * sizeof(u8 *));
    if (nd == 0) {
        M537Q_ALLOC_FAIL();
    }
    if (before != 0) {
        m537qCopy(m537qBegin(self), *pos, nd);
    }
    p = nd + before;
    for (i = n; i != 0; i--) {
        *p = *val;
        p++;
    }
    if (before < self->count) {
        m537qCopy(*pos, m537qEnd(self), nd + (before + n));
    }
    if (self->data != 0) {
        fn_1_C8(self->data);
    }
    self->data = nd;
    self->count = newSize;
    self->cap = newCap;
    self->start = 0;
}

/* cdeque::max_size */
u32 fn_1_FE0C(M537_DEQUEMAP *self)
{
    return m537qMaxLen();
}

/* static initialiser: default speeds and the work list head */
void fn_1_FE6C(void)
{
    lbl_1_bss_A28 = 0x78;
    lbl_1_bss_A2C = 0xD2;
    lbl_1_bss_A30 = 0xD2;
    fn_1_1264(&lbl_1_bss_A40);
#ifndef TARGET_PC
    __register_global_object(&lbl_1_bss_A40, (void *)fn_1_FEF4, lbl_1_bss_A34);
#endif
}

void *fn_1_FEF4(void *self, s16 freeIt)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

}

#ifdef TARGET_PC
// The GC build links fn_1_FE6C through .ctors; PC runs it at load time.
static struct M537PlayerBInit {
    M537PlayerBInit() { fn_1_FE6C(); }
} lbl_port_playerBInit;
#else
__declspec(section ".ctors") static void (*const lbl_ctor_FE6C)(void) = fn_1_FE6C;
#endif
