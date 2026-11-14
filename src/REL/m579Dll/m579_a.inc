/* m579 part A: operator new/delete, module entry points, assert/fprintf, camera pose maths, pad helpers,
 * debug free-fly camera (original .text 0x0..0x1264). */
#include <stdio.h>

#ifdef __MWERKS__
#define M579A_DATA __declspec(section ".data")
#else
#define M579A_DATA
#endif

extern "C" {

void OSPanic(const char *file, int line, const char *msg, ...);
void ObjectSetup(void);
extern void (*_ctors[])(void);
extern void (*_dtors[])(void);

M579A_DATA char lbl_1_data_0[12] = "ptr != NULL";
M579A_DATA char lbl_1_data_C[8] = "new.cpp";
M579A_DATA char lbl_1_data_14[30] = "\nASSERTION FAILED at %s:%d\n%s";
M579A_DATA char lbl_1_data_32[11] = "assert.cpp";
M579A_DATA char lbl_1_data_3D[57] = "{{0x%08x,0x%08x,0x%08x},{0x%08x,0x%08x,0x%08x,0x%08x}},\n";

void fn_1_1D4(const char *expr, const char *file, s32 line);
void fn_1_408(M579Cam *self, Vec *right, Vec *up, Vec *fwd);
void fn_1_56C(M579Cam *self, Vec *delta);
f32 fn_1_AB8(s32 pad);
f32 fn_1_B0C(s32 pad);
f32 fn_1_B60(s32 pad);
f32 fn_1_BB4(s32 pad);

/* operator new */
void *fn_1_0(s32 size)
{
    void *p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);

    if (p != 0) {
    } else {
        fn_1_1D4(lbl_1_data_0, lbl_1_data_C, 0x17);
    }
    return p;
}

/* operator new[] */
void *fn_1_64(s32 size)
{
    void *p = HuMemDirectMallocNum(HEAP_HEAP, size, 0x10000000);

    if (p != 0) {
    } else {
        fn_1_1D4(lbl_1_data_0, lbl_1_data_C, 0x17);
    }
    return p;
}

/* operator delete */
void fn_1_C8(void *ptr)
{
    if (ptr != 0) {
        HuMemDirectFree(ptr);
    }
}

/* operator delete[] */
void fn_1_100(void *ptr)
{
    if (ptr != 0) {
        HuMemDirectFree(ptr);
    }
}

void _prolog(void)
{
    void (**ctor)(void) = _ctors;

    while (*ctor != 0) {
        (*ctor)();
        ctor++;
    }
    ObjectSetup();
}

void _epilog(void)
{
    void (**dtor)(void) = _dtors;

    while (*dtor != 0) {
        (*dtor)();
        dtor++;
    }
}

/* __assert_fail */
void fn_1_1D4(const char *expr, const char *file, s32 line)
{
    char buf[0x40C];

    sprintf(buf, lbl_1_data_14, file, line, expr);
    OSPanic(lbl_1_data_32, 0x12, buf);
}

/* fprintf: the stream is ignored, everything goes to vprintf */
int fn_1_234(void *unused, const char *fmt, ...)
{
    va_list ap;
    int ret;

    va_start(ap, fmt);
    ret = vprintf(fmt, ap);
    va_end(ap);
    return ret;
}

/* CCamera::rotateCamera */
void fn_1_2D8(M579Cam *self, f32 pitch, f32 yaw)
{
    Vec ax, ay, az;
    Mtx rotA, rotB, rot;

    fn_1_408(self, &ax, &ay, &az);
    PSMTXRotAxisRad(rotA, &ax, 0.017453292f * pitch);
    PSMTXRotRad(rotB, 'Y', 0.017453292f * yaw);
    PSMTXConcat(rotA, rotB, rot);
    PSMTXMultVecSR(rot, &az, &az);
    PSMTXMultVecSR(rot, &ay, &self->up);
    PSVECSubtract(&self->pos, &az, &self->target);
}

/* CCamera::rotateCameraUp */
void fn_1_39C(M579Cam *self, f32 angle)
{
    Mtx m;
    Vec axis;

    PSVECSubtract(&self->target, &self->pos, &axis);
    PSMTXRotAxisRad(m, &axis, 0.017453292f * angle);
    PSMTXMultVec(m, &self->up, &self->up);
}

/* CCamera::getLookAtVec */
void fn_1_408(M579Cam *self, Vec *x, Vec *y, Vec *z)
{
    Mtx m;

    C_MTXLookAt(m, &self->pos, &self->up, &self->target);
    x->x = m[0][0];
    x->y = m[0][1];
    x->z = m[0][2];
    y->x = m[1][0];
    y->y = m[1][1];
    y->z = m[1][2];
    z->x = m[2][0];
    z->y = m[2][1];
    z->z = m[2][2];
}

