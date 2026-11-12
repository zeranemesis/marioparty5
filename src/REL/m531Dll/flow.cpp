// m531Dll "flow": the cleaning contest's game flow -- the camera object, the
// CFlow mode functions (setup, intro, play, result, exit), the result check and
// the std::sort-style helpers that order the players by team.
#include "REL/m531Dll.h"
#include "game/flag.h"
#include "game/gamework.h"

inline void *operator new(unsigned long, void *p)
{
    return p;
}

extern "C" {
double atan2(double y, double x);
void PSVECSubtract(const Vec *a, const Vec *b, Vec *ab);
void PSVECNormalize(const Vec *src, Vec *dst);
void PSVECScale(const Vec *src, Vec *dst, f32 scale);

void *__dl__FPv(void *ptr);

void *fn_1_0(s32 size);
CTimer *fn_1_2380(CTimer *self, s16 flag);
void fn_1_23DC(CTimer *self, s32 a, s32 b, s32 c);
s32 fn_1_2424(CTimer *self);
void fn_1_24D4(CTimer *self, s32 mode);
void fn_1_2598(CTimer *self);
void fn_1_25C8(CTimer *self);
u8 fn_1_2630(CTimer *self);
void *fn_1_6080(void *self, s32 maxCnt, const Vec *pos, const Vec *rot);
void fn_1_79E4(m531::CScore *score, s32 index, s32 value);
void fn_1_7AC0(m531::CScore *score);
void fn_1_7B38(m531::CScore *score);
CStageSet *fn_1_7BB0(CStageSet *self, OMOBJMAN *objMan);
void *fn_1_7E40(CStageSet *self, s16 flag);
CPlayer *fn_1_9270(CPlayer *self, m531::CFlow *flow, void *arg2, PlayerMotInfo *tbl, s32 num, s16 playerNo, s32 team);
CPlayerRunner *fn_1_B55C(CPlayerRunner *self, OMOBJMAN *objMan, PlayerMotInfo *tbl, s32 num, s32 dataNum);
void fn_1_BC40(m531::CFlow *flow, s32 mode);
void fn_1_CA44(CPlayerBase *self, s32 mode, s32 mot);
void *fn_1_D62C(void *self, OMOBJMAN *objMan);
void *fn_1_D8F8(void *self, s16 flag);
s32 fn_1_DA00(void *clean, u8 arg);
void *fn_1_DAEC(void *clean, s32 side);
void fn_1_F668(void *clean, Vec *pos, f32 value);
f32 fn_1_F744(void *clean);

void Hu3DBGColorSet(u8 r, u8 g, u8 b);
void Hu3DCameraCreate(u16 bit);
void Hu3DCameraPerspectiveSet(u16 bit, f32 fov, f32 nearZ, f32 farZ, f32 aspect);
void Hu3DCameraViewportSet(u16 bit, f32 x, f32 y, f32 w, f32 h, f32 nearZ, f32 farZ);
void Hu3DCameraPosSetV(u16 bit, Vec *pos, Vec *up, Vec *target);
s16 Hu3DGLightCreateV(Vec *pos, Vec *dir, GXColor *color);
void Hu3DGLightInfinitytSet(s16 id);
void Hu3DGLightStaticSet(s16 id, s32 on);
void Hu3DShadowCreate(f32 fov, f32 nearZ, f32 farZ);
void Hu3DShadowPosSet(Vec *pos, Vec *up, Vec *target);
void Hu3DShadowTPLvlSet(f32 level);
s16 GameMesCreate(s32 mes, s32 id, ...);
u8 GameMesStatGet(s16 id);
void GameMesDispSet(s16 id, s32 a, s32 b);
void GameMesClose(void);
s32 HuAudBGMPlay(int bgm);
void HuAudSStreamFadeOut(int stream, int frames);
void CharLoseVoicePlay(s16 a, s16 b, s16 c, s16 d);
void WipeCreate(s32 type, s32 mode, s32 frames);
u8 WipeCheckEnd(void);

void fn_1_2CA0(OMOBJ *obj);
void fn_1_2CE8(OMOBJ *obj);
CFlowCamera *fn_1_2D30(CFlowCamera *self, OMOBJMAN *objMan);
CFlowCamera *fn_1_2F64(CFlowCamera *self, s16 flag);
void fn_1_2FD4(CFlowCamera *self);
void fn_1_30CC(CFlowCamera *self);
void fn_1_31F8(CFlowCamera *self);
void *fn_1_3264(m531::CFlow *self, s16 flag);
void fn_1_33D4(void);
void fn_1_33D8(m531::CFlow *self);
void fn_1_3C14(m531::CFlow *self);
void fn_1_3C8C(m531::CFlow *self);
void fn_1_3D74(m531::CFlow *self);
void fn_1_3F60(m531::CFlow *self);
void fn_1_41EC(m531::CFlow *self);
void fn_1_491C(m531::CFlow *self);
u8 fn_1_4A18(m531::CFlow *self, s32 time);
f32 fn_1_4B6C(m531::CFlow *flow, s32 mode, s32 col, s32 row);
void fn_1_4BC0(m531::CFlow *flow, s32 mode, s32 col, s32 row, f32 value);
void fn_1_4BF8(void);

extern s32 lbl_1_bss_10;
extern s32 lbl_1_bss_14;
extern s32 lbl_1_bss_18;
extern void *lbl_1_bss_1C;
extern FlowCamVt lbl_1_data_154;
}

