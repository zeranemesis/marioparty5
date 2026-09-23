typedef struct PTMF {
    long this_delta;
    long v_offset;
    union {
        void* f_addr;
        long ve_offset;
    } f_data;
} PTMF;

const PTMF __ptmf_null = { 0, 0, 0 };

/* clang-format off */
asm long __ptmf_test(register const PTMF* ptmf) {
    nofralloc
    lwz r5, 0x0(r3)
    lwz r6, 0x4(r3)
    lwz r7, 0x8(r3)
    li r3, 1
    cmpwi cr0, r5, 0
    cmpwi cr6, r6, 0
    cmpwi cr7, r7, 0
    bnelr cr0
    bnelr cr6
    bnelr cr7
    li r3, 0
    blr
}

asm long __ptmf_cmpr(register const PTMF* ptmf1, register const PTMF* ptmf2) {
    nofralloc
    lwz r5, 0x0(r3)
    lwz r6, 0x0(r4)
    lwz r7, 0x4(r3)
    lwz r8, 0x4(r4)
    lwz r9, 0x8(r3)
    lwz r10, 0x8(r4)
    li r3, 1
    cmpw cr0, r5, r6
    cmpw cr6, r7, r8
    cmpw cr7, r9, r10
    bnelr cr0
    bnelr cr6
    bnelr cr7
    li r3, 0
    blr
}

asm void __ptmf_call(...) {
    nofralloc
    lwz r0, 0x0(r12)
    lwz r11, 0x4(r12)
    lwz r12, 0x8(r12)
    cmpwi r11, 0
    add r3, r3, r0
    blt L1
    lwzx r12, r3, r12
    add r12, r12, r11
    lwz r0, 0x4(r12)
    lwz r12, 0x0(r12)
    add r3, r3, r0
L1:
    mtctr r12
    bctr
}

asm void __ptmf_call4(...) {
    nofralloc
    lwz r0, 0x0(r12)
    lwz r11, 0x4(r12)
    lwz r12, 0x8(r12)
    cmpwi r11, 0
    add r4, r4, r0
    blt L1
    lwzx r12, r4, r12
    add r12, r12, r11
    lwz r0, 0x4(r12)
    lwz r12, 0x0(r12)
    add r4, r4, r0
L1:
    mtctr r12
    bctr
}

asm void __ptmf_scall(...) {
    nofralloc
    lwz r0, 0x0(r12)
    lwz r11, 0x4(r12)
    lwz r12, 0x8(r12)
    add r3, r3, r0
    cmpwi r11, 0
    blt L1
    lwzx r12, r3, r12
    lwzx r12, r12, r11
L1:
    mtctr r12
    bctr
}

asm void __ptmf_scall4(...) {
    nofralloc
    lwz r0, 0x0(r12)
    lwz r11, 0x4(r12)
    lwz r12, 0x8(r12)
    add r4, r4, r0
    cmpwi r11, 0
    blt L1
    lwzx r12, r4, r12
    lwzx r12, r12, r11
L1:
    mtctr r12
    bctr
}
/* clang-format on */

PTMF* __ptmf_cast(long offset, const PTMF* ptmfrom, PTMF* ptmto) {
    ptmto->this_delta = ptmfrom->this_delta + offset;
    ptmto->v_offset = ptmfrom->v_offset;
    ptmto->f_data.f_addr = ptmfrom->f_data.f_addr;
    return ptmto;
}
