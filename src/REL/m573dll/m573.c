#include "game/process.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

extern s32 lbl_1_bss_164;
extern s32 lbl_1_bss_230[4];

void ObjectSetup(void);
void fn_1_5C8(void);
void fn_1_8BC(void);
void fn_1_DAC(void);
void fn_1_EE4(void);
void fn_1_1028(void);
void fn_1_12B8(void);
void fn_1_171C(void);
void fn_1_182C(void);
void fn_1_1A14(void);
void fn_1_1B78(void);
void fn_1_1C3C(void);
void fn_1_1CA4(void);
void fn_1_1DC4(void);
void fn_1_20B4(void);
void fn_1_2260(void);
void fn_1_22EC(void);
void fn_1_22F0(void);
void fn_1_25B4(void);
void fn_1_2690(void);
void fn_1_2814(void);
void fn_1_2850(void);
void fn_1_28E8(void);
void fn_1_29CC(void);
void fn_1_2AE4(void);
void fn_1_2DA0(void);
void fn_1_2F40(void);
void fn_1_3300(void);
void fn_1_3324(void);
void fn_1_33AC(void);
void fn_1_3450(void);
void fn_1_34A8(void);
void fn_1_35BC(void);
void fn_1_3718(void);
void fn_1_37A4(void);
void fn_1_3AD8(void);
void fn_1_3EBC(void);
void fn_1_3FC0(void);
void fn_1_402C(void);
void fn_1_4084(void);
void fn_1_40C0(void);
void fn_1_4120(void);
void fn_1_41E4(void);
void fn_1_42A8(void);
void fn_1_43B0(void);
void fn_1_442C(void);
void fn_1_448C(void);
void fn_1_4614(void);
void fn_1_4794(void);
void fn_1_6BDC(void);
void fn_1_6FE4(void);
void fn_1_7034(void);
void fn_1_7364(void);
void fn_1_7898(void);
void fn_1_78CC(void);
void fn_1_792C(void);
void fn_1_793C(void);
void fn_1_79E4(s32 index, s32 value);
void fn_1_79FC(void);
void fn_1_7AF0(void);
void fn_1_7BC0(void);
void fn_1_7EB4(void);
void fn_1_7FC4(void);
void fn_1_8114(void);
void fn_1_82C0(void);
void fn_1_8374(void);
void fn_1_83C8(void);
void fn_1_83DC(void);
void fn_1_840C(void);
void fn_1_843C(void);
void fn_1_8634(void);
void fn_1_8694(void);
void fn_1_8804(void);
void fn_1_8840(void);
void fn_1_88BC(void);

void fn_1_83C8(void)
{
    lbl_1_bss_164 = -1;
}

void fn_1_79E4(s32 index, s32 value)
{
    lbl_1_bss_230[index] = value;
}