// Comparator state of the partition helpers: a flag byte, then the pivot pointer.
struct PartCmp {
    u8 m_flag;
    u32 *m_pivot;
};

// Comparator calls go through nested inline wrappers (the original keeps each result in a stack copy).
static inline void SwapU32(u32 *a, u32 *b)
{
    u32 t = *a;
    *a = *b;
    *b = t;
}

static inline u8 PivotLess1(u32 *x, PartCmp *c)
{
    u32 *p = c->m_pivot;
    return *x < *p;
}

static inline u8 PivotLess2(u32 *x, PartCmp *c)
{
    return PivotLess1(x, c);
}

static inline u8 PivotLess(u32 *x, PartCmp *c)
{
    return PivotLess2(x, c);
}

static inline u8 PivotGreater1(PartCmp *c, u32 *x)
{
    u32 *p = c->m_pivot;
    return *p < *x;
}

static inline u8 PivotGreater2(PartCmp *c, u32 *x)
{
    return PivotGreater1(c, x);
}

static inline u8 PivotGreater(PartCmp *c, u32 *x)
{
    return PivotGreater2(c, x);
}

extern "C" {
void fn_1_4BFC(u32 *first, u32 *last);
void fn_1_4E60(u32 *a, u32 *b, u32 *c);
u32 *fn_1_4F58(u32 *first, u32 *last, PartCmp *cmp);
u32 *fn_1_5264(u32 *first, u32 *last, PartCmp *cmp);
void fn_1_562C(u32 *first, u32 *last);
void fn_1_56CC(void);
}

s32 lbl_1_bss_10;
s32 lbl_1_bss_14;
s32 lbl_1_bss_18;
void *lbl_1_bss_1C;

// Motion tables of the players (12 entries) and of the runner (5 entries, then two spare placements).
static const PlayerMotInfo kPlayerMotions[12] = {
    { 0x8C0000, 0x40000001, 8.0f },
    { 0x8C0001, 0x40000001, 8.0f },
    { 0x8C0002, 0x40000001, 8.0f },
    { 0x8C0003, 0, 8.0f },
    { 0x8C0004, 0, 8.0f },
    { 0x8C0009, 0, 8.0f },
    { 0x8C0010, 0, 8.0f },
    { 0x8C0011, 0, 8.0f },
    { 0x8C0006, 0, 1.0f },
    { 0x8C0007, 0, 1.0f },
    { 0x8C0029, 0x40000001, 8.0f },
    { 0x8C004D, 0x40000001, 8.0f },
};

static const PlayerMotInfo kRunnerMotions[5] = {
    { 0x59000F, 0x40000001, 8.0f },
    { 0x59000F, 0x40000001, 8.0f },
    { 0x59000F, 0x40000001, 8.0f },
    { 0x59000F, 0x40000001, 8.0f },
    { 0x59000F, 0x40000001, 8.0f },
};

struct PlayerPlace {
    Vec m_pos;
    Vec m_rot;
};

static const PlayerPlace kRunnerSpare[2] = {
    { { -1000.0f, 0.0f, 560.0f }, { 0.0f, 180.0f, 0.0f } },
    { { 1000.0f, 0.0f, 560.0f }, { 0.0f, 180.0f, 0.0f } },
};

static const PlayerPlace kPlayerPlace[4] = {
    { { -450.0f, 1000.0f, -130.0f }, { 0.0f, 0.0f, 0.0f } },
    { { -200.0f, 1000.0f, 560.0f }, { 0.0f, 180.0f, 0.0f } },
    { { 200.0f, 1000.0f, 560.0f }, { 0.0f, 180.0f, 0.0f } },
    { { 450.0f, 1000.0f, -130.0f }, { 0.0f, 0.0f, 0.0f } },
};

static const PlayerPlace kPlayerHome[4] = {
    { { -420.0f, 10.0f, 560.0f }, { 0.0f, 180.0f, 0.0f } },
    { { -230.0f, 10.0f, 560.0f }, { 0.0f, 180.0f, 0.0f } },
    { { 230.0f, 10.0f, 560.0f }, { 0.0f, 180.0f, 0.0f } },
    { { 420.0f, 10.0f, 560.0f }, { 0.0f, 180.0f, 0.0f } },
};

// Camera destinations: eye (x is set separately) then target.
struct CamDest {
    f32 m_x;
    f32 m_eyeY;
    f32 m_eyeZ;
    Vec m_target;
};

static const CamDest kCamBack = { 0.0f, 300.0f, 2800.0f, { 0.0f, 100.0f, 600.0f } };
static const CamDest kCamLeft = { -825.0f, 900.0f, 2600.0f, { -325.0f, -100.0f, 600.0f } };
static const CamDest kCamRight = { 825.0f, 900.0f, 2600.0f, { 325.0f, -100.0f, 600.0f } };
static const CamDest kCamStart = { -325.0f, 300.0f, 1000.0f, { -325.0f, 0.0f, -250.0f } };

