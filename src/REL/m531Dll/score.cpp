#include "dolphin/mtx.h"

// Declared here rather than through game/esp.h: that header pulls in MSL math.h,
// whose inline std::sqrtf emits constants into .rodata.
extern "C" {
typedef s16 HU3DMODELID;
typedef s16 HU3DMOTID;
typedef struct Process_s OMOBJMAN;
typedef struct omObj_s OMOBJ;
typedef void (*OMOBJFUNC)(OMOBJ *obj);

struct omObj_s {
    u16 stat;
    s16 objNext;
    s16 prio;
    s16 prev;
    s16 next;
    s16 nextNo;
    s16 grpNo;
    u16 memberNo;
    u32 mode;
    OMOBJFUNC objFunc;
    Vec trans;
    Vec rot;
    Vec scale;
    u16 mdlcnt;
    HU3DMODELID *mdlId;
    u16 mtncnt;
    HU3DMOTID *mtnId;
    u32 work[4];
    void *data;
};

s16 espEntry(u32 dataNum, s16 bank, s16 group);
void espKill(s16 espId);
void espDispOn(s16 espId);
void espDispOff(s16 espId);
void espBankSet(s16 espId, s16 bank);
void espColorSet(s16 espId, u8 r, u8 g, u8 b);
void espPosSet(s16 espId, f32 x, f32 y);
u32 frand(void);
}

// Declared outside the class: with the destructor as the first virtual and
// inline, MWCC emits the vtable (weak) in every unit that needs it.
namespace m531 {
class CScoreBase {
public:
    OMOBJMAN *m_objMan;
    OMOBJ *m_obj;

    CScoreBase(OMOBJMAN *objMan);
    static void ObjUpdate(OMOBJ *obj);
    virtual void on_update();
    virtual ~CScoreBase();
};

class CScore : public CScoreBase {
public:
    s16 m_esp[2][3];
    s32 m_unk18[2];
    u8 m_unk20;
    s32 m_unk24;

    CScore(OMOBJMAN *objMan);
    virtual void on_update();
    virtual ~CScore();
};

extern "C" {
void fn_1_79E4(CScore *self, s32 row, s32 value);
void fn_1_7AC0(CScore *self);
void fn_1_7B38(CScore *self);
}
}

// Row/colour of the six ESPs this class owns: two rows of three digits.
struct ScoreColor {
    u8 r;
    u8 g;
    u8 b;
    u8 pad;
};

static const ScoreColor lbl_1_rodata_3F8[2] = {
    { 0xF0, 0x00, 0x00, 0xFF },
    { 0x30, 0x60, 0xF0, 0xFF },
};

static const f32 lbl_1_rodata_400 = 32.0f;
static const f32 lbl_1_rodata_404 = 144.0f;
static const f32 lbl_1_rodata_408 = 120.0f;
static const f64 lbl_1_rodata_410 = 4503601774854144.0;
static const f32 lbl_1_rodata_418 = 100.0f;
static const f32 lbl_1_rodata_41C = 0.000015258789f;
static const f64 lbl_1_rodata_420 = 4503599627370496.0;

namespace m531 {

CScore::CScore(OMOBJMAN *objMan) : CScoreBase(objMan)
{
    s32 i;
    s32 j;

    m_unk20 = 0;
    m_unk24 = 0;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            m_esp[i][j] = espEntry(0x59000D, 10, 0);
            espPosSet(m_esp[i][j],
                lbl_1_rodata_400 * j + (lbl_1_rodata_404 * ((i & 1) * 2 + 1) - lbl_1_rodata_400),
                lbl_1_rodata_408 * ((s16)(i / 2) * 2 + 1));
            espColorSet(m_esp[i][j], lbl_1_rodata_3F8[i].r, lbl_1_rodata_3F8[i].g, lbl_1_rodata_3F8[i].b);
        }
        espBankSet(m_esp[i][2], 10);
    }
    for (i = 0; i < 2; i++) {
        m_unk18[i] = 0;
    }
}

CScore::~CScore()
{
    for (s32 i = 0; i < 2; i++) {
        for (s32 j = 0; j < 3; j++) {
            espKill(m_esp[i][j]);
        }
    }
}

void CScore::on_update()
{
    if (m_unk20 == 0) {
        if (((m_unk24 = m_unk24 + 1) & 1) == 0) {
            for (s32 i = 0; i < 2; i++) {
                s16 value = (s16)(lbl_1_rodata_418 * (lbl_1_rodata_41C * (u16)frand()));
                espBankSet(m_esp[i][0], (s16)((value / 10) % 10));
                espBankSet(m_esp[i][1], (s16)(value % 10));
            }
        }
    }
}

void fn_1_79E4(CScore *self, s32 row, s32 value)
{
    s32 hundreds = value / 100;
    espBankSet(self->m_esp[row][0], (s16)((value / 10) % 10));
    espBankSet(self->m_esp[row][1], (s16)(value % 10));
}

void fn_1_7AC0(CScore *self)
{
    for (s32 i = 0; i < 2; i++) {
        for (s32 j = 0; j < 3; j++) {
            espDispOn(self->m_esp[i][j]);
        }
    }
}

void fn_1_7B38(CScore *self)
{
    for (s32 i = 0; i < 2; i++) {
        for (s32 j = 0; j < 3; j++) {
            espDispOff(self->m_esp[i][j]);
        }
    }
}
}