/* CCamera::getLookAtMtxInverse */
void fn_1_4B0(M579Cam *self, Mtx out)
{
    C_MTXLookAt(out, &self->pos, &self->up, &self->target);
    PSMTXInverse(out, out);
}

/* CCamera::applyOffsetLocal */
void fn_1_504(M579Cam *self, Vec *local)
{
    Mtx m;
    Vec world;

    C_MTXLookAt(m, &self->pos, &self->up, &self->target);
    PSMTXInverse(m, m);
    PSMTXMultVecSR(m, local, &world);
    fn_1_56C(self, &world);
}

/* CCamera::applyOffset */
void fn_1_56C(M579Cam *self, Vec *delta)
{
    PSVECAdd(delta, &self->target, &self->target);
    PSVECAdd(delta, &self->pos, &self->pos);
}

/* CCamera::resetLookAt */
void fn_1_5C0(M579Cam *self)
{
    self->up.x = 0.0f;
    self->up.y = 1.0f;
    self->up.z = 0.0f;
    self->pos.x = 0.0f;
    self->pos.y = 0.0f;
    self->pos.z = 5000.0f;
    self->target.x = 0.0f;
    self->target.y = 0.0f;
    self->target.z = 0.0f;
}

/* CCamera::setView */
void fn_1_654(M579Cam *self, const M579Pose *pose)
{
    Mtx m;

    PSMTXQuat(m, (Quaternion *)pose->rot);
    self->up.x = m[0][1];
    self->up.y = m[1][1];
    self->up.z = m[2][1];
    self->pos.x = pose->pos.x;
    self->pos.y = pose->pos.y;
    self->pos.z = pose->pos.z;
    self->target.x = pose->pos.x - m[0][2];
    self->target.y = pose->pos.y - m[1][2];
    self->target.z = pose->pos.z - m[2][2];
}

/* CCamera::getView */
void fn_1_6F4(M579Cam *self, M579Pose *out)
{
    Mtx m;

    C_MTXLookAt(m, &self->pos, &self->up, &self->target);
    PSMTXInverse(m, m);
    out->pos.x = self->pos.x;
    out->pos.y = self->pos.y;
    out->pos.z = self->pos.z;
    C_QUATMtx((Quaternion *)out->rot, m);
}

/* CCamera::lerpView */
void fn_1_76C(M579Cam *self, M579Pose *a, M579Pose *b, f32 t)
{
    Mtx m;
    Quaternion q;
    Vec p, pa, pb;

    C_QUATSlerp((Quaternion *)a->rot, (Quaternion *)b->rot, &q, t);
    PSVECScale(&a->pos, &pa, 1.0f - t);
    PSVECScale(&b->pos, &pb, t);
    PSVECAdd(&pa, &pb, &p);
    PSMTXQuat(m, &q);
    self->up.x = m[0][1];
    self->up.y = m[1][1];
    self->up.z = m[2][1];
    self->pos.x = p.x;
    self->pos.y = p.y;
    self->pos.z = p.z;
    self->target.x = p.x - m[0][2];
    self->target.y = p.y - m[1][2];
    self->target.z = p.z - m[2][2];
}

/* CCamera::getOffset: move the eye to `out`, dragging the target along */
void fn_1_87C(M579Cam *self, Vec *out)
{
    Vec delta;

    PSVECSubtract(out, &self->pos, &delta);
    self->pos.x = out->x;
    self->pos.y = out->y;
    self->pos.z = out->z;
    PSVECAdd(&self->target, &delta, &self->target);
}

/* eye = lerp(a, b, t), dragging the target along */
void fn_1_8E8(M579Cam *self, Vec *a, Vec *b, f32 t)
{
    Vec pa, pb, delta;

    PSVECScale(a, &pa, 1.0f - t);
    PSVECScale(b, &pb, t);
    PSVECAdd(&pa, &pb, &pb);
    PSVECSubtract(&pb, &self->pos, &delta);
    self->pos.x = pb.x;
    self->pos.y = pb.y;
    self->pos.z = pb.z;
    PSVECAdd(&self->target, &delta, &self->target);
}

/* CCamera::setPos */
void fn_1_9A0(M579Cam *self, u16 camBit)
{
    Hu3DCameraPosSetV(camBit, &self->pos, &self->up, &self->target);
}

/* CCamera::getPos */
void fn_1_9E0(M579Cam *self, u16 camBit)
{
    Hu3DCameraPosGet(camBit, &self->pos, &self->up, &self->target);
}