// Camera preset: eye and target of the field view, then fov, near and far.
struct CamPreset {
    f32 m_a[3];
    f32 m_b[3];
    f32 m_pad;
    f32 m_fov;
    f32 m_near;
    f32 m_far;
};

static const CamPreset kCamPreset = { { 3.279f, 2531.566f, 1651.557f }, { 3.279f, -570.875f, 28.275f }, 0.0f, 25.0f, 20.0f, 32768.0f };
static const Vec kShadowDir = { -4425.815f, 6717.712f, 5071.9f };
static const Vec kUpVec = { 0.0f, 1.0f, 0.0f };
static const Vec kLightPos = { 0.0f, 171.01f, 469.85f };
static const Vec kLightDir = { 0.0f, -0.68f, -0.94f };
static const GXColor kLightColor = { 0xFF, 0xFF, 0xFF, 0xFF };
static const Vec kShadowUp = { 0.0f, 1.0f, 0.0f };
static const Vec kShadowTarget = { 0.0f, 0.0f, 0.0f };
static const Vec kRunnerPos = { -700.0f, 0.3f, -200.0f };
static const Vec kZeroVec = { 0.0f, 0.0f, 0.0f };

void fn_1_2CA0(OMOBJ *obj)
{
    CFlowCamera *self = (CFlowCamera *)obj->data;

    self->m_vt->m_update0(self);
}

void fn_1_2CE8(OMOBJ *obj)
{
    CFlowCamera *self = (CFlowCamera *)obj->data;

    self->m_vt->m_update1(self);
}

CFlowCamera *fn_1_2D30(CFlowCamera *self, OMOBJMAN *objMan)
{
    Vec up;

    self->m_vt = &lbl_1_data_154;
    self->m_objMan = objMan;
    self->m_frame = 0;
    self->m_angle = 0.0f;
    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, kCamPreset.m_fov, kCamPreset.m_near, kCamPreset.m_far, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    self->m_obj = omAddObjEx(objMan, 1, 0, 0, -1, fn_1_2CA0);
    self->m_obj->data = self;
    up = kUpVec;
    self->m_pos.x = -325.0f;
    self->m_pos.y = kCamStart.m_eyeY;
    self->m_pos.z = kCamStart.m_eyeZ;
    self->m_up.x = up.x;
    self->m_up.y = up.y;
    self->m_up.z = up.z;
    self->m_target.x = kCamStart.m_target.x;
    self->m_target.y = kCamStart.m_target.y;
    self->m_target.z = kCamStart.m_target.z;
    Hu3DCameraPosSetV(1, (Vec *)&kCamStart, &up, (Vec *)&kCamStart.m_target);
    self->m_destPos.x = -325.0f;
    self->m_destPos.y = kCamStart.m_eyeY;
    self->m_destPos.z = kCamStart.m_eyeZ;
    self->m_destTarget.x = kCamStart.m_target.x;
    self->m_destTarget.y = kCamStart.m_target.y;
    self->m_destTarget.z = kCamStart.m_target.z;
    return self;
}

