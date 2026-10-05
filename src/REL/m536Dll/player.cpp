#include "types.h"

// m536::Player. fn_1_AA58 and fn_1_D6A4 are byte-identical list-node removers
// that unlink through fn_1_1270 and only then free, and only when the frame
// counter passed in is still positive. fn_1_BB30 re-applies attribute 1 to the
// model id cached at 0x88. fn_1_D084/fn_1_D0E4 are the linked-list walk: the
// outer function just forwards (node->next, node, cb) into the walker.
extern "C" {

struct M536ModelData;

typedef void (*M536ListCallback)(void *);

// Doubly-linked list node: fn_1_1270 swaps 0x00 and 0x04 to unlink, and
// fn_1_D0E4 walks 0x00 until it reaches the sentinel it was handed.
struct M536ListNode {
    M536ListNode *m_next;
    M536ListNode *m_prev;
};

// Only the model id at 0x88 is read out of this one.
struct M536ModelData {
    s8 m_unk0[0x88];
    s16 m_modelId;
};

void fn_1_C8(void *node);
void *fn_1_1270(void *node, s16 frame);
void Hu3DModelAttrSet(s16 modelId, u32 attr);
void fn_1_D0E4(M536ListNode *node, M536ListNode *stop, M536ListCallback cb);

M536ListNode *fn_1_AA58(M536ListNode *node, s16 frame)
{
    if (node != 0) {
        fn_1_1270(node, 0);
        if (frame > 0) {
            fn_1_C8(node);
        }
    }
    return node;
}

void fn_1_BB30(M536ModelData *model)
{
    s16 attr;
    s16 id;

    id = (s16)model->m_modelId;
    attr = id;
    Hu3DModelAttrSet(attr, 1);
}

void fn_1_D084(M536ListNode *node, M536ListCallback cb)
{
    // Three named steps: the original keeps a live copy of the head, the load
    // of head->m_next and a second copy of that load in three separate
    // registers, so the forwarding cannot be collapsed to one expression.
    M536ListNode *head;
    M536ListNode *first;
    M536ListNode *start;

    head = node;
    first = node->m_next;
    start = first;
    fn_1_D0E4(start, head, cb);
}

void fn_1_D0E4(M536ListNode *node, M536ListNode *stop, M536ListCallback cb)
{
    M536ListNode *next;

    while (node != stop) {
        cb(node);
        next = node->m_next;
        node = next;
    }
}

M536ListNode *fn_1_D6A4(M536ListNode *node, s16 frame)
{
    if (node != 0) {
        fn_1_1270(node, 0);
        if (frame > 0) {
            fn_1_C8(node);
        }
    }
    return node;
}
}
