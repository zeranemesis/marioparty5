#ifndef M509TYPES_H
#define M509TYPES_H
/* Object types shared by every m509 unit; needs OSThread, Vec, Quaternion and GXColor from the caller. */

typedef struct M509Node {
    M509Node *next;
    M509Node *prev;
} M509Node;

/* Header of every m509 object: list node, vtable and flag word. */
typedef struct M509Base {
    M509Node node;
    void **vtbl;
    u32 flags;
} M509Base;

typedef struct M509Ptmf {
    s32 adj;
    s32 vtblIdx;
    void *func;
} M509Ptmf;

typedef struct M509Sema {
    OSThreadQueue queue;
    s32 count;
} M509Sema;

typedef struct M509Thread {
    OSThread th;
    u32 *stack;
} M509Thread;

typedef struct M509SubThread {
    void **vtbl;
    M509Thread thread;
    M509Sema sema;
    s32 arg;
} M509SubThread;

typedef struct M509Core {
    void **vtbl0;
    void **vtbl;
    s32 winner[4];
    s32 gameMes;
    s32 delay;
} M509Core;

typedef struct M509Cam {
    Vec pos;
    Vec up;
    Vec target;
} M509Cam;

typedef struct M509CamView {
    Vec pos;
    Quaternion rot;
} M509CamView;

typedef struct Camera {
    M509Base base;
    Vec pos;
    Vec up;
    Vec target;
    s32 frame;
    M509Ptmf update;
} Camera;

typedef struct BackItem {
    s32 idx;
    s16 *model;
    Vec pos;
    Vec vel;
    Vec rot;
    Vec rotVel;
    f32 scale;
} BackItem;

typedef struct Back {
    M509Base base;
    u8 mode;
    s16 model[3];
    BackItem item[3];
} Back;

typedef struct BackSky {
    M509Base base;
    f32 spin;
    f32 spinRot;
    Vec pos;
    s16 model;
} BackSky;

typedef struct MazeCell {
    u8 wall;
    u8 mark;
    u8 fade;
    u8 vTimer;
    u8 hTimer;
} MazeCell;

typedef struct MazeWalker {
    u8 state;
    u8 dir;
    u8 unused;
    u8 row;
    u8 col;
} MazeWalker;

typedef struct Maze {
    M509Base base;
    s16 hook[2];
    u8 dirty;
    u32 phase;
    s32 alpha;
    u8 busy;
    s32 busyTimer;
    s32 busyParam;
    s32 timer;
    s32 hold;
    s32 sfx;
    MazeCell cell[16][12];
    MazeWalker walker[3];
    u64 seedSaved;
    u64 seed;
} Maze;

typedef struct PlayerNode {
    PlayerNode *next;
    PlayerNode *prev;
} PlayerNode;

typedef struct PlayerVec {
    u32 cap;
    u32 size;
    s16 *data;
} PlayerVec;

typedef struct Player {
    PlayerNode node;
    M509Ptmf state;
    s32 idx;
    s32 charNo;
    s32 pad;
    s32 unk20;
    s32 motion;
    s32 dif;
    f32 rotY;
    f32 targetRotY;
    f32 speed;
    s32 pushTimer;
    s32 stunTimer;
    s32 act;
    Vec pos;
    Vec prev;
    s32 cellKey;
    u8 cellWalls;
    s32 aiTimer;
    s16 model;
    u8 inGoal;
    u8 blink;
    s32 frame;
    PlayerVec mot;
} Player;

typedef struct PlayerMgr {
    M509Base base;
    s16 model[2];
} PlayerMgr;


typedef struct M509MT {
    u32 state[624];
    s32 index;
} M509MT;

typedef struct M509GameMaster {
    M509Core core;
    M509SubThread sub;
    MGTIMER *timer;
    M509MT mt;
} M509GameMaster;

#endif