CFlowCamera *fn_1_2F64(CFlowCamera *self, s16 flag)
{
    if (self != NULL) {
        self->m_vt = &lbl_1_data_154;
        self->m_obj->data = NULL;
        omDelObjEx(self->m_objMan, self->m_obj);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

void fn_1_2FD4(CFlowCamera *self)
{
    f32 unused;

    unused = 30.0f;
    Hu3DCameraPosSetV(1, &self->m_pos, &self->m_up, &self->m_target);
    self->m_frame = self->m_frame + 1;
    if (self->m_frame >= 0xB4) {
        self->m_destTarget.x = kCamPreset.m_b[0];
        self->m_destTarget.y = kCamPreset.m_b[1];
        self->m_destTarget.z = kCamPreset.m_b[2];
        self->m_destPos.x = 3.279f;
        self->m_destPos.y = kCamPreset.m_a[1];
        self->m_destPos.z = kCamPreset.m_a[2];
        self->m_obj->objFunc = fn_1_2CE8;
    }
}

void fn_1_30CC(CFlowCamera *self)
{
    Vec *pos;
    Vec *target;

    pos = &self->m_pos;
    target = &self->m_target;
    self->m_pos.x = self->m_pos.x + ((self->m_destPos.x - self->m_pos.x) / 30.0f);
    pos->y = pos->y + ((self->m_destPos.y - pos->y) / 30.0f);
    pos->z = pos->z + ((self->m_destPos.z - pos->z) / 30.0f);
    self->m_target.x = self->m_target.x + ((self->m_destTarget.x - self->m_target.x) / 30.0f);
    target->y = target->y + ((self->m_destTarget.y - target->y) / 30.0f);
    target->z = target->z + ((self->m_destTarget.z - target->z) / 30.0f);
    Hu3DCameraPosSetV(1, pos, &self->m_up, target);
    fn_1_31F8(self);
}

void fn_1_31F8(CFlowCamera *self)
{
    Vec diff;

    PSVECSubtract(&self->m_pos, &self->m_target, &diff);
    self->m_angle = 180.0 * (atan2(diff.x, diff.z) / 3.141592653589793);
}

void *fn_1_3264(m531::CFlow *self, s16 flag)
{
    s32 i;

    if (self != NULL) {
        for (i = 0; i < 4; i++) {
            CPlayer *player = self->m_player[i];
            if (player != NULL) {
                player->m_base.m_vt->m_dtor(&player->m_base, 1);
            }
        }
        {
            CPlayerRunner *runner = self->m_runner;
            if (runner != NULL) {
                runner->m_base.m_vt->m_dtor(&runner->m_base, 1);
            }
        }
        {
            CFlowCamera *cam = self->m_camera;
            if (cam != NULL) {
                cam->m_vt = &lbl_1_data_154;
                cam->m_obj->data = NULL;
                omDelObjEx(cam->m_objMan, cam->m_obj);
                __dl__FPv(cam);
            }
        }
        fn_1_D8F8(self->m_cleaner, 1);
        fn_1_7E40(self->m_stageSet, 1);
        delete self->m_score;
        fn_1_2380(&self->m_timer, -1);
        if (self != NULL) {
            self->CMinigameCore::~CMinigameCore();
        }
        if (flag > 0) {
            CMinigameCore::operator delete(self);
        }
    }
    return self;
}

void fn_1_33D4(void)
{
}

void fn_1_33D8(m531::CFlow *self)
{
    CFlowCamera *cam;
    void *obj;
    Vec lightPos;
    Vec lightDir;
    GXColor lightColor;
    s16 lightId;
    Vec shadowPos;
    Vec shadowUp;
    Vec shadowTarget;
    u32 order[4];
    Vec runnerPos;
    CPlayerRunner *runner;
    s32 i;
    s32 j;

    Hu3DBGColorSet(0x1D, 0x80, 0xFD);
    obj = fn_1_0(0x50);
    if (obj != NULL) {
        obj = fn_1_2D30((CFlowCamera *)obj, self->m_objMan);
    }
    self->m_camera = (CFlowCamera *)obj;
    lightPos = kLightPos;
    lightDir = kLightDir;
    lightColor = kLightColor;
    PSVECNormalize(&lightDir, &lightDir);
    lightId = Hu3DGLightCreateV(&lightPos, &lightDir, &lightColor);
    Hu3DGLightInfinitytSet(lightId);
    Hu3DGLightStaticSet(lightId, 1);
    Hu3DShadowCreate(kCamPreset.m_fov, kCamPreset.m_near, kCamPreset.m_far);
    Hu3DShadowTPLvlSet(0.2f);
    shadowUp = kShadowUp;
    shadowTarget = kShadowTarget;
    PSVECNormalize(&kShadowDir, &shadowPos);
    PSVECScale(&shadowPos, &shadowPos, 3400.0f);
    Hu3DShadowPosSet(&shadowPos, &shadowUp, &shadowTarget);
    obj = fn_1_0(0x40);
    if (obj != NULL) {
        obj = fn_1_7BB0((CStageSet *)obj, self->m_objMan);
    }
    self->m_stageSet = (CStageSet *)obj;
    obj = fn_1_0(8);
    if (obj != NULL) {
        obj = fn_1_D62C(obj, self->m_objMan);
    }
    self->m_cleaner = obj;
    for (i = 0; i < 4; i++) {
        order[i] = i | (GwPlayerConf[i].grp << 16);
    }
    fn_1_4BFC(order, order + 4);
    for (j = 0; j < 4; j++) {
        s32 playerNo = order[j] & 3;
        void *side = fn_1_DAEC(self->m_cleaner, j / 2);
        CPlayer *player = (CPlayer *)fn_1_0(sizeof(CPlayer));
        if (player != NULL) {
            player = fn_1_9270(player, self, side, (PlayerMotInfo *)kPlayerMotions, 0xC, (s16)playerNo, j);
        }
        player->m_base.m_pos.x = kPlayerPlace[j].m_pos.x;
        player->m_base.m_pos.y = kPlayerPlace[j].m_pos.y;
        player->m_base.m_pos.z = kPlayerPlace[j].m_pos.z;
        player->m_base.m_spawn.x = kPlayerPlace[j].m_pos.x;
        player->m_base.m_spawn.y = kPlayerPlace[j].m_pos.y;
        player->m_base.m_spawn.z = kPlayerPlace[j].m_pos.z;
        player->m_base.m_home.x = kPlayerHome[j].m_pos.x;
        player->m_base.m_home.y = kPlayerHome[j].m_pos.y;
        player->m_base.m_home.z = kPlayerHome[j].m_pos.z;
        player->m_base.m_rot.x = kPlayerPlace[j].m_rot.x;
        player->m_base.m_rot.y = kPlayerPlace[j].m_rot.y;
        player->m_base.m_rot.z = kPlayerPlace[j].m_rot.z;
        self->m_player[playerNo] = player;
    }
    runnerPos = kRunnerPos;
    runner = (CPlayerRunner *)fn_1_0(sizeof(CPlayerRunner));
    if (runner != NULL) {
        runner = fn_1_B55C(runner, self->m_objMan, (PlayerMotInfo *)kRunnerMotions, 5, 0x59000E);
    }
    self->m_runner = runner;
    runner = self->m_runner;
    runner->m_base.m_pos.x = runnerPos.x;
    runner->m_base.m_pos.y = runnerPos.y;
    runner->m_base.m_pos.z = runnerPos.z;
    WipeCreate(1, 5, 0x3C);
    self->m_mesCount = -1;
    self->m_mesStart = -1;
    self->m_mesEnd = -1;
    self->m_counter = 0;
    self->m_winner = -1;
    self->m_result = -1;
    obj = fn_1_0(0x28);
    if (obj != NULL) {
        obj = new (obj) m531::CScore(self->m_objMan);
    }
    self->m_score = (m531::CScore *)obj;
    fn_1_7B38(self->m_score);
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 5; j++) {
            self->m_grid[0][i][j] = 0.0f;
            self->m_grid[1][i][j] = 0.0f;
        }
    }
    self->m_scoreVal[0] = 0.0f;
    self->m_scoreVal[1] = 0.0f;
    self->m_bgm = HuAudBGMPlay(0x32);
    fn_1_BC40(self, 1);
}

