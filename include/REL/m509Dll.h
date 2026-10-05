#ifndef _M509DLL_H
#define _M509DLL_H

#include "dolphin.h"

#include "dolphin/mtx/GeoTypes.h"

extern "C" {
typedef struct PTMF {
    long this_delta;
    long v_offset;
    union {
        void *f_addr;
        long ve_offset;
    } f_data;
} PTMF;

long __ptmf_test(const PTMF *ptmf);
}

/* Doubly linked list node every m509 subsystem is threaded on.
   CM509Node::CM509Node, CM509ActorBase::~CM509ActorBase and
   CM509ActorBase::on_update live in m509.cpp (the module's shared framework). */
class CM509Node {
public:
    CM509Node();
    CM509Node *m_prev;
    CM509Node *m_next;
};

/* Common actor base: vtable at offset 0x8 of every m509 actor. */
class CM509ActorBase : public CM509Node {
public:
    CM509ActorBase() { m_unkC = 0; }
    virtual ~CM509ActorBase();
    virtual void on_close(s32 arg) = 0;
    virtual void on_update();

    s32 m_unkC;
};

/* The four "GO!" name boards shown before the maze starts. */
typedef struct M509Vec2_s {
    float x;
    float y;
} M509Vec2;

typedef struct M509StartSprEntry_s {
    s16 m_grp;
    float m_x;
    float m_y;
} M509StartSprEntry;

class CM509StartSpr : public CM509ActorBase {
public:
    CM509StartSpr(s32 *animIdx);
    virtual ~CM509StartSpr();
    virtual void on_close(s32 arg);
    virtual void on_update();

    u8 m_flag;
    u8 m_pad11[3];
    s32 m_timer;
    float m_pos;
    M509StartSprEntry m_entry[4];
};

/* Camera: follows the players, reached through a singleton pointer. */
class CM509Camera : public CM509ActorBase {
public:
    CM509Camera();
    virtual ~CM509Camera();
    virtual void on_close(s32 arg);
    virtual void on_update();

    Vec m_target;
    Vec m_pos;
    Vec m_up;
    s32 m_counter;
    PTMF m_callback;
};

#endif
