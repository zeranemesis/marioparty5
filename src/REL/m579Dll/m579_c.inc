/* m579 library part c: fade/exit helpers, CModel wrappers (a model is an s16 handle) and the CMT19937 RNG. */

extern "C" {


/* Mersenne twister state, object layout of CMT19937 */

/* .rodata owned by this part (the others come from part a) */
extern const u32 lbl_1_rodata_50[2] = { 0, 0x9908B0DF };
extern const f32 lbl_1_rodata_58 = 2.3283064e-10f;
extern const f32 lbl_1_rodata_5C = 0.5f;
extern const f64 lbl_1_rodata_60 = 1.1102230246251565e-16;
extern const f64 lbl_1_rodata_68 = 67108864.0;
extern const f32 lbl_1_rodata_70 = 1.1920929e-7f;

/* .data owned by this part */
char lbl_1_data_A2[10] = "model.cpp";
char lbl_1_data_AC[10] = "p != NULL";

void fn_1_2BEC(void)
{
    WipeCreate(1, 5, 60);
}

void fn_1_2C18(void)
{
    WipeCreate(2, 6, 60);
}

void fn_1_2C44(void)
{
    omSysExitReq = 1;
}

s16 *fn_1_2C58(s16 *self, void *data)
{
    *self = Hu3DModelCreate(data);
    return self;
}

s16 *fn_1_2C94(s16 *self, s32 dataNum)
{
    void *data = HuDataSelHeapReadNum(dataNum, 0x10000000, HEAP_MODEL);
    *self = Hu3DModelCreate(data);
    return self;
}

s16 *fn_1_2CDC(s16 *self, HU3DMODELHOOK hook)
{
    *self = Hu3DHookFuncCreate(hook);
    return self;
}

s16 *fn_1_2D18(s16 *self, s32 charNo, s32 mode)
{
    s16 type = 0;
    switch (mode) {
    case 0:
        type = 1;
        break;
    case 1:
        type = 2;
        break;
    case 2:
        type = 4;
        break;
    case 3:
        type = 8;
        break;
    }
    *self = CharModelCreate((s16) charNo, type);
    return self;
}

s16 *fn_1_2DB4(s16 *self, s16 *src)
{
    s16 srcId = *src;
    *self = Hu3DModelLink(srcId);
    return self;
}

void fn_1_2E08(s16 *self, s32 attr)
{
    Hu3DModelAttrSet(*self, attr);
}

void fn_1_2E40(s16 *self, s32 attr)
{
    Hu3DModelAttrReset(*self, attr);
}

void fn_1_2E78(s16 *self, omObj_s *obj)
{
    s16 id = *self;
    Hu3DModelPosSet(id, obj->trans.x, obj->trans.y, obj->trans.z);
    Hu3DModelRotSet(id, obj->rot.x, obj->rot.y, obj->rot.z);
    Hu3DModelScaleSet(id, obj->scale.x, obj->scale.y, obj->scale.z);
}

void fn_1_2EF4(s16 *self, Vec *pos)
{
    Hu3DModelPosSet(*self, pos->x, pos->y, pos->z);
}

void fn_1_2F3C(s16 *self, Vec *rot)
{
    Hu3DModelRotSet(*self, rot->x, rot->y, rot->z);
}

void fn_1_2F84(s16 *self, Vec *scale)
{
    Hu3DModelScaleSet(*self, scale->x, scale->y, scale->z);
}

void fn_1_2FCC(s16 *self, f32 scale)
{
    Hu3DModelScaleSet(*self, scale, scale, scale);
}

void fn_1_3014(s16 *self, f32 lvl)
{
    Hu3DModelTPLvlSet(*self, lvl);
}

void fn_1_304C(s16 *self, s16 layer)
{
    Hu3DModelLayerSet(*self, layer);
}

void fn_1_3084(s16 *self, u16 camBit)
{
    Hu3DModelCameraSet(*self, camBit);
}

void fn_1_30BC(s16 *self, void *data)
{
    Hu3DData[*self].hookData = data;
}

void *fn_1_30D8(s16 *self)
{
    return (void *) Hu3DData[*self].mtx;
}

/* destructor: kills the model, frees the object when flag > 0 */
void fn_1_30F4(s16 *self, s32 flag)
{
    if (self != 0) {
        Hu3DModelKill(*self);
        if (flag > 0) {
            HuMemDirectFree(self);
        }
    }
}

HSFOBJECT *fn_1_3150(s16 *self, const char *name)
{
    HSFOBJECT *obj = Hu3DModelObjPtrGet(*self, (char *) name);
    if (obj != 0) {
    } else {
        fn_1_1D4(lbl_1_data_AC, lbl_1_data_A2, 0x8A);
    }
    return obj;
}

void fn_1_31E4(s16 *self)
{
    Hu3DModelShadowSet(*self);
}

void fn_1_3214(s16 *self)
{
    Hu3DModelShadowMapSet(*self);
}

void fn_1_3244(s16 *self, const char *name)
{
    Hu3DModelShadowMapObjSet(*self, (char *) name);
}

void fn_1_327C(s16 *self)
{
    Hu3DModelShadowMapObjReset(*self);
}

void fn_1_32AC(s16 *self, const char *name)
{
    HSFOBJECT *obj = fn_1_3150(self, name);
    ((HSFCONSTDATA *) obj->constData)->attr &= ~8;
}

void fn_1_3358(s16 *self, const char *name)
{
    HSFOBJECT *obj = fn_1_3150(self, name);
    obj->flags |= 0x400;
}

void fn_1_3400(s16 *self, const char *name)
{
    HSFOBJECT *obj = fn_1_3150(self, name);
    obj->flags &= ~0x400;
}

void fn_1_34A8(s16 *self, const char *name)
{
    HSFOBJECT *obj = fn_1_3150(self, name);
    obj->flags |= 0x400;
    HSFTRANSFORM *base = &obj->mesh.base;
    base->scale.x = 0.0f;
    base->scale.y = 0.0f;
    base->scale.z = 0.0f;
}

void fn_1_3584(s16 *self, const char *name)
{
    HSFOBJECT *obj = fn_1_3150(self, name);
    obj->flags &= ~0x400;
    HSFTRANSFORM *base = &obj->mesh.base;
    base->scale.x = 1.0f;
    base->scale.y = 1.0f;
    base->scale.z = 1.0f;
}

void fn_1_3660(s16 *self, s32 time)
{
    Hu3DMotionTimeSet(*self, (f32) time);
}

void fn_1_36CC(M579Mt *self, s32 seed)
{
    self->state[0] = seed;
    for (self->index = 1; self->index < 624; self->index++) {
        self->state[self->index] = self->index + (0x6C078965 * (self->state[self->index - 1] ^ (self->state[self->index - 1] >> 30)));
    }
}

void fn_1_3740(M579Mt *self, u32 *key, u32 keyLen)
{
    u32 i;
    u32 j;
    u32 k;

    self->state[0] = 0x012BD6AA;
    for (self->index = 1; self->index < 624; self->index++) {
        self->state[self->index] = self->index + (0x6C078965 * (self->state[self->index - 1] ^ (self->state[self->index - 1] >> 30)));
    }
    i = 1;
    j = 0;
    k = keyLen < 624 ? 624 : keyLen;
    for (; k != 0; k--) {
        self->state[i] = key[j] + ((self->state[i] ^ ((self->state[i - 1] ^ (self->state[i - 1] >> 30)) * 0x19660D)) + j);
        i++;
        j++;
        if (i >= 624) {
            self->state[0] = self->state[623];
            i = 1;
        }
        if (j >= keyLen) {
            j = 0;
        }
    }
    for (k = 623; k != 0; k--) {
        self->state[i] = (self->state[i] ^ ((self->state[i - 1] ^ (self->state[i - 1] >> 30)) * 0x5D588B65)) - i;
        i++;
        if (i >= 624) {
            self->state[0] = self->state[623];
            i = 1;
        }
    }
    self->state[0] = 0x80000000;
}

/* tempered 32-bit output (the inlined CMT19937::gen) */
static u32 M579MtGen(M579Mt *self)
{
    u32 y;

    if (self->index >= 624) {
        u32 kk = 0;
        for (; kk < 227; kk++) {
            y = (self->state[kk] & 0x80000000) | (self->state[kk + 1] & 0x7FFFFFFF);
            self->state[kk] = lbl_1_rodata_50[y & 1] ^ ((y >> 1) ^ self->state[kk + 397]);
        }
        for (; kk < 623; kk++) {
            y = (self->state[kk] & 0x80000000) | (self->state[kk + 1] & 0x7FFFFFFF);
            self->state[kk] = lbl_1_rodata_50[y & 1] ^ ((y >> 1) ^ self->state[kk - 227]);
        }
        y = (self->state[623] & 0x80000000) | (self->state[0] & 0x7FFFFFFF);
        self->state[623] = (y >> 1) ^ self->state[396] ^ lbl_1_rodata_50[y & 1];
        self->index = 0;
    }
    y = self->state[self->index++];
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9D2C5680;
    y ^= (y << 15) & 0xEFC60000;
    return y ^ (y >> 18);
}

u32 fn_1_3904(M579Mt *self)
{
    return M579MtGen(self);
}

u32 fn_1_3A90(M579Mt *self)
{
    return M579MtGen(self) >> 1;
}

f32 fn_1_3C28(M579Mt *self)
{
    return lbl_1_rodata_58 * (f32) M579MtGen(self);
}

f32 fn_1_3DEC(M579Mt *self)
{
    return lbl_1_rodata_58 * (f32) M579MtGen(self);
}

f32 fn_1_3FB0(M579Mt *self)
{
    return lbl_1_rodata_58 * (lbl_1_rodata_5C + (f32) M579MtGen(self));
}

f64 fn_1_4184(M579Mt *self)
{
    u32 a = M579MtGen(self);
    u32 b = M579MtGen(self);
    return lbl_1_rodata_60 * ((lbl_1_rodata_68 * (f64) (a >> 5)) + (f64) (b >> 6));
}

f32 fn_1_44FC(M579Mt *self)
{
    return lbl_1_rodata_70 * (f32) (M579MtGen(self) >> 9);
}

} /* extern "C" */