void fn_1_3C14(m531::CFlow *self)
{
    if (WipeCheckEnd() == 0) {
        self->m_counter = self->m_counter + 1;
        if (self->m_counter >= 0xB4) {
            self->m_counter = 0;
            self->m_mesCount = GameMesCreate(2, 0);
            fn_1_BC40(self, 2);
        }
    }
}

void fn_1_3C8C(m531::CFlow *self)
{
    s32 i;

    if (GameMesStatGet(self->m_mesCount) == 0) {
        GameMesDispSet(self->m_mesCount, 2, -1);
        fn_1_23DC(&self->m_timer, 0xE10, 0, 0);
        fn_1_24D4(&self->m_timer, 1);
        for (i = 0; i < 4; i++) {
            CPlayer *player = self->m_player[i];
            player->m_base.m_vt->m_set0(&player->m_base);
        }
        {
            CPlayerRunner *runner = self->m_runner;
            if (runner != NULL) {
                runner->m_base.m_vt->m_dtor(&runner->m_base, 1);
            }
        }
        self->m_runner = (CPlayerRunner *)0;
        fn_1_BC40(self, 3);
    }
}

void fn_1_3D74(m531::CFlow *self)
{
    s32 i;
    s32 j;
    s32 k;

    for (i = 0; i < 4; i++) {
        s32 team = self->m_player[i]->m_team;
        void *side = fn_1_DAEC(self->m_cleaner, team / 2);
        CPlayer *player = self->m_player[i];
        Vec pos = player->m_unkC8;
        fn_1_F668(side, &pos, 130.0f);
    }
    if (fn_1_4A18(self, fn_1_2424(&self->m_timer)) != 0 || fn_1_2630(&self->m_timer) != 0) {
        if (self->m_result == 1) {
            fn_1_2598(&self->m_timer);
        }
        for (j = 0; j < 2; j++) {
            self->m_scoreVal[j] = (s32)(100.0f * fn_1_F744(fn_1_DAEC(self->m_cleaner, j)));
        }
        self->m_mesStart = GameMesCreate(2, 1);
        fn_1_25C8(&self->m_timer);
        for (k = 0; k < 4; k++) {
            CPlayer *player = self->m_player[k];
            player->m_base.m_vt->m_set1(&player->m_base);
        }
        self->m_step = 0;
        self->m_counter = 0;
        HuAudSStreamFadeOut(self->m_bgm, 0x64);
        fn_1_BC40(self, 4);
    }
}

void fn_1_3F60(m531::CFlow *self)
{
    CFlowCamera *cam;
    u8 all;
    s32 i;
    s32 j;

    switch (self->m_step) {
        case 0:
            if (GameMesStatGet(self->m_mesStart) != 0) {
                break;
            }
            GameMesDispSet(self->m_mesStart, 2, -1);
            cam = self->m_camera;
            cam->m_destPos.x = 0.0f;
            cam->m_destPos.y = kCamBack.m_eyeY;
            cam->m_destPos.z = kCamBack.m_eyeZ;
            cam = self->m_camera;
            cam->m_destTarget.x = kCamBack.m_target.x;
            cam->m_destTarget.y = kCamBack.m_target.y;
            cam->m_destTarget.z = kCamBack.m_target.z;
            self->m_step = self->m_step + 1;
        case 1:
            all = 1;
            for (i = 0; i < 4; i++) {
                if (self->m_player[i]->m_base.m_flag55 == 0) {
                    all = 0;
                }
            }
            if (all == 0) {
                break;
            }
            if (self->m_result == 0) {
                fn_1_7AC0(self->m_score);
                HuAudFXPlay(0x1D);
                self->m_step = self->m_step + 1;
            } else {
                self->m_step = -1;
            }
        case 2:
            self->m_counter = self->m_counter + 1;
            if ((f32)self->m_counter < 90.0f) {
                break;
            }
            for (j = 0; j < 2; j++) {
                fn_1_79E4(self->m_score, j, (s32)self->m_scoreVal[j]);
            }
            self->m_score->m_unk20 = 1;
            self->m_counter = 0;
            if (self->m_result == 0) {
                HuAudFXPlay(0x1E);
            }
            self->m_step = self->m_step + 1;
        case 3:
            self->m_counter = self->m_counter + 1;
            if ((f32)self->m_counter < 90.0f) {
                break;
            }
            self->m_step = self->m_step + 1;
        default:
            self->m_step = 0;
            self->m_counter = 0;
            fn_1_BC40(self, 5);
    }
}