/* CCamera::printView: dump the current view as a pose table row (floats as raw words) */
void fn_1_A20(M579Cam *self)
{
    Mtx m;
    union {
        M579Pose pose;
        u32 w[7];
    } out;

    C_MTXLookAt(m, &self->pos, &self->up, &self->target);
    PSMTXInverse(m, m);
    out.pose.pos.x = self->pos.x;
    out.pose.pos.y = self->pos.y;
    out.pose.pos.z = self->pos.z;
    C_QUATMtx((Quaternion *)out.pose.rot, m);
    printf(lbl_1_data_3D, out.w[0], out.w[1], out.w[2], out.w[3], out.w[4], out.w[5], out.w[6]);
}

f32 fn_1_AB8(s32 pad)
{
    return 0.017857144f * HuPadStkX[pad];
}

f32 fn_1_B0C(s32 pad)
{
    return 0.017857144f * HuPadStkY[pad];
}

f32 fn_1_B60(s32 pad)
{
    return 0.016949153f * HuPadSubStkX[pad];
}

f32 fn_1_BB4(s32 pad)
{
    return 0.016949153f * HuPadSubStkY[pad];
}

/* CCamera::debugView: free-fly debug camera driven by pad `pad` */
void fn_1_C08(M579Cam *self, s32 pad)
{
    Mtx inv, rot, rotB, rotA, m, dump;
    union {
        M579Pose pose;
        u32 w[7];
    } out;
    Vec mv, fwd, z, y, x;
    f32 yaw, pitch, speed, len;
    u32 btn;
    u8 fast;

    btn = HuPadBtn[pad];
    if (btn & 0x800) {
        self->up.x = 0.0f;
        self->up.y = 1.0f;
        self->up.z = 0.0f;
    }
    yaw = -4.0f * fn_1_AB8(pad);
    pitch = -4.0f * fn_1_B0C(pad);
    C_MTXLookAt(m, &self->pos, &self->up, &self->target);
    x.x = m[0][0];
    x.y = m[0][1];
    x.z = m[0][2];
    y.x = m[1][0];
    y.y = m[1][1];
    y.z = m[1][2];
    z.x = m[2][0];
    z.y = m[2][1];
    z.z = m[2][2];
    PSMTXRotAxisRad(rotA, &x, 0.017453292f * pitch);
    PSMTXRotRad(rotB, 'Y', 0.017453292f * yaw);
    PSMTXConcat(rotA, rotB, rot);
    PSMTXMultVecSR(rot, &z, &z);
    PSMTXMultVecSR(rot, &y, &self->up);
    PSVECSubtract(&self->pos, &z, &self->target);
    fast = (btn & 0x200) != 0;
    speed = fast ? 70.0f : 20.0f;
    C_MTXLookAt(inv, &self->pos, &self->up, &self->target);
    PSMTXInverse(inv, inv);
    mv.x = (speed * (u8)((btn & 2) != 0)) - (speed * (u8)((btn & 1) != 0));
    mv.y = (speed * (u8)((btn & 8) != 0)) - (speed * (u8)((btn & 4) != 0));
    mv.x = mv.x + (speed * fn_1_B60(pad));
    mv.z = 0.0f;
    PSMTXMultVecSR(inv, &mv, &mv);
    mv.y = mv.y + (speed * fn_1_BB4(pad));
    PSVECAdd(&mv, &self->target, &self->target);
    PSVECAdd(&mv, &self->pos, &self->pos);
    fwd.x = 0.0f;
    fwd.y = 0.0f;
    fwd.z = (f32)((u8)((btn & 0x400) != 0) - (u8)((btn & 0x100) != 0));
    PSMTXMultVecSR(inv, &fwd, &fwd);
    fwd.y = 0.0f;
    len = PSVECMag(&fwd);
    if (len > 1e-10f) {
        PSVECScale(&fwd, &fwd, speed / len);
        PSVECAdd(&fwd, &self->target, &self->target);
        PSVECAdd(&fwd, &self->pos, &self->pos);
    }
    if ((HuPadBtnDown[pad] & 0x200) != 0) {
        C_MTXLookAt(dump, &self->pos, &self->up, &self->target);
        PSMTXInverse(dump, dump);
        out.pose.pos.x = self->pos.x;
        out.pose.pos.y = self->pos.y;
        out.pose.pos.z = self->pos.z;
        C_QUATMtx((Quaternion *)out.pose.rot, dump);
        printf(lbl_1_data_3D, out.w[0], out.w[1], out.w[2], out.w[3], out.w[4], out.w[5], out.w[6]);
    }
}

} /* extern "C" */
