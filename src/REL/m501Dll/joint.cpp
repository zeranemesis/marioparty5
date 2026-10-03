#include <dolphin/mtx.h>
#include "REL/m501Dll.h"

extern "C" {
void __assert_fail(const char *expr, const char *file, int line);
void __dl__FPv(void *ptr);
}

namespace m501 {

class CJoint {
public:
    CJoint();
    ~CJoint();
    Mtx *getMatrix();
    void lockMatrixChain();
    void lockMatrix();
    void setSuper(CJoint *super);
    void setSuperJoint(CJoint *super);
    void resetSuperJoint();

private:
    Mtx m_world;    // 0x00 composed matrix of the whole chain
    Mtx m_local;    // 0x30 this joint's own matrix, the one getMatrix returns
    CJoint *m_super; // 0x60
    CJoint *m_child; // 0x64 tail of the child list
    CJoint *m_next;  // 0x68
    CJoint *m_prev;  // 0x6C
    u8 m_locked;     // 0x70 matrix needs recomputing
};

CJoint::CJoint()
{
    PSMTXIdentity(m_world);

    m_super = (CJoint *)0;
    m_next = this;
    m_prev = this;
    m_child = (CJoint *)0;
    m_locked = 1;
}

CJoint::~CJoint()
{
    resetSuperJoint();

    CJoint *node = m_child;
    if (node != 0) {
        CJoint *last = node;

        do {
            CJoint *next = node->m_next;
            node->m_next = node;
            node->m_prev = node;
            node->m_super = (CJoint *)0;
            node = next;
        } while (node != last);
    }
}

Mtx *CJoint::getMatrix()
{
    if (m_locked) {
        m_locked = 0;

        if (m_super == 0 || m_super == this) {
            PSMTXCopy(m_world, m_local);
        } else {
            Mtx *mtx = m_super->getMatrix();
            PSMTXConcat(*mtx, m_world, m_local);
        }
    }

    return &m_local;
}

void CJoint::lockMatrixChain()
{
    m_locked = 1;

    CJoint *child = m_child;
    if (child != 0) {
        child->lockMatrixChain();
    }
}

void CJoint::lockMatrix()
{
    m_locked = 1;
}

void CJoint::setSuper(CJoint *super)
{
    if ((CJoint *)0 == super) {
        if ((CJoint *)0 == m_super) {
            __assert_fail("m_super != NULL", "joint.cpp", 0x4E);
        }

        Mtx *mtx = m_super->getMatrix();
        PSMTXConcat(*mtx, m_world, m_local);
        resetSuperJoint();
    } else {
        Mtx *mtx = super->getMatrix();
        Mtx inv;

        PSMTXInverse(*mtx, inv);
        PSMTXConcat(inv, m_world, m_local);
        setSuperJoint(super);
    }
}

// NONMATCHING: the original emits the assert block out of line (bne->assert, b->join)
void CJoint::setSuperJoint(CJoint *super)
{
    if (m_super != 0) {
        __assert_fail("m_super == NULL", "joint.cpp", 0x5F);
    }

    m_super = super;

    CJoint *node = super->m_child;
    if (node != 0) {
        m_prev = node;
        m_next = node->m_next;
        node->m_next->m_prev = this;
        node->m_next = this;
    } else {
        super->m_child = this;
    }
}

void CJoint::resetSuperJoint()
{
    CJoint *next = (CJoint *)0;

    if (m_next != this) {
        next = m_next;
        m_next->m_prev = m_prev;
        m_prev->m_next = m_next;
        m_next = this;
        m_prev = this;
    }

    if (m_super != 0) {
        m_super->m_child = next;
        m_super = (CJoint *)0;
    }
}
}