void fn_1_41EC(m531::CFlow *self)
{
    s32 i;
    s32 n;
    s32 winners[2];
    s16 chars[4];
    s16 charA;
    s16 charB;
    f32 dir;
    CFlowCamera *cam;

    switch (self->m_step) {
        case 0:
            if (GameMesStatGet(self->m_mesStart) != 0) {
                break;
            }
            GameMesDispSet(self->m_mesStart, 2, -1);
            if (self->m_winner != -1) {
                n = 0;
                for (i = 0; i < 4; i++) {
                    CPlayer *player = self->m_player[i];
                    if (self->m_winner == 0) {
                        s32 team = player->m_team;
                        if (team < 2) {
                            fn_1_CA44(&player->m_base, 0, 8);
                            winners[n] = i;
                            n++;
                        } else {
                            fn_1_CA44(&player->m_base, 1, 9);
                        }
                    } else {
                        s32 team = player->m_team;
                        if (team > 1) {
                            fn_1_CA44(&player->m_base, 0, 8);
                            winners[n] = i;
                            n++;
                        } else {
                            fn_1_CA44(&player->m_base, 1, 9);
                        }
                    }
                }
                charA = self->m_player[winners[1]]->m_base.m_charNo;
                charB = self->m_player[winners[0]]->m_base.m_charNo;
                self->m_mesEnd = GameMesCreate(4, 3, charB, charA, -1, -1);
                if (self->m_winner == 0) {
                    cam = self->m_camera;
                    cam->m_destPos.x = -825.0f;
                    cam->m_destPos.y = kCamLeft.m_eyeY;
                    cam->m_destPos.z = kCamLeft.m_eyeZ;
                    cam = self->m_camera;
                    cam->m_destTarget.x = kCamLeft.m_target.x;
                    cam->m_destTarget.y = kCamLeft.m_target.y;
                    cam->m_destTarget.z = kCamLeft.m_target.z;
                } else {
                    cam = self->m_camera;
                    cam->m_destPos.x = 825.0f;
                    cam->m_destPos.y = kCamRight.m_eyeY;
                    cam->m_destPos.z = kCamRight.m_eyeZ;
                    cam = self->m_camera;
                    cam->m_destTarget.x = kCamRight.m_target.x;
                    cam->m_destTarget.y = kCamRight.m_target.y;
                    cam->m_destTarget.z = kCamRight.m_target.z;
                }
                HuAudJinglePlay(0x79);
            } else {
                self->m_mesEnd = GameMesCreate(2, 2);
                for (i = 0; i < 4; i++) {
                    fn_1_CA44(&self->m_player[i]->m_base, -1, 9);
                    chars[i] = self->m_player[i]->m_base.m_charNo;
                }
                HuAudJinglePlay(0x7B);
                CharLoseVoicePlay(chars[0], chars[1], chars[2], chars[3]);
            }
            HuAudFXPlay(0x7AA);
            self->m_step = self->m_step + 1;
        case 1:
            self->m_counter = self->m_counter + 1;
            if ((self->m_scoreVal[0] == 100.0f || self->m_scoreVal[1] == 100.0f) && (self->m_counter % 15) == 0 &&
                self->m_winner != -1) {
                Vec zero = kZeroVec;
                Vec pos;
                void *effect;

                if (self->m_winner == 0) {
                    dir = -1.0f;
                } else {
                    dir = 1.0f;
                }
                pos.x = dir * (325.0f + (200.0f * (0.000030517578f * ((f32)frand() - 32768.0f))));
                pos.y = -(200.0f * (0.000015258789f * (f32)frand()));
                pos.z = 700.0f;
                effect = fn_1_0(0x10);
                if (effect != NULL) {
                    Vec rot = zero;
                    Vec p = pos;
                    fn_1_6080(effect, 0xA, &p, &rot);
                }
                pos.x = 550.0f * dir;
                pos.y = -(200.0f * (0.000015258789f * (f32)frand()));
                pos.z = -252.0f + (43.8f * (20.0f * (0.000015258789f * (f32)frand())));
                effect = fn_1_0(0x10);
                if (effect != NULL) {
                    Vec rot = zero;
                    Vec p = pos;
                    fn_1_6080(effect, 0xA, &p, &rot);
                }
            }
            if ((f32)self->m_counter < 210.0f) {
                break;
            }
            GameMesDispSet(self->m_mesEnd, 2, -1);
        default:
            self->m_step = 0;
            fn_1_BC40(self, 6);
    }
}

void fn_1_491C(m531::CFlow *self)
{
    switch (self->m_step) {
        case 0:
            WipeCreate(2, 6, 0x3C);
            delete self->m_score;
            self->m_score = (m531::CScore *)0;
            self->m_step = self->m_step + 1;
        case 1:
            if (WipeCheckEnd() != 0) {
                break;
            }
            self->m_step = self->m_step + 1;
        default:
            delete self;
            GameMesClose();
            HuAudFadeOut(0x3C);
            omOvlReturnEx(1, 1);
    }
}

// Decides the contest result from the cleaning progress; returns 1 once it is final.
u8 fn_1_4A18(m531::CFlow *self, s32 time)
{
    void *clean;
    s32 r;
    void *a;
    void *b;

    clean = self->m_cleaner;
    if (time > 0) {
        r = fn_1_DA00(clean, 0);
        if (r != 2) {
            if (r == 0) {
                self->m_winner = 0;
            } else {
                self->m_winner = 1;
            }
            self->m_result = 1;
            return 1;
        }
        a = fn_1_DAEC(clean, 0);
        b = fn_1_DAEC(clean, 1);
        if (fn_1_F744(a) >= 1.0f) {
            if (fn_1_F744(b) >= 1.0f) {
                self->m_winner = -1;
                self->m_result = 1;
                return 1;
            }
        }
        return 0;
    }
    r = fn_1_DA00(clean, 1);
    if (r != 2) {
        if (r == 0) {
            self->m_winner = 0;
        } else {
            self->m_winner = 1;
        }
    } else {
        self->m_winner = -1;
    }
    self->m_result = 0;
    return 1;
}

f32 fn_1_4B6C(m531::CFlow *flow, s32 mode, s32 col, s32 row)
{
    f32 value;

    if (mode == 0) {
        value = flow->m_grid[0][row][col];
    } else {
        value = flow->m_grid[1][row][col];
    }
    return value;
}

#pragma opt_level 4
void fn_1_4BC0(m531::CFlow *flow, s32 mode, s32 col, s32 row, f32 value)
{
    if (mode == 0) {
        flow->m_grid[0][row][col] = value;
    } else {
        flow->m_grid[1][row][col] = value;
    }
}
#pragma opt_level 0

void fn_1_4BF8(void)
{
}

// Sorts [first, last) ascending: quicksort with a rotating pivot choice, selection sort below 21 elements.
void fn_1_4BFC(u32 *first, u32 *last)
{
    PartCmp cmpA;
    PartCmp cmpB;
    u32 *lo;
    u32 *hi;
    u32 *mid;
    u32 *q3;
    u32 *lastElem;
    u32 *part;
    s32 n;
    u32 tmp;
    static s32 s_pivotStep;
    static u8 s_pivotInit;

    lo = first;
    hi = last;
    while (1) {
        n = hi - lo;
        if (n > 1) {
            if (n <= 0x14) {
                fn_1_562C(lo, hi);
                return;
            }
            if (s_pivotInit == 0) {
                s_pivotStep = -4;
                s_pivotInit = 1;
            }
            mid = lo + (n / 4) + (s_pivotStep % 5);
            s_pivotStep = s_pivotStep + 1;
            if (s_pivotStep >= 5) {
                s_pivotStep = -4;
            }
            q3 = lo + ((n * 3) / 4) + (s_pivotStep % 5);
            s_pivotStep = s_pivotStep + 1;
            if (s_pivotStep >= 5) {
                s_pivotStep = -4;
            }
            lastElem = hi - 1;
            fn_1_4E60(mid, q3, lastElem);
            cmpA.m_pivot = lastElem;
            part = fn_1_4F58(lo, lastElem, &cmpA);
            if (part == lo) {
                tmp = *part;
                *part = *lastElem;
                *lastElem = tmp;
                cmpB.m_pivot = lo;
                lo = fn_1_5264(part + 1, hi, &cmpB);
            } else if ((part - lo) < (hi - part)) {
                fn_1_4BFC(lo, part);
                lo = part;
            } else {
                fn_1_4BFC(part, hi);
                hi = part;
            }
            continue;
        }
        break;
    }
}

// Median of three: orders the three elements around the pivot slot.
void fn_1_4E60(u32 *a, u32 *b, u32 *c)
{
    u8 ac;
    u8 cb;
    u32 tmp;

    ac = *a <= *c;
    cb = *c <= *b;
    if (ac == 0 || cb == 0) {
        if (ac == 0 && cb == 0) {
            tmp = *a;
            *a = *b;
            *b = tmp;
            return;
        }
        if (*b < *a) {
            tmp = *a;
            *a = *b;
            *b = tmp;
        }
        if (ac != 0) {
            tmp = *b;
            *b = *c;
            *c = tmp;
            return;
        }
        tmp = *a;
        *a = *c;
        *c = tmp;
    }
}

// Partition around the pivot: elements below it stay in front.
u32 *fn_1_4F58(u32 *first, u32 *last, PartCmp *cmp)
{
    u32 *lo;
    u32 *hi;
    u32 *i;
    u32 *j;
    u8 loLess;
    u8 hiGreater;
    u32 tmp;

    lo = first;
    if (lo == last) {
        return lo;
    }
    loLess = PivotLess(lo, cmp);
    hi = last - 1;
    hiGreater = PivotLess(hi, cmp) == 0;
    if (loLess != 0 && hiGreater != 0) {
        j = hi;
        i = lo + 1;
        while (1) {
            while (PivotLess(i, cmp)) {
                i++;
            }
            do {
                j--;
            } while (PivotLess(j, cmp) == 0);
            if (i < j) {
                SwapU32(i, j);
                i++;
                continue;
            }
            return i;
        }
    }
    if (hiGreater != 0) {
        if (lo == hi) {
            return lo;
        }
        while (1) {
            hi--;
            if (lo != hi) {
                if (PivotLess(hi, cmp) == 0) {
                    continue;
                }
            }
            break;
        }
        if (lo == hi) {
            return lo;
        }
    } else if (loLess != 0) {
        u32 *end = hi + 1;
        lo++;
        while (1) {
            if (lo != end && (PivotLess(lo, cmp)) != 0) {
                lo++;
                continue;
            }
            break;
        }
        if (lo == end) {
            return lo;
        }
        hi = end - 1;
    }
    SwapU32(lo, hi);
    j = hi;
    i = lo + 1;
    while (1) {
        while (PivotLess(i, cmp)) {
            i++;
        }
        do {
            j--;
        } while (PivotLess(j, cmp) == 0);
        if (i < j) {
            SwapU32(i, j);
            i++;
            continue;
        }
        return i;
    }
}

// Same partition with the comparison reversed (pivot below element).
u32 *fn_1_5264(u32 *first, u32 *last, PartCmp *cmp)
{
    u32 *lo;
    u32 *hi;
    u32 *i;
    u32 *j;
    u8 loGreater;
    u8 notLoGreater;
    u8 hiGreater;
    u32 tmp;

    lo = first;
    if (lo == last) {
        return lo;
    }
    loGreater = PivotGreater(cmp, lo);
    notLoGreater = loGreater == 0;
    hi = last - 1;
    hiGreater = PivotGreater(cmp, hi);
    if (notLoGreater != 0 && hiGreater != 0) {
        j = hi;
        i = lo + 1;
        while (1) {
            while (PivotGreater(cmp, i) == 0) {
                i++;
            }
            do {
                j--;
            } while (PivotGreater(cmp, j) != 0);
            if (i < j) {
                SwapU32(i, j);
                i++;
                continue;
            }
            return i;
        }
    }
    if (hiGreater != 0) {
        if (lo == hi) {
            return lo;
        }
        while (1) {
            hi--;
            if (lo != hi) {
                if ((PivotGreater(cmp, hi)) != 0) {
                    continue;
                }
            }
            break;
        }
        if (lo == hi) {
            return lo;
        }
    } else if (notLoGreater != 0) {
        u32 *end = hi + 1;
        lo++;
        while (1) {
            if (lo != end) {
                if ((PivotGreater(cmp, lo)) == 0) {
                    lo++;
                    continue;
                }
            }
            break;
        }
        if (lo == end) {
            return lo;
        }
        hi = end - 1;
    }
    SwapU32(lo, hi);
    j = hi;
    i = lo + 1;
    while (1) {
        while (PivotGreater(cmp, i) == 0) {
            i++;
        }
        do {
            j--;
        } while (PivotGreater(cmp, j) != 0);
        if (i < j) {
            SwapU32(i, j);
            i++;
            continue;
        }
        return i;
    }
}

// Selection sort for the short ranges.
void fn_1_562C(u32 *first, u32 *last)
{
    u32 *p;
    u32 *min;
    u32 *q;
    u32 tmp;

    p = first;
    if (p != last) {
        while (p < last - 1) {
            min = p;
            if (p != last) {
                q = p + 1;
                while (q != last) {
                    if (*q < *min) {
                        min = q;
                    }
                    q++;
                }
            }
            if (min != p) {
                tmp = *min;
                *min = *p;
                *p = tmp;
            }
            p++;
        }
    }
}

void fn_1_56CC(void)
{
    lbl_1_bss_10 = 0x78;
    lbl_1_bss_14 = 0xD2;
    lbl_1_bss_18 = 0xD2;
}

// The flow camera vtable (slot 0 and 1 are driven through the object function).
FlowCamVt lbl_1_data_154 = { { NULL, NULL }, fn_1_2FD4, fn_1_30CC };

// The CFlow vtable: dtor, setup, update, close, then the seven mode functions.
extern "C" {
void *__vt__Q24m5315CFlow[13] = {
    NULL, NULL, (void *)fn_1_3264, NULL, NULL, (void *)fn_1_4BF8,
    (void *)fn_1_33D8, (void *)fn_1_3C14, (void *)fn_1_3C8C, (void *)fn_1_3D74,
    (void *)fn_1_3F60, (void *)fn_1_41EC, (void *)fn_1_491C,
};
}
