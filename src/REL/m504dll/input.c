#define _MATH_H

#include "REL/m504dll.h"
#include "string.h"

extern GWPLAYERCONF GwPlayerConf[];
extern u16 HuPadBtn[];
extern u16 HuPadBtnDown[];
extern u16 HuPadBtnRep[];
extern s8 HuPadStkX[];
extern s8 HuPadStkY[];
extern s8 HuPadSubStkX[];
extern s8 HuPadSubStkY[];
extern u8 HuPadTrigL[];
extern u8 HuPadTrigR[];
extern u8 HuPadDStk[];
extern u8 HuPadDStkRep[];


extern void fn_1_BC54(INPUTWORK *work);
extern void fn_1_BC84(INPUTWORK *work);
void fn_1_BAC4(INPUTWORK *work, INPUTMAIN *main);

extern char lbl_1_bss_7810[];

extern u32 lbl_1_data_528[];
extern u32 lbl_1_data_538[];
extern u32 lbl_1_data_548[];
extern u32 lbl_1_data_564[];
extern u32 lbl_1_data_574[];
extern u32 lbl_1_data_584[];
extern u32 lbl_1_data_594[];
extern u32 lbl_1_data_5A4[];
extern u32 lbl_1_data_5B4[];
extern u32 lbl_1_data_5C4[];
extern u32 lbl_1_data_5D4[];
extern u32 lbl_1_data_5E4[];
extern u32 lbl_1_data_5F4[];
extern u32 lbl_1_data_604[];
extern u32 lbl_1_data_614[];
extern u32 lbl_1_data_624[];
extern u32 lbl_1_data_634[];
extern u32 lbl_1_data_644[];
extern u32 lbl_1_data_654[];
extern u32 lbl_1_data_670[];
extern u32 lbl_1_data_680[];
extern u32 lbl_1_data_6A8[];
extern u32 lbl_1_data_6B8[];
extern u32 lbl_1_data_6C8[];
extern u32 lbl_1_data_6D8[];
extern u32 lbl_1_data_700[];
extern u32 lbl_1_data_710[];
extern u32 lbl_1_data_720[];
extern u32 lbl_1_data_730[];
extern u32 lbl_1_data_740[];
extern u32 lbl_1_data_750[];
extern u32 lbl_1_data_760[];
extern u32 lbl_1_data_770[];
extern u32 lbl_1_data_780[];
extern u32 lbl_1_data_790[];
extern u32 lbl_1_data_7A0[];
extern u32 lbl_1_data_7B0[];
extern u32 lbl_1_data_7CC[];
extern u32 lbl_1_data_7DC[];
extern u32 lbl_1_data_7EC[];
extern u32 lbl_1_data_7FC[];
extern u32 lbl_1_data_80C[];
extern u32 lbl_1_data_81C[];
extern u32 lbl_1_data_838[];
extern u32 lbl_1_data_848[];
extern u32 lbl_1_data_858[];
extern u32 lbl_1_data_868[];
extern u32 lbl_1_data_890[];
extern u32 lbl_1_data_8A0[];
extern u32 lbl_1_data_8B0[];
extern u32 lbl_1_data_8C0[];
extern u32 lbl_1_data_8D0[];
extern u32 lbl_1_data_8E0[];
extern u32 lbl_1_data_8F0[];
extern u32 lbl_1_data_900[];
extern u32 lbl_1_data_91C[];
extern u32 lbl_1_data_92C[];
extern u32 lbl_1_data_93C[];
extern u32 lbl_1_data_94C[];
extern u32 lbl_1_data_95C[];
extern u32 lbl_1_data_96C[];
extern u32 lbl_1_data_97C[];
extern u32 lbl_1_data_98C[];
extern u32 lbl_1_data_99C[];
extern u32 lbl_1_data_9AC[];
extern u32 lbl_1_data_9BC[];
extern u32 lbl_1_data_9CC[];
extern u32 lbl_1_data_9DC[];
extern u32 lbl_1_data_9EC[];
extern u32 lbl_1_data_9FC[];
extern u32 lbl_1_data_A0C[];
extern u32 lbl_1_data_A1C[];
extern u32 lbl_1_data_A2C[];
extern u32 lbl_1_data_A48[];
extern u32 lbl_1_data_A58[];
extern u32 lbl_1_data_A68[];
extern u32 lbl_1_data_A78[];
extern u32 lbl_1_data_A88[];
extern u32 lbl_1_data_A98[];
extern u32 lbl_1_data_AC0[];
extern u32 lbl_1_data_AD0[];
extern u32 lbl_1_data_AE0[];
extern u32 lbl_1_data_AF0[];
extern u32 lbl_1_data_B00[];
extern u32 lbl_1_data_B10[];
extern u32 lbl_1_data_B20[];
extern u32 lbl_1_data_B30[];
extern u32 lbl_1_data_B40[];
extern u32 lbl_1_data_B50[];
extern u32 lbl_1_data_B78[];
extern u32 lbl_1_data_B88[];
extern u32 lbl_1_data_B98[];
extern u32 lbl_1_data_BA8[];
extern u32 lbl_1_data_BC4[];
extern u32 lbl_1_data_BD4[];
extern u32 lbl_1_data_BE4[];
extern u32 lbl_1_data_BF4[];
extern u32 lbl_1_data_C04[];
extern u32 lbl_1_data_C14[];
extern u32 lbl_1_data_C24[];
extern u32 lbl_1_data_C34[];
extern u32 lbl_1_data_C44[];
extern u32 lbl_1_data_C54[];
extern u32 lbl_1_data_C64[];
extern u32 lbl_1_data_C74[];
extern u32 lbl_1_data_C90[];
extern u32 lbl_1_data_CA0[];
extern u32 lbl_1_data_CB0[];
extern u32 lbl_1_data_CC0[];
extern u32 lbl_1_data_CD0[];
extern u32 lbl_1_data_CE0[];
extern u32 lbl_1_data_CF0[];
extern u32 lbl_1_data_D00[];
extern u32 lbl_1_data_D10[];
extern u32 lbl_1_data_D20[];
extern u32 lbl_1_data_D30[];
extern u32 lbl_1_data_D40[];
extern u32 lbl_1_data_D50[];
extern u32 lbl_1_data_D60[];
extern u32 lbl_1_data_D70[];
extern u32 lbl_1_data_D80[];
extern u32 lbl_1_data_D90[];
extern u32 lbl_1_data_DA0[];
extern u32 lbl_1_data_DB0[];
extern u32 lbl_1_data_DC0[];
extern u32 lbl_1_data_DD0[];
extern u32 lbl_1_data_DE0[];
extern u32 lbl_1_data_DF0[];
extern u32 lbl_1_data_E00[];
extern u32 lbl_1_data_E1C[];
extern u32 lbl_1_data_E2C[];
extern u32 lbl_1_data_E3C[];
extern u32 lbl_1_data_E4C[];
extern u32 lbl_1_data_E74[];
extern u32 lbl_1_data_E84[];
extern u32 lbl_1_data_E94[];
extern u32 lbl_1_data_EA4[];
extern u32 lbl_1_data_EB4[];
extern u32 lbl_1_data_EC4[];
extern u32 lbl_1_data_ED4[];
extern u32 lbl_1_data_EE4[];
extern u32 lbl_1_data_EF4[];
extern u32 lbl_1_data_F04[];
extern u32 lbl_1_data_F14[];
extern u32 lbl_1_data_F24[];
extern u32 lbl_1_data_F34[];
extern u32 lbl_1_data_F44[];
extern u32 lbl_1_data_F54[];
extern u32 lbl_1_data_F64[];
extern u32 lbl_1_data_F74[];
extern u32 lbl_1_data_F84[];
extern u32 lbl_1_data_F94[];
extern u32 lbl_1_data_FA4[];
extern u32 lbl_1_data_FB4[];
extern u32 lbl_1_data_FC4[];
extern u32 lbl_1_data_FD4[];
extern u32 lbl_1_data_FE4[];
extern u32 lbl_1_data_FF4[];
extern u32 lbl_1_data_1004[];
extern u32 lbl_1_data_1014[];
extern u32 lbl_1_data_1024[];
extern u32 lbl_1_data_1034[];
extern u32 lbl_1_data_1050[];
extern u32 lbl_1_data_1078[];
extern u32 lbl_1_data_1088[];
extern u32 lbl_1_data_1098[];
extern u32 lbl_1_data_10A8[];
extern u32 lbl_1_data_10B8[];
extern u32 lbl_1_data_10C8[];
extern u32 lbl_1_data_10D8[];
extern u32 lbl_1_data_10E8[];
extern u32 lbl_1_data_10F8[];
extern u32 lbl_1_data_1108[];
extern u32 lbl_1_data_1118[];
extern u32 lbl_1_data_1128[];
extern u32 lbl_1_data_1138[];
extern u32 lbl_1_data_1148[];
extern u32 lbl_1_data_117C[];
extern u32 lbl_1_data_118C[];
extern u32 lbl_1_data_119C[];
extern u32 lbl_1_data_11AC[];
extern u32 lbl_1_data_11BC[];
extern u32 lbl_1_data_11CC[];
extern u32 lbl_1_data_11DC[];
extern u32 lbl_1_data_11EC[];
extern u32 lbl_1_data_11FC[];
extern u32 lbl_1_data_120C[];
extern u32 lbl_1_data_121C[];
extern u32 lbl_1_data_122C[];
extern u32 lbl_1_data_123C[];
extern u32 lbl_1_data_124C[];
extern u32 lbl_1_data_125C[];
extern u32 lbl_1_data_126C[];
extern u32 lbl_1_data_12A0[];
extern u32 lbl_1_data_12B0[];
extern u32 lbl_1_data_12C0[];
extern u32 lbl_1_data_12D0[];
extern u32 lbl_1_data_12E0[];
extern u32 lbl_1_data_12F0[];
extern u32 lbl_1_data_1300[];
extern u32 lbl_1_data_1310[];
extern u32 lbl_1_data_1350[];
extern u32 lbl_1_data_1360[];
extern u32 lbl_1_data_1370[];
extern u32 lbl_1_data_1380[];
extern u32 lbl_1_data_1390[];
extern u32 lbl_1_data_13A0[];
extern u32 lbl_1_data_13B0[];
extern u32 lbl_1_data_13C0[];
extern u32 lbl_1_data_13D0[];
extern u32 lbl_1_data_13E0[];
extern u32 lbl_1_data_13F0[];
extern u32 lbl_1_data_1400[];
extern u32 lbl_1_data_1428[];
extern u32 lbl_1_data_1438[];
extern u32 lbl_1_data_1448[];
extern u32 lbl_1_data_1458[];
extern u32 lbl_1_data_1468[];
extern u32 lbl_1_data_1478[];
extern u32 lbl_1_data_1488[];
extern u32 lbl_1_data_1498[];
extern u32 lbl_1_data_14A8[];
extern u32 lbl_1_data_14B8[];
extern u32 lbl_1_data_14C8[];
extern u32 lbl_1_data_14D8[];
extern u32 lbl_1_data_14E8[];
extern u32 lbl_1_data_14F8[];
extern u32 lbl_1_data_1520[];
extern u32 lbl_1_data_1530[];
extern u32 lbl_1_data_1540[];
extern u32 lbl_1_data_1550[];
extern u32 lbl_1_data_1560[];
extern u32 lbl_1_data_1570[];
extern u32 lbl_1_data_1580[];
extern u32 lbl_1_data_1590[];
extern u32 lbl_1_data_15A0[];
extern u32 lbl_1_data_15B0[];
extern u32 lbl_1_data_15C0[];
extern u32 lbl_1_data_15D0[];
extern u32 lbl_1_data_15E0[];
extern u32 lbl_1_data_15F0[];
extern u32 lbl_1_data_1600[];
extern u32 lbl_1_data_1610[];
extern u32 lbl_1_data_1620[];
extern u32 lbl_1_data_1630[];
extern u32 lbl_1_data_1640[];
extern u32 lbl_1_data_1650[];
extern u32 lbl_1_data_166C[];
extern u32 lbl_1_data_167C[];
extern u32 lbl_1_data_1698[];
extern u32 lbl_1_data_16A8[];
extern u32 lbl_1_data_16B8[];
extern u32 lbl_1_data_16C8[];
extern u32 lbl_1_data_16D8[];
extern u32 lbl_1_data_16E8[];
extern u32 lbl_1_data_16F8[];
extern u32 lbl_1_data_1708[];
extern u32 lbl_1_data_1718[];
extern u32 lbl_1_data_1728[];
extern u32 lbl_1_data_1738[];
extern u32 lbl_1_data_1748[];
extern u32 lbl_1_data_1758[];
extern u32 lbl_1_data_1768[];
extern u32 lbl_1_data_1778[];
extern u32 lbl_1_data_1788[];
extern u32 lbl_1_data_17A4[];
extern u32 lbl_1_data_17B4[];
extern u32 lbl_1_data_17C4[];
extern u32 lbl_1_data_17D4[];
extern u32 lbl_1_data_17E4[];
extern u32 lbl_1_data_17F4[];
extern u32 lbl_1_data_1804[];
extern u32 lbl_1_data_1814[];
extern u32 lbl_1_data_1824[];
extern u32 lbl_1_data_1834[];
extern u32 lbl_1_data_1844[];
extern u32 lbl_1_data_1854[];
extern u32 lbl_1_data_1864[];
extern u32 lbl_1_data_1874[];
extern u32 lbl_1_data_1884[];
extern u32 lbl_1_data_1894[];
extern u32 lbl_1_data_18A4[];
extern u32 lbl_1_data_18B4[];
extern u32 lbl_1_data_18C4[];
extern u32 lbl_1_data_18D4[];
extern u32 lbl_1_data_18E4[];
extern u32 lbl_1_data_18F4[];
extern u32 lbl_1_data_1904[];
extern u32 lbl_1_data_1914[];
extern u32 lbl_1_data_1924[];
extern u32 lbl_1_data_1934[];
extern u32 lbl_1_data_1944[];
extern u32 lbl_1_data_1954[];
extern u32 lbl_1_data_1964[];
extern u32 lbl_1_data_1974[];
extern u32 lbl_1_data_1984[];
extern u32 lbl_1_data_1994[];
extern u32 lbl_1_data_19B0[];
extern u32 lbl_1_data_19C0[];
extern u32 lbl_1_data_19D0[];
extern u32 lbl_1_data_19E0[];
extern u32 lbl_1_data_19F0[];
extern u32 lbl_1_data_1A00[];
extern u32 lbl_1_data_1A10[];
extern u32 lbl_1_data_1A20[];
extern u32 lbl_1_data_1A6C[];
extern u32 lbl_1_data_1A7C[];
extern u32 lbl_1_data_1A8C[];
extern u32 lbl_1_data_1A9C[];
extern u32 lbl_1_data_1AAC[];
extern u32 lbl_1_data_1ABC[];
extern u32 lbl_1_data_1ACC[];
extern u32 lbl_1_data_1ADC[];
extern u32 lbl_1_data_1AEC[];
extern u32 lbl_1_data_1AFC[];
extern u32 lbl_1_data_1B0C[];
extern u32 lbl_1_data_1B1C[];
extern u32 lbl_1_data_1B2C[];
extern u32 lbl_1_data_1B3C[];
extern u32 lbl_1_data_1B4C[];
extern u32 lbl_1_data_1B5C[];
extern u32 lbl_1_data_1B84[];
extern u32 lbl_1_data_1B94[];
extern u32 lbl_1_data_1BA4[];
extern u32 lbl_1_data_1BB4[];
extern u32 lbl_1_data_1BC4[];
extern u32 lbl_1_data_1BD4[];
extern u32 lbl_1_data_1BE4[];
extern u32 lbl_1_data_1BF4[];
extern u32 lbl_1_data_1C1C[];
extern u32 lbl_1_data_1C2C[];
extern u32 lbl_1_data_1C3C[];
extern u32 lbl_1_data_1C4C[];
extern u32 lbl_1_data_1C5C[];
extern u32 lbl_1_data_1C6C[];
extern u32 lbl_1_data_1C7C[];
extern u32 lbl_1_data_1C8C[];
extern u32 lbl_1_data_1C9C[];
extern u32 lbl_1_data_1CAC[];
extern u32 lbl_1_data_1CBC[];
extern u32 lbl_1_data_1CCC[];
extern u32 lbl_1_data_1CDC[];
extern u32 lbl_1_data_1CEC[];
extern u32 lbl_1_data_1CFC[];
extern u32 lbl_1_data_1D0C[];
extern u32 lbl_1_data_1D40[];
extern u32 lbl_1_data_1D50[];
extern u32 lbl_1_data_1D6C[];
extern u32 lbl_1_data_1D7C[];
extern u32 lbl_1_data_1D8C[];
extern u32 lbl_1_data_1D9C[];
extern u32 lbl_1_data_1DB8[];
extern u32 lbl_1_data_1DC8[];
extern u32 lbl_1_data_1DD8[];
extern u32 lbl_1_data_1DE8[];
extern u32 lbl_1_data_1DF8[];
extern u32 lbl_1_data_1E08[];
extern u32 lbl_1_data_1E18[];
extern u32 lbl_1_data_1E28[];
extern u32 lbl_1_data_1E38[];
extern u32 lbl_1_data_1E48[];
extern u32 lbl_1_data_1E58[];
extern u32 lbl_1_data_1E68[];
extern u32 lbl_1_data_1E78[];
extern u32 lbl_1_data_1E88[];
extern u32 lbl_1_data_1E98[];
extern u32 lbl_1_data_1EA8[];
extern u32 lbl_1_data_1EB8[];
extern u32 lbl_1_data_1EC8[];
extern u32 lbl_1_data_1EF0[];
extern u32 lbl_1_data_1F00[];
extern u32 lbl_1_data_1F10[];
extern u32 lbl_1_data_1F20[];
extern u32 lbl_1_data_1F30[];
extern u32 lbl_1_data_1F40[];
extern u32 lbl_1_data_1F50[];
extern u32 lbl_1_data_1F60[];
extern u32 lbl_1_data_1F70[];
extern u32 lbl_1_data_1F80[];
extern u32 lbl_1_data_1F90[];
extern u32 lbl_1_data_1FA0[];
extern u32 lbl_1_data_1FB0[];
extern u32 lbl_1_data_1FC0[];
extern u32 lbl_1_data_1FDC[];
extern u32 lbl_1_data_1FEC[];
extern u32 lbl_1_data_1FFC[];
extern u32 lbl_1_data_200C[];
extern u32 lbl_1_data_201C[];
extern u32 lbl_1_data_202C[];
extern u32 lbl_1_data_203C[];
extern u32 lbl_1_data_204C[];
extern u32 lbl_1_data_205C[];
extern u32 lbl_1_data_206C[];
extern u32 lbl_1_data_2088[];
extern u32 lbl_1_data_2098[];
extern u32 lbl_1_data_20A8[];
extern u32 lbl_1_data_20B8[];
extern u32 lbl_1_data_20C8[];
extern u32 lbl_1_data_20D8[];
extern u32 lbl_1_data_20E8[];
extern u32 lbl_1_data_20F8[];
extern u32 lbl_1_data_2138[];
extern u32 lbl_1_data_2148[];
extern u32 lbl_1_data_2158[];
extern u32 lbl_1_data_2168[];
extern u32 lbl_1_data_2178[];
extern u32 lbl_1_data_2188[];
extern u32 lbl_1_data_21A4[];
extern u32 lbl_1_data_21B4[];
extern u32 lbl_1_data_21F4[];
extern u32 lbl_1_data_2204[];
extern u32 lbl_1_data_222C[];
extern u32 lbl_1_data_223C[];
extern u32 lbl_1_data_224C[];
extern u32 lbl_1_data_225C[];
extern u32 lbl_1_data_226C[];
extern u32 lbl_1_data_227C[];
extern u32 lbl_1_data_228C[];
extern u32 lbl_1_data_229C[];
extern u32 lbl_1_data_22AC[];
extern u32 lbl_1_data_22BC[];
extern u32 lbl_1_data_22CC[];
extern u32 lbl_1_data_22DC[];
extern u32 lbl_1_data_22EC[];
extern u32 lbl_1_data_22FC[];
extern u32 lbl_1_data_230C[];
extern u32 lbl_1_data_231C[];
extern u32 lbl_1_data_2344[];
extern u32 lbl_1_data_2354[];
extern u32 lbl_1_data_2364[];
extern u32 lbl_1_data_2374[];
extern u32 lbl_1_data_239C[];
extern u32 lbl_1_data_23AC[];
extern u32 lbl_1_data_23D4[];
extern u32 lbl_1_data_23E4[];
extern u32 lbl_1_data_23F4[];
extern u32 lbl_1_data_2404[];
extern u32 lbl_1_data_2414[];
extern u32 lbl_1_data_2424[];
extern u32 lbl_1_data_2434[];
extern u32 lbl_1_data_2444[];
extern u32 lbl_1_data_2454[];
extern u32 lbl_1_data_2464[];
extern u32 lbl_1_data_2474[];
extern u32 lbl_1_data_2484[];
extern u32 lbl_1_data_2494[];
extern u32 lbl_1_data_24A4[];
extern u32 lbl_1_data_24B4[];
extern u32 lbl_1_data_24C4[];
extern u32 lbl_1_data_24D4[];
extern u32 lbl_1_data_24E4[];
extern u32 lbl_1_data_24F4[];
extern u32 lbl_1_data_2504[];
extern u32 lbl_1_data_2514[];
extern u32 lbl_1_data_2524[];
extern u32 lbl_1_data_2534[];
extern u32 lbl_1_data_2544[];
extern u32 lbl_1_data_2554[];
extern u32 lbl_1_data_2564[];
extern u32 lbl_1_data_2574[];
extern u32 lbl_1_data_2584[];
extern u32 lbl_1_data_25A0[];
extern u32 lbl_1_data_25B0[];
extern u32 lbl_1_data_25C0[];
extern u32 lbl_1_data_25D0[];
extern u32 lbl_1_data_25F8[];
extern u32 lbl_1_data_2608[];
extern u32 lbl_1_data_2624[];
extern u32 lbl_1_data_2634[];
extern u32 lbl_1_data_2650[];
extern u32 lbl_1_data_2660[];
extern u32 lbl_1_data_2670[];
extern u32 lbl_1_data_2680[];
extern u32 lbl_1_data_26B4[];
extern u32 lbl_1_data_26C4[];
extern u32 lbl_1_data_26D4[];
extern u32 lbl_1_data_26E4[];
extern u32 lbl_1_data_26F4[];
extern u32 lbl_1_data_2704[];
extern u32 lbl_1_data_2738[];
extern u32 lbl_1_data_2748[];
extern u32 lbl_1_data_2758[];
extern u32 lbl_1_data_2768[];
extern u32 lbl_1_data_2778[];
extern u32 lbl_1_data_2788[];
extern u32 lbl_1_data_2798[];
extern u32 lbl_1_data_27A8[];
extern u32 lbl_1_data_27B8[];
extern u32 lbl_1_data_27C8[];
extern u32 lbl_1_data_27E4[];
extern u32 lbl_1_data_27F4[];
extern u32 lbl_1_data_2804[];
extern u32 lbl_1_data_2814[];
extern u32 lbl_1_data_2824[];
extern u32 lbl_1_data_2834[];
extern u32 lbl_1_data_2844[];
extern u32 lbl_1_data_2854[];
extern u32 lbl_1_data_2864[];
extern u32 lbl_1_data_2874[];
extern u32 lbl_1_data_2890[];
extern u32 lbl_1_data_28A0[];
extern u32 lbl_1_data_28B0[];
extern u32 lbl_1_data_28C0[];
extern u32 lbl_1_data_28D0[];
extern u32 lbl_1_data_28E0[];
extern u32 lbl_1_data_28F0[];
extern u32 lbl_1_data_2900[];
extern u32 lbl_1_data_291C[];
extern u32 lbl_1_data_292C[];
extern u32 lbl_1_data_293C[];
extern u32 lbl_1_data_294C[];
extern u32 lbl_1_data_295C[];
extern u32 lbl_1_data_296C[];
extern u32 lbl_1_data_297C[];
extern u32 lbl_1_data_298C[];
extern u32 lbl_1_data_299C[];
extern u32 lbl_1_data_29AC[];
extern u32 lbl_1_data_29BC[];
extern u32 lbl_1_data_29CC[];
extern u32 lbl_1_data_29DC[];
extern u32 lbl_1_data_29EC[];
extern u32 lbl_1_data_29FC[];
extern u32 lbl_1_data_2A0C[];
extern u32 lbl_1_data_2A34[];
extern u32 lbl_1_data_2A44[];
extern u32 lbl_1_data_2A54[];
extern u32 lbl_1_data_2A64[];
extern u32 lbl_1_data_2A74[];
extern u32 lbl_1_data_2A84[];
extern u32 lbl_1_data_2A94[];
extern u32 lbl_1_data_2AA4[];
extern u32 lbl_1_data_2AB4[];
extern u32 lbl_1_data_2AC4[];
extern u32 lbl_1_data_2AD4[];
extern u32 lbl_1_data_2AE4[];
extern u32 lbl_1_data_2B0C[];
extern u32 lbl_1_data_2B1C[];
extern u32 lbl_1_data_2B2C[];
extern u32 lbl_1_data_2B3C[];
extern u32 lbl_1_data_2B4C[];
extern u32 lbl_1_data_2B5C[];
extern u32 lbl_1_data_2B6C[];
extern u32 lbl_1_data_2B7C[];
extern u32 lbl_1_data_2B98[];
extern u32 lbl_1_data_2BA8[];
extern u32 lbl_1_data_2BB8[];
extern u32 lbl_1_data_2BC8[];
extern u32 lbl_1_data_2BD8[];
extern u32 lbl_1_data_2BE8[];
extern u32 lbl_1_data_2BF8[];
extern u32 lbl_1_data_2C08[];
extern u32 lbl_1_data_2C48[];
extern u32 lbl_1_data_2C58[];
extern u32 lbl_1_data_2C68[];
extern u32 lbl_1_data_2C78[];
extern u32 lbl_1_data_2C88[];
extern u32 lbl_1_data_2C98[];
extern u32 lbl_1_data_2CA8[];
extern u32 lbl_1_data_2CB8[];
extern u32 lbl_1_data_2CC8[];
extern u32 lbl_1_data_2CD8[];
extern u32 lbl_1_data_2CE8[];
extern u32 lbl_1_data_2CF8[];
extern u32 lbl_1_data_2D08[];
extern u32 lbl_1_data_2D18[];
extern u32 lbl_1_data_2D28[];
extern u32 lbl_1_data_2D38[];
extern u32 lbl_1_data_2D54[];
extern u32 lbl_1_data_2D64[];
extern u32 lbl_1_data_2D74[];
extern u32 lbl_1_data_2D84[];
extern u32 lbl_1_data_2D94[];
extern u32 lbl_1_data_2DA4[];
extern u32 lbl_1_data_2DB4[];
extern u32 lbl_1_data_2DC4[];
extern u32 lbl_1_data_2DD4[];
extern u32 lbl_1_data_2DE4[];
extern u32 lbl_1_data_2DF4[];
extern u32 lbl_1_data_2E04[];
extern void *lbl_1_data_2E14[];
extern void *lbl_1_data_2E38[];
extern void *lbl_1_data_2E74[];
extern void *lbl_1_data_2EB0[];
extern u32 lbl_1_data_2ED4[];
extern u32 lbl_1_data_2EE4[];
extern u32 lbl_1_data_2EFC[];
extern u32 lbl_1_data_2F0C[];
extern u32 lbl_1_data_2F20[];
extern u32 lbl_1_data_2F30[];
extern u32 lbl_1_data_2F40[];
extern u32 lbl_1_data_2F50[];
extern u32 lbl_1_data_2F60[];
extern u32 lbl_1_data_2F70[];
extern u32 lbl_1_data_2F88[];
extern u32 lbl_1_data_2F98[];
extern u32 lbl_1_data_2FA8[];
extern u32 lbl_1_data_2FB8[];
extern u32 lbl_1_data_2FC8[];
extern u32 lbl_1_data_2FD8[];
extern u32 lbl_1_data_2FE8[];
extern u32 lbl_1_data_2FF8[];
extern u32 lbl_1_data_3008[];
extern u32 lbl_1_data_3020[];
extern u32 lbl_1_data_3030[];
extern u32 lbl_1_data_3044[];
extern u32 lbl_1_data_3054[];
extern u32 lbl_1_data_3064[];
extern u32 lbl_1_data_3074[];
extern u32 lbl_1_data_3084[];
extern u32 lbl_1_data_3094[];
extern u32 lbl_1_data_30A4[];
extern u32 lbl_1_data_30B4[];
extern u32 lbl_1_data_30C4[];
extern u32 lbl_1_data_30D4[];
extern u32 lbl_1_data_30E4[];
extern u32 lbl_1_data_30F4[];
extern u32 lbl_1_data_3104[];
extern u32 lbl_1_data_3114[];
extern u32 lbl_1_data_3124[];
extern u32 lbl_1_data_3134[];
extern u32 lbl_1_data_3144[];
extern u32 lbl_1_data_3154[];
extern u32 lbl_1_data_3164[];
extern u32 lbl_1_data_3174[];
extern u32 lbl_1_data_3184[];
extern u32 lbl_1_data_3194[];
extern u32 lbl_1_data_31A4[];
extern u32 lbl_1_data_31B4[];
extern u32 lbl_1_data_31C4[];
extern u32 lbl_1_data_31D4[];
extern u32 lbl_1_data_31E4[];
extern u32 lbl_1_data_31F4[];
extern u32 lbl_1_data_3204[];
extern u32 lbl_1_data_3214[];
extern u32 lbl_1_data_3224[];
extern u32 lbl_1_data_3234[];
extern u32 lbl_1_data_3244[];
extern u32 lbl_1_data_3254[];
extern u32 lbl_1_data_3264[];
extern u32 lbl_1_data_3274[];
extern u32 lbl_1_data_3284[];
extern u32 lbl_1_data_3294[];
extern u32 lbl_1_data_32A4[];
extern u32 lbl_1_data_32B4[];
extern u32 lbl_1_data_32C4[];
extern u32 lbl_1_data_32D4[];
extern u32 lbl_1_data_32E4[];
extern u32 lbl_1_data_32F4[];
extern u32 lbl_1_data_3304[];
extern u32 lbl_1_data_3314[];
extern u32 lbl_1_data_3324[];
extern u32 lbl_1_data_3334[];
extern u32 lbl_1_data_3344[];
extern u32 lbl_1_data_3354[];
extern u32 lbl_1_data_3364[];
extern u32 lbl_1_data_3374[];
extern u32 lbl_1_data_3384[];
extern u32 lbl_1_data_3394[];
extern u32 lbl_1_data_33A4[];
extern u32 lbl_1_data_33B4[];
extern u32 lbl_1_data_33C4[];
extern u32 lbl_1_data_33D4[];
extern u32 lbl_1_data_33E4[];
extern u32 lbl_1_data_33F4[];
extern u32 lbl_1_data_3404[];
extern u32 lbl_1_data_3414[];
extern u32 lbl_1_data_3424[];
extern u32 lbl_1_data_3434[];
extern u32 lbl_1_data_3444[];
extern u32 lbl_1_data_3454[];
extern u32 lbl_1_data_3464[];
extern u32 lbl_1_data_3474[];
extern u32 lbl_1_data_3484[];
extern u32 lbl_1_data_3494[];
extern u32 lbl_1_data_34A4[];
extern u32 lbl_1_data_34B4[];
extern u32 lbl_1_data_34C4[];
extern u32 lbl_1_data_34D4[];
extern u32 lbl_1_data_34E4[];
extern u32 lbl_1_data_34F4[];
extern u32 lbl_1_data_3504[];
extern u32 lbl_1_data_3514[];
extern u32 lbl_1_data_3524[];
extern u32 lbl_1_data_3534[];
extern u32 lbl_1_data_3544[];
extern u32 lbl_1_data_3554[];
extern u32 lbl_1_data_3564[];
extern u32 lbl_1_data_3574[];
extern u32 lbl_1_data_3584[];
extern u32 lbl_1_data_3594[];
extern u32 lbl_1_data_35A4[];
extern u32 lbl_1_data_35B4[];
extern u32 lbl_1_data_35C4[];
extern u32 lbl_1_data_35D4[];
extern u32 lbl_1_data_35E4[];
extern u32 lbl_1_data_35F4[];
extern u32 lbl_1_data_3604[];
extern u32 lbl_1_data_3614[];
extern u32 lbl_1_data_3624[];
extern u32 lbl_1_data_3634[];
extern u32 lbl_1_data_3644[];
extern u32 lbl_1_data_3654[];
extern u32 lbl_1_data_3664[];
extern u32 lbl_1_data_3674[];
extern u32 lbl_1_data_3684[];
extern u32 lbl_1_data_3694[];
extern u32 lbl_1_data_36A4[];
extern u32 lbl_1_data_36B4[];
extern u32 lbl_1_data_36C4[];
extern u32 lbl_1_data_36D4[];
extern u32 lbl_1_data_36E4[];
extern u32 lbl_1_data_36F4[];
extern u32 lbl_1_data_3704[];
extern u32 lbl_1_data_3714[];
extern u32 lbl_1_data_3724[];
extern u32 lbl_1_data_3734[];
extern u32 lbl_1_data_3744[];
extern u32 lbl_1_data_3754[];
extern u32 lbl_1_data_3764[];
extern u32 lbl_1_data_3774[];
extern u32 lbl_1_data_3784[];
extern u32 lbl_1_data_3794[];
extern u32 lbl_1_data_37A4[];
extern u32 lbl_1_data_37B4[];
extern u32 lbl_1_data_37C4[];
extern u32 lbl_1_data_37D4[];
extern u32 lbl_1_data_37E4[];
extern u32 lbl_1_data_37F4[];
extern u32 lbl_1_data_3804[];
extern u32 lbl_1_data_3814[];
extern u32 lbl_1_data_3824[];
extern u32 lbl_1_data_3834[];
extern u32 lbl_1_data_3844[];
extern u32 lbl_1_data_3854[];
extern u32 lbl_1_data_3864[];
extern u32 lbl_1_data_3874[];
extern u32 lbl_1_data_3884[];
extern u32 lbl_1_data_3894[];
extern u32 lbl_1_data_38A4[];
extern u32 lbl_1_data_38B4[];
extern u32 lbl_1_data_38C4[];
extern u32 lbl_1_data_38D4[];
extern u32 lbl_1_data_38E4[];
extern u32 lbl_1_data_38F4[];
extern u32 lbl_1_data_3904[];
extern u32 lbl_1_data_3914[];
extern u32 lbl_1_data_3924[];
extern u32 lbl_1_data_3934[];
extern u32 lbl_1_data_3944[];
extern u32 lbl_1_data_3954[];
extern u32 lbl_1_data_3964[];
extern u32 lbl_1_data_3974[];
extern u32 lbl_1_data_3984[];
extern u32 lbl_1_data_3994[];
extern u32 lbl_1_data_39A4[];
extern u32 lbl_1_data_39B4[];
extern u32 lbl_1_data_39C4[];
extern u32 lbl_1_data_39D4[];
extern u32 lbl_1_data_39E4[];
extern u32 lbl_1_data_39F4[];
extern u32 lbl_1_data_3A04[];
extern u32 lbl_1_data_3A14[];
extern u32 lbl_1_data_3A24[];
extern u32 lbl_1_data_3A34[];
extern u32 lbl_1_data_3A44[];
extern u32 lbl_1_data_3A54[];
extern u32 lbl_1_data_3A64[];
extern u32 lbl_1_data_3A74[];
extern u32 lbl_1_data_3A84[];
extern u32 lbl_1_data_3A94[];
extern u32 lbl_1_data_3AA4[];
extern u32 lbl_1_data_3AB4[];
extern u32 lbl_1_data_3AC4[];
extern u32 lbl_1_data_3AD4[];
extern u32 lbl_1_data_3AE4[];
extern u32 lbl_1_data_3AF4[];
extern u32 lbl_1_data_3B04[];
extern u32 lbl_1_data_3B14[];
extern u32 lbl_1_data_3B24[];
extern u32 lbl_1_data_3B38[];
extern u32 lbl_1_data_3B48[];
extern u32 lbl_1_data_3B58[];
extern u32 lbl_1_data_3B68[];
extern u32 lbl_1_data_3B7C[];
extern u32 lbl_1_data_3B8C[];
extern u32 lbl_1_data_3B9C[];
extern u32 lbl_1_data_3BAC[];
extern u32 lbl_1_data_3BBC[];
extern u32 lbl_1_data_3BD0[];
extern u32 lbl_1_data_3BE0[];
extern u32 lbl_1_data_3BF0[];
extern u32 lbl_1_data_3C00[];
extern u32 lbl_1_data_3C10[];
extern u32 lbl_1_data_3C20[];
extern u32 lbl_1_data_3C30[];
extern u32 lbl_1_data_3C40[];
extern u32 lbl_1_data_3C50[];
extern u32 lbl_1_data_3C64[];
extern u32 lbl_1_data_3C74[];
extern u32 lbl_1_data_3C84[];
extern u32 lbl_1_data_3C94[];
extern u32 lbl_1_data_3CA8[];
extern u32 lbl_1_data_3CB8[];
extern u32 lbl_1_data_3CC8[];
extern u32 lbl_1_data_3CD8[];
extern u32 lbl_1_data_3CE8[];
extern u32 lbl_1_data_3CF8[];
extern u32 lbl_1_data_3D08[];
extern u32 lbl_1_data_3D1C[];
extern u32 lbl_1_data_3D2C[];
extern u32 lbl_1_data_3D3C[];
extern u32 lbl_1_data_3D4C[];
extern u32 lbl_1_data_3D5C[];
extern u32 lbl_1_data_3D6C[];
extern u32 lbl_1_data_3D7C[];
extern u32 lbl_1_data_3D90[];
extern u32 lbl_1_data_3DA0[];
extern u32 lbl_1_data_3DB0[];
extern u32 lbl_1_data_3DC0[];
extern u32 lbl_1_data_3DD0[];
extern u32 lbl_1_data_3DE0[];
extern u32 lbl_1_data_3DF0[];
extern u32 lbl_1_data_3E00[];
extern u32 lbl_1_data_3E10[];
extern u32 lbl_1_data_3E20[];
extern u32 lbl_1_data_3E30[];
extern u32 lbl_1_data_3E40[];
extern u32 lbl_1_data_3E50[];
extern u32 lbl_1_data_3E60[];
extern u32 lbl_1_data_3E74[];
extern u32 lbl_1_data_3E84[];
extern u32 lbl_1_data_3E94[];
extern u32 lbl_1_data_3EA4[];
extern u32 lbl_1_data_3EB8[];
extern u32 lbl_1_data_3EC8[];
extern u32 lbl_1_data_3ED8[];
extern u32 lbl_1_data_3EE8[];
extern u32 lbl_1_data_3EF8[];
extern u32 lbl_1_data_3F08[];
extern u32 lbl_1_data_3F18[];
extern u32 lbl_1_data_3F28[];
extern u32 lbl_1_data_3F38[];
extern u32 lbl_1_data_3F48[];
extern u32 lbl_1_data_3F58[];
extern u32 lbl_1_data_3F68[];
extern u32 lbl_1_data_3F78[];
extern u32 lbl_1_data_3F8C[];
extern u32 lbl_1_data_3F9C[];
extern u32 lbl_1_data_3FAC[];
extern u32 lbl_1_data_3FBC[];
extern u32 lbl_1_data_3FCC[];
extern u32 lbl_1_data_3FE0[];
extern u32 lbl_1_data_3FF0[];
extern u32 lbl_1_data_4000[];
extern u32 lbl_1_data_4010[];
extern u32 lbl_1_data_4020[];
extern u32 lbl_1_data_4030[];
extern u32 lbl_1_data_4040[];
extern u32 lbl_1_data_4050[];
extern u32 lbl_1_data_4060[];
extern u32 lbl_1_data_4074[];
extern u32 lbl_1_data_4084[];
extern u32 lbl_1_data_4094[];
extern u32 lbl_1_data_40A4[];
extern u32 lbl_1_data_40B4[];
extern u32 lbl_1_data_40C4[];
extern u32 lbl_1_data_40D4[];
extern u32 lbl_1_data_40E4[];
extern u32 lbl_1_data_40F4[];
extern u32 lbl_1_data_4104[];
extern u32 lbl_1_data_4114[];
extern u32 lbl_1_data_4124[];
extern u8 *lbl_1_data_4134[];
extern u32 gap_04_000045BC_data[];

u32 lbl_1_data_528[4] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
};

u32 lbl_1_data_538[4] = {
    0xFFFF0000, 0x00000000, 0x00000000, 0x00000000,
};

u32 lbl_1_data_548[7] = {
    0x00020000, 0xC1A00000, 0x00000000, 0x43F00000, 0xC3B40000, 0x00000000, 0x440C0000,
};

u32 lbl_1_data_564[4] = {
    0x00010000, 0xC3B40000, 0x00000000, 0x44570000,
};

u32 lbl_1_data_574[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x44820000,
};

u32 lbl_1_data_584[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x44AA0000,
};

u32 lbl_1_data_594[4] = {
    0x00010000, 0x430C0000, 0x00000000, 0x44D5C000,
};

u32 lbl_1_data_5A4[4] = {
    0x00010000, 0xC3340000, 0x00000000, 0x44D5C000,
};

u32 lbl_1_data_5B4[4] = {
    0x00010000, 0xC3B40000, 0x00000000, 0x44AF0000,
};

u32 lbl_1_data_5C4[4] = {
    0x00010000, 0xC3850000, 0x00000000, 0x44E4C000,
};

u32 lbl_1_data_5D4[4] = {
    0x00010000, 0xC3020000, 0x00000000, 0x45069000,
};

u32 lbl_1_data_5E4[4] = {
    0x00010000, 0xC3B40000, 0x00000000, 0x45098000,
};

u32 lbl_1_data_5F4[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x450B1000,
};

u32 lbl_1_data_604[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x45228000,
};

u32 lbl_1_data_614[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x45354000,
};

u32 lbl_1_data_624[4] = {
    0x00010000, 0x42C80000, 0x00000000, 0x45480000,
};

u32 lbl_1_data_634[4] = {
    0x00010000, 0x42A00000, 0x00000000, 0x455AC000,
};

u32 lbl_1_data_644[4] = {
    0x00010000, 0xC3200000, 0x00000000, 0x455AC000,
};

u32 lbl_1_data_654[7] = {
    0x00020000, 0xC1A00000, 0x00000000, 0x43F00000, 0xC3160000, 0x00000000, 0x44110000,
};

u32 lbl_1_data_670[4] = {
    0x00010000, 0xC3B40000, 0x00000000, 0x43FF0000,
};

u32 lbl_1_data_680[10] = {
    0x00030000, 0xC28E0000, 0x00000000, 0x43FF0000, 0xC2700000, 0x00000000, 0x443E0000, 
    0x43B40000, 0x00000000, 0x44610000,
};

u32 lbl_1_data_6A8[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x44AA0000,
};

u32 lbl_1_data_6B8[4] = {
    0x00010000, 0x42700000, 0x00000000, 0x44D5C000,
};

u32 lbl_1_data_6C8[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x44D5C000,
};

u32 lbl_1_data_6D8[10] = {
    0x00030000, 0x43B40000, 0x00000000, 0x45098000, 0xC3340000, 0x00000000, 0x45098000, 
    0xC3AF0000, 0x00000000, 0x44AF0000,
};

u32 lbl_1_data_700[4] = {
    0x00010000, 0xC3850000, 0x00000000, 0x44E4C000,
};

u32 lbl_1_data_710[4] = {
    0x00010000, 0xC3AF0000, 0x00000000, 0x45160000,
};

u32 lbl_1_data_720[4] = {
    0x00010000, 0x425C0000, 0x00000000, 0x450FC000,
};

u32 lbl_1_data_730[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x45214000,
};

u32 lbl_1_data_740[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x45354000,
};

u32 lbl_1_data_750[4] = {
    0x00010000, 0x43700000, 0x00000000, 0x453B8000,
};

u32 lbl_1_data_760[4] = {
    0x00010000, 0xC2480000, 0x00000000, 0x453EA000,
};

u32 lbl_1_data_770[4] = {
    0x00010000, 0xC2480000, 0x00000000, 0x453EA000,
};

u32 lbl_1_data_780[4] = {
    0x00010000, 0xC3960000, 0x00000000, 0x453EA000,
};

u32 lbl_1_data_790[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x455AC000,
};

u32 lbl_1_data_7A0[4] = {
    0x00010000, 0x437A0000, 0x00000000, 0x455AC000,
};

u32 lbl_1_data_7B0[7] = {
    0x00020000, 0xC1A00000, 0x00000000, 0x43F00000, 0xC37F0000, 0x00000000, 0x44278000,
};

u32 lbl_1_data_7CC[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x44278000,
};

u32 lbl_1_data_7DC[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x44480000,
};

u32 lbl_1_data_7EC[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x44AA0000,
};

u32 lbl_1_data_7FC[4] = {
    0x00010000, 0x43800000, 0x00000000, 0x44AA0000,
};

u32 lbl_1_data_80C[4] = {
    0x00010000, 0x42820000, 0x00000000, 0x44AA0000,
};

u32 lbl_1_data_81C[7] = {
    0x00020000, 0x42820000, 0x00000000, 0x44BB8000, 0xC3520000, 0x00000000, 0x44BA4000,
};

u32 lbl_1_data_838[4] = {
    0x00010000, 0xC3850000, 0x00000000, 0x44E4C000,
};

u32 lbl_1_data_848[4] = {
    0x00010000, 0xC2F00000, 0x00000000, 0x45066000,
};

u32 lbl_1_data_858[4] = {
    0x00010000, 0xC3B40000, 0x00000000, 0x45066000,
};

u32 lbl_1_data_868[10] = {
    0x00030000, 0x42DC0000, 0x00000000, 0x4508E000, 0x43520000, 0x00000000, 0x45188000, 
    0x435C0000, 0x00000000, 0x4512E000,
};

u32 lbl_1_data_890[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x45354000,
};

u32 lbl_1_data_8A0[4] = {
    0x00010000, 0x43960000, 0x00000000, 0x454E4000,
};

u32 lbl_1_data_8B0[4] = {
    0x00010000, 0x42340000, 0x00000000, 0x454E4000,
};

u32 lbl_1_data_8C0[4] = {
    0x00010000, 0xC2480000, 0x00000000, 0x453EA000,
};

u32 lbl_1_data_8D0[4] = {
    0x00010000, 0x43960000, 0x00000000, 0x453EA000,
};

u32 lbl_1_data_8E0[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x456D8000,
};

u32 lbl_1_data_8F0[4] = {
    0x00010000, 0xC3340000, 0x00000000, 0x45674000,
};

u32 lbl_1_data_900[7] = {
    0x00020000, 0x43AF0000, 0x00000000, 0x44340000, 0xC38C0000, 0x00000000, 0x442F0000,
};

u32 lbl_1_data_91C[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x446B0000,
};

u32 lbl_1_data_92C[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x447F0000,
};

u32 lbl_1_data_93C[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x44A50000,
};

u32 lbl_1_data_94C[4] = {
    0x00010000, 0x431B0000, 0x00000000, 0x44D48000,
};

u32 lbl_1_data_95C[4] = {
    0x00010000, 0xC28C0000, 0x00000000, 0x44C94000,
};

u32 lbl_1_data_96C[4] = {
    0x00010000, 0xC3480000, 0x00000000, 0x44BA4000,
};

u32 lbl_1_data_97C[4] = {
    0x00010000, 0xC39B0000, 0x00000000, 0x44E4A000,
};

u32 lbl_1_data_98C[4] = {
    0x00010000, 0xC3B40000, 0x00000000, 0x45192000,
};

u32 lbl_1_data_99C[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x451F6000,
};

u32 lbl_1_data_9AC[4] = {
    0x00010000, 0x43520000, 0x00000000, 0x45124000,
};

u32 lbl_1_data_9BC[4] = {
    0x00010000, 0x43870000, 0x00000000, 0x4526E000,
};

u32 lbl_1_data_9CC[4] = {
    0x00010000, 0x43960000, 0x00000000, 0x45502000,
};

u32 lbl_1_data_9DC[4] = {
    0x00010000, 0x428C0000, 0x00000000, 0x454E4000,
};

u32 lbl_1_data_9EC[4] = {
    0x00010000, 0xC2480000, 0x00000000, 0x453EA000,
};

u32 lbl_1_data_9FC[4] = {
    0x00010000, 0xC3960000, 0x00000000, 0x454B2000,
};

u32 lbl_1_data_A0C[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x45674000,
};

u32 lbl_1_data_A1C[4] = {
    0x00010000, 0xC34D0000, 0x00000000, 0x45674000,
};

u32 lbl_1_data_A2C[7] = {
    0x00020000, 0x43AF0000, 0x00000000, 0x44340000, 0xC3570000, 0x00000000, 0x446D8000,
};

u32 lbl_1_data_A48[4] = {
    0x00010000, 0x42B40000, 0x00000000, 0x446D8000,
};

u32 lbl_1_data_A58[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x446D8000,
};

u32 lbl_1_data_A68[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x44A28000,
};

u32 lbl_1_data_A78[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x44DAC000,
};

u32 lbl_1_data_A88[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x44ED8000,
};

u32 lbl_1_data_A98[10] = {
    0x00030000, 0x43B40000, 0x00000000, 0x450B6000, 0xC2DC0000, 0x00000000, 0x450B6000, 
    0xC3340000, 0x00000000, 0x44BE0000,
};

u32 lbl_1_data_AC0[4] = {
    0x00010000, 0xC3340000, 0x00000000, 0x44ED8000,
};

u32 lbl_1_data_AD0[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x4512E000,
};

u32 lbl_1_data_AE0[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x4512E000,
};

u32 lbl_1_data_AF0[4] = {
    0x00010000, 0xC3B40000, 0x00000000, 0x4512E000,
};

u32 lbl_1_data_B00[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x45192000,
};

u32 lbl_1_data_B10[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x452C8000,
};

u32 lbl_1_data_B20[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x4544E000,
};

u32 lbl_1_data_B30[4] = {
    0x00010000, 0x42200000, 0x00000000, 0x455AC000,
};

u32 lbl_1_data_B40[4] = {
    0x00010000, 0xC3480000, 0x00000000, 0x455AC000,
};

u32 lbl_1_data_B50[10] = {
    0x00030000, 0x431B0000, 0x00000000, 0x43B40000, 0xC2C80000, 0x00000000, 0x43B40000, 
    0xC3820000, 0x00000000, 0x44098000,
};

u32 lbl_1_data_B78[4] = {
    0x00010000, 0x43200000, 0x00000000, 0x44520000,
};

u32 lbl_1_data_B88[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x4499C000,
};

u32 lbl_1_data_B98[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x44B2C000,
};

u32 lbl_1_data_BA8[7] = {
    0x00020000, 0x00000000, 0x00000000, 0x44C80000, 0xC3020000, 0x00000000, 0x44E74000,
};

u32 lbl_1_data_BC4[4] = {
    0x00010000, 0xC2E60000, 0x00000000, 0x4512E000,
};

u32 lbl_1_data_BD4[4] = {
    0x00010000, 0xC1F00000, 0x00000000, 0x45200000,
};

u32 lbl_1_data_BE4[4] = {
    0x00010000, 0x435E0000, 0x00000000, 0x4514A000,
};

u32 lbl_1_data_BF4[4] = {
    0x00010000, 0x435E0000, 0x00000000, 0x4514A000,
};

u32 lbl_1_data_C04[4] = {
    0x00010000, 0x43A00000, 0x00000000, 0x452E6000,
};

u32 lbl_1_data_C14[4] = {
    0x00010000, 0x43A00000, 0x00000000, 0x454F8000,
};

u32 lbl_1_data_C24[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x454F8000,
};

u32 lbl_1_data_C34[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x453EA000,
};

u32 lbl_1_data_C44[4] = {
    0x00010000, 0xC39B0000, 0x00000000, 0x4549E000,
};

u32 lbl_1_data_C54[4] = {
    0x00010000, 0xC1200000, 0x00000000, 0x456A6000,
};

u32 lbl_1_data_C64[4] = {
    0x00010000, 0x43480000, 0x00000000, 0x456A6000,
};

u32 lbl_1_data_C74[7] = {
    0x00020000, 0x42480000, 0x00000000, 0x43B20000, 0xC3960000, 0x00000000, 0x43B20000,
};

u32 lbl_1_data_C90[4] = {
    0x00010000, 0xC3200000, 0x00000000, 0x441B0000,
};

u32 lbl_1_data_CA0[4] = {
    0x00010000, 0xC2C80000, 0x00000000, 0x447C8000,
};

u32 lbl_1_data_CB0[4] = {
    0x00010000, 0x43200000, 0x00000000, 0x447C8000,
};

u32 lbl_1_data_CC0[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x447C8000,
};

u32 lbl_1_data_CD0[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x44A28000,
};

u32 lbl_1_data_CE0[4] = {
    0x00010000, 0x430C0000, 0x00000000, 0x44D20000,
};

u32 lbl_1_data_CF0[4] = {
    0x00010000, 0xC2B40000, 0x00000000, 0x44CBC000,
};

u32 lbl_1_data_D00[4] = {
    0x00010000, 0xC3700000, 0x00000000, 0x44BA4000,
};

u32 lbl_1_data_D10[4] = {
    0x00010000, 0xC3700000, 0x00000000, 0x44E88000,
};

u32 lbl_1_data_D20[4] = {
    0x00010000, 0xC39B0000, 0x00000000, 0x44FF0000,
};

u32 lbl_1_data_D30[4] = {
    0x00010000, 0xC3480000, 0x00000000, 0x450CA000,
};

u32 lbl_1_data_D40[4] = {
    0x00010000, 0xC2DC0000, 0x00000000, 0x451F6000,
};

u32 lbl_1_data_D50[4] = {
    0x00010000, 0x42B40000, 0x00000000, 0x45142000,
};

u32 lbl_1_data_D60[4] = {
    0x00010000, 0x430C0000, 0x00000000, 0x4508E000,
};

u32 lbl_1_data_D70[4] = {
    0x00010000, 0x43820000, 0x00000000, 0x451D8000,
};

u32 lbl_1_data_D80[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x45282000,
};

u32 lbl_1_data_D90[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x453D6000,
};

u32 lbl_1_data_DA0[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x45548000,
};

u32 lbl_1_data_DB0[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x45548000,
};

u32 lbl_1_data_DC0[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x453EA000,
};

u32 lbl_1_data_DD0[4] = {
    0x00010000, 0xC3A50000, 0x00000000, 0x453EA000,
};

u32 lbl_1_data_DE0[4] = {
    0x00010000, 0xC1A00000, 0x00000000, 0x455B6000,
};

u32 lbl_1_data_DF0[4] = {
    0x00010000, 0x438C0000, 0x00000000, 0x455B6000,
};

u32 lbl_1_data_E00[7] = {
    0x00020000, 0xC1A00000, 0x00000000, 0x43A00000, 0xC3B40000, 0x00000000, 0x44318000,
};

u32 lbl_1_data_E1C[4] = {
    0x00010000, 0xC3B40000, 0x00000000, 0x449C4000,
};

u32 lbl_1_data_E2C[4] = {
    0x00010000, 0xC3B40000, 0x00000000, 0x449C4000,
};

u32 lbl_1_data_E3C[4] = {
    0x00010000, 0xC3B40000, 0x00000000, 0x44AF0000,
};

u32 lbl_1_data_E4C[10] = {
    0x00030000, 0xC3B40000, 0x00000000, 0x44C1C000, 0x42C80000, 0x00000000, 0x44C1C000, 
    0x42C80000, 0x00000000, 0x446D8000,
};

u32 lbl_1_data_E74[4] = {
    0x00010000, 0x42C80000, 0x00000000, 0x449C4000,
};

u32 lbl_1_data_E84[4] = {
    0x00010000, 0x42C80000, 0x00000000, 0x44B18000,
};

u32 lbl_1_data_E94[4] = {
    0x00010000, 0x42C80000, 0x00000000, 0x44D0C000,
};

u32 lbl_1_data_EA4[4] = {
    0x00010000, 0x42C80000, 0x00000000, 0x44D0C000,
};

u32 lbl_1_data_EB4[4] = {
    0x00010000, 0xC3160000, 0x00000000, 0x44D0C000,
};

u32 lbl_1_data_EC4[4] = {
    0x00010000, 0xC3B40000, 0x00000000, 0x44C1C000,
};

u32 lbl_1_data_ED4[4] = {
    0x00010000, 0xC33E0000, 0x00000000, 0x44ED8000,
};

u32 lbl_1_data_EE4[4] = {
    0x00010000, 0xC2200000, 0x00000000, 0x44FC8000,
};

u32 lbl_1_data_EF4[4] = {
    0x00010000, 0xC0800000, 0x00000000, 0x45138000,
};

u32 lbl_1_data_F04[4] = {
    0x00010000, 0xC2200000, 0x00000000, 0x45200000,
};

u32 lbl_1_data_F14[4] = {
    0x00010000, 0x430C0000, 0x00000000, 0x45200000,
};

u32 lbl_1_data_F24[4] = {
    0x00010000, 0x43520000, 0x00000000, 0x45124000,
};

u32 lbl_1_data_F34[4] = {
    0x00010000, 0x43910000, 0x00000000, 0x45296000,
};

u32 lbl_1_data_F44[4] = {
    0x00010000, 0x43938000, 0x00000000, 0x45480000,
};

u32 lbl_1_data_F54[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x45480000,
};

u32 lbl_1_data_F64[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x453D6000,
};

u32 lbl_1_data_F74[4] = {
    0x00010000, 0xC3A50000, 0x00000000, 0x454E4000,
};

u32 lbl_1_data_F84[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x45656000,
};

u32 lbl_1_data_F94[4] = {
    0x00010000, 0xC3960000, 0x00000000, 0x45548000,
};

u32 lbl_1_data_FA4[4] = {
    0x00010000, 0x42480000, 0x00000000, 0x43B40000,
};

u32 lbl_1_data_FB4[4] = {
    0x00010000, 0xC35C0000, 0x00000000, 0x43B40000,
};

u32 lbl_1_data_FC4[4] = {
    0x00010000, 0xC35C0000, 0x00000000, 0x43B40000,
};

u32 lbl_1_data_FD4[4] = {
    0x00010000, 0xC35C0000, 0x00000000, 0x44188000,
};

u32 lbl_1_data_FE4[4] = {
    0x00010000, 0xC2700000, 0x00000000, 0x447C8000,
};

u32 lbl_1_data_FF4[4] = {
    0x00010000, 0x43960000, 0x00000000, 0x447C8000,
};

u32 lbl_1_data_1004[4] = {
    0x00010000, 0x43960000, 0x00000000, 0x447C8000,
};

u32 lbl_1_data_1014[4] = {
    0x00010000, 0x43960000, 0x00000000, 0x44A00000,
};

u32 lbl_1_data_1024[4] = {
    0x00010000, 0x43200000, 0x00000000, 0x44CD0000,
};

u32 lbl_1_data_1034[7] = {
    0x00020000, 0x43B40000, 0x00000000, 0x44CD0000, 0x43B40000, 0x00000000, 0x44E38000,
};

u32 lbl_1_data_1050[10] = {
    0x00030000, 0x43B40000, 0x00000000, 0x450AC000, 0xC37A0000, 0x00000000, 0x450AC000, 
    0xC37A0000, 0x00000000, 0x44C6C000,
};

u32 lbl_1_data_1078[4] = {
    0x00010000, 0xC37A0000, 0x00000000, 0x44E88000,
};

u32 lbl_1_data_1088[4] = {
    0x00010000, 0xC2480000, 0x00000000, 0x45020000,
};

u32 lbl_1_data_1098[4] = {
    0x00010000, 0xC2480000, 0x00000000, 0x45110000,
};

u32 lbl_1_data_10A8[4] = {
    0x00010000, 0xC3B40000, 0x00000000, 0x45110000,
};

u32 lbl_1_data_10B8[4] = {
    0x00010000, 0xC3020000, 0x00000000, 0x451C4000,
};

u32 lbl_1_data_10C8[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x4526E000,
};

u32 lbl_1_data_10D8[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x45480000,
};

u32 lbl_1_data_10E8[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x454E4000,
};

u32 lbl_1_data_10F8[4] = {
    0x00010000, 0xC2480000, 0x00000000, 0x454E4000,
};

u32 lbl_1_data_1108[4] = {
    0x00010000, 0xC2480000, 0x00000000, 0x454E4000,
};

u32 lbl_1_data_1118[4] = {
    0x00010000, 0xC2480000, 0x00000000, 0x455D4000,
};

u32 lbl_1_data_1128[4] = {
    0x00010000, 0x43020000, 0x00000000, 0x45674000,
};

u32 lbl_1_data_1138[4] = {
    0x00010000, 0x43020000, 0x00000000, 0x454E4000,
};

u32 lbl_1_data_1148[13] = {
    0x00040000, 0x43B40000, 0x00000000, 0x43960000, 0x43B40000, 0x00000000, 0x442F0000, 
    0xC3B40000, 0x00000000, 0x442F0000, 0xC3B40000, 0x00000000, 0x43A50000,
};

u32 lbl_1_data_117C[4] = {
    0x00010000, 0xC3B40000, 0x00000000, 0x44228000,
};

u32 lbl_1_data_118C[4] = {
    0x00010000, 0xC3340000, 0x00000000, 0x447C8000,
};

u32 lbl_1_data_119C[4] = {
    0x00010000, 0x42B40000, 0x00000000, 0x447C8000,
};

u32 lbl_1_data_11AC[4] = {
    0x00010000, 0x435C0000, 0x00000000, 0x447C8000,
};

u32 lbl_1_data_11BC[4] = {
    0x00010000, 0x435C0000, 0x00000000, 0x44A28000,
};

u32 lbl_1_data_11CC[4] = {
    0x00010000, 0x43160000, 0x00000000, 0x44D48000,
};

u32 lbl_1_data_11DC[4] = {
    0x00010000, 0xC2DC0000, 0x00000000, 0x44D48000,
};

u32 lbl_1_data_11EC[4] = {
    0x00010000, 0xC3870000, 0x00000000, 0x44BA4000,
};

u32 lbl_1_data_11FC[4] = {
    0x00010000, 0xC3870000, 0x00000000, 0x44DE8000,
};

u32 lbl_1_data_120C[4] = {
    0x00010000, 0xC3870000, 0x00000000, 0x44FA0000,
};

u32 lbl_1_data_121C[4] = {
    0x00010000, 0xC3870000, 0x00000000, 0x45138000,
};

u32 lbl_1_data_122C[4] = {
    0x00010000, 0xC25C0000, 0x00000000, 0x45200000,
};

u32 lbl_1_data_123C[4] = {
    0x00010000, 0x42F00000, 0x00000000, 0x450AC000,
};

u32 lbl_1_data_124C[4] = {
    0x00010000, 0x42F00000, 0x00000000, 0x450AC000,
};

u32 lbl_1_data_125C[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x450AC000,
};

u32 lbl_1_data_126C[13] = {
    0x00040000, 0x43B40000, 0x00000000, 0x44CD0000, 0xC3160000, 0x00000000, 0x44CD0000, 
    0xC3160000, 0x00000000, 0x451A6000, 0x43B40000, 0x00000000, 0x451A6000,
};

u32 lbl_1_data_12A0[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x45354000,
};

u32 lbl_1_data_12B0[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x454E4000,
};

u32 lbl_1_data_12C0[4] = {
    0x00010000, 0xC2C80000, 0x00000000, 0x454E4000,
};

u32 lbl_1_data_12D0[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x454B2000,
};

u32 lbl_1_data_12E0[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x45624000,
};

u32 lbl_1_data_12F0[4] = {
    0x00010000, 0x42DC0000, 0x00000000, 0x45624000,
};

u32 lbl_1_data_1300[4] = {
    0x00010000, 0xC33E0000, 0x00000000, 0x45624000,
};

u32 lbl_1_data_1310[16] = {
    0x00050000, 0x43B40000, 0x00000000, 0x43A50000, 0x43B40000, 0x00000000, 0x44408000, 
    0x43200000, 0x00000000, 0x44520000, 0xC2700000, 0x00000000, 0x444A8000, 0xC3480000, 
    0x00000000, 0x44098000,
};

u32 lbl_1_data_1350[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x44520000,
};

u32 lbl_1_data_1360[4] = {
    0x00010000, 0x43820000, 0x00000000, 0x44548000,
};

u32 lbl_1_data_1370[4] = {
    0x00010000, 0x43820000, 0x00000000, 0x44898000,
};

u32 lbl_1_data_1380[4] = {
    0x00010000, 0x43200000, 0x00000000, 0x44D20000,
};

u32 lbl_1_data_1390[4] = {
    0x00010000, 0xC2F00000, 0x00000000, 0x44D20000,
};

u32 lbl_1_data_13A0[4] = {
    0x00010000, 0xC3820000, 0x00000000, 0x44BA4000,
};

u32 lbl_1_data_13B0[4] = {
    0x00010000, 0xC3820000, 0x00000000, 0x44E10000,
};

u32 lbl_1_data_13C0[4] = {
    0x00010000, 0xC3820000, 0x00000000, 0x451B0000,
};

u32 lbl_1_data_13D0[4] = {
    0x00010000, 0xC38C0000, 0x00000000, 0x45368000,
};

u32 lbl_1_data_13E0[4] = {
    0x00010000, 0xC38C0000, 0x00000000, 0x45368000,
};

u32 lbl_1_data_13F0[4] = {
    0x00010000, 0xC3020000, 0x00000000, 0x4544E000,
};

u32 lbl_1_data_1400[10] = {
    0x00030000, 0x43660000, 0x00000000, 0x4544E000, 0x43660000, 0x00000000, 0x45200000, 
    0x42480000, 0x00000000, 0x45098000,
};

u32 lbl_1_data_1428[4] = {
    0x00010000, 0x42480000, 0x00000000, 0x451EC000,
};

u32 lbl_1_data_1438[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x45228000,
};

u32 lbl_1_data_1448[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x453FE000,
};

u32 lbl_1_data_1458[4] = {
    0x00010000, 0xC2700000, 0x00000000, 0x453FE000,
};

u32 lbl_1_data_1468[4] = {
    0x00010000, 0x43340000, 0x00000000, 0x4544E000,
};

u32 lbl_1_data_1478[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x454E4000,
};

u32 lbl_1_data_1488[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x454E4000,
};

u32 lbl_1_data_1498[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x453EA000,
};

u32 lbl_1_data_14A8[4] = {
    0x00010000, 0xC3910000, 0x00000000, 0x45494000,
};

u32 lbl_1_data_14B8[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x45548000,
};

u32 lbl_1_data_14C8[4] = {
    0x00010000, 0x43AF0000, 0x00000000, 0x455AC000,
};

u32 lbl_1_data_14D8[4] = {
    0x00010000, 0x435C0000, 0x00000000, 0x437A0000,
};

u32 lbl_1_data_14E8[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x43FA0000,
};

u32 lbl_1_data_14F8[10] = {
    0x00030000, 0x43B40000, 0x00000000, 0x442F0000, 0xC3200000, 0x00000000, 0x442F0000, 
    0xC3200000, 0x00000000, 0x43960000,
};

u32 lbl_1_data_1520[4] = {
    0x00010000, 0xC3200000, 0x00000000, 0x44228000,
};

u32 lbl_1_data_1530[4] = {
    0x00010000, 0xC3200000, 0x00000000, 0x44610000,
};

u32 lbl_1_data_1540[4] = {
    0x00010000, 0x43160000, 0x00000000, 0x44610000,
};

u32 lbl_1_data_1550[4] = {
    0x00010000, 0x43960000, 0x00000000, 0x44610000,
};

u32 lbl_1_data_1560[4] = {
    0x00010000, 0x43960000, 0x00000000, 0x44A28000,
};

u32 lbl_1_data_1570[4] = {
    0x00010000, 0x428C0000, 0x00000000, 0x44AB4000,
};

u32 lbl_1_data_1580[4] = {
    0x00010000, 0x430C0000, 0x00000000, 0x44C58000,
};

u32 lbl_1_data_1590[4] = {
    0x00010000, 0x430C0000, 0x00000000, 0x44D34000,
};

u32 lbl_1_data_15A0[4] = {
    0x00010000, 0xC2B40000, 0x00000000, 0x44D34000,
};

u32 lbl_1_data_15B0[4] = {
    0x00010000, 0xC3340000, 0x00000000, 0x44BA4000,
};

u32 lbl_1_data_15C0[4] = {
    0x00010000, 0xC3340000, 0x00000000, 0x44E38000,
};

u32 lbl_1_data_15D0[4] = {
    0x00010000, 0xC3910000, 0x00000000, 0x45070000,
};

u32 lbl_1_data_15E0[4] = {
    0x00010000, 0xC2DC0000, 0x00000000, 0x450FC000,
};

u32 lbl_1_data_15F0[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x452DC000,
};

u32 lbl_1_data_1600[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x453D6000,
};

u32 lbl_1_data_1610[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x453D6000,
};

u32 lbl_1_data_1620[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x453D6000,
};

u32 lbl_1_data_1630[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x453D6000,
};

u32 lbl_1_data_1640[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x453D6000,
};

u32 lbl_1_data_1650[7] = {
    0x00020000, 0x43B40000, 0x00000000, 0x45566000, 0x00000000, 0x00000000, 0x45566000,
};

u32 lbl_1_data_166C[4] = {
    0x00010000, 0xC3A50000, 0x00000000, 0x45566000,
};

u32 lbl_1_data_167C[7] = {
    0x00020000, 0x42A00000, 0x00000000, 0x43AF0000, 0xC3020000, 0x00000000, 0x43AF0000,
};

u32 lbl_1_data_1698[4] = {
    0x00010000, 0xC3B40000, 0x00000000, 0x43AF0000,
};

u32 lbl_1_data_16A8[4] = {
    0x00010000, 0x428C0000, 0x00000000, 0x44368000,
};

u32 lbl_1_data_16B8[4] = {
    0x00010000, 0x428C0000, 0x00000000, 0x44898000,
};

u32 lbl_1_data_16C8[4] = {
    0x00010000, 0x428C0000, 0x00000000, 0x4499C000,
};

u32 lbl_1_data_16D8[4] = {
    0x00010000, 0x43960000, 0x00000000, 0x44AC8000,
};

u32 lbl_1_data_16E8[4] = {
    0x00010000, 0x43520000, 0x00000000, 0x44D5C000,
};

u32 lbl_1_data_16F8[4] = {
    0x00010000, 0xC2700000, 0x00000000, 0x44D5C000,
};

u32 lbl_1_data_1708[4] = {
    0x00010000, 0xC3480000, 0x00000000, 0x44BA4000,
};

u32 lbl_1_data_1718[4] = {
    0x00010000, 0xC3480000, 0x00000000, 0x44EB0000,
};

u32 lbl_1_data_1728[4] = {
    0x00010000, 0xC39B0000, 0x00000000, 0x44EB0000,
};

u32 lbl_1_data_1738[4] = {
    0x00010000, 0xC39B0000, 0x00000000, 0x45052000,
};

u32 lbl_1_data_1748[4] = {
    0x00010000, 0xC3160000, 0x00000000, 0x4517E000,
};

u32 lbl_1_data_1758[4] = {
    0x00010000, 0x42B40000, 0x00000000, 0x4517E000,
};

u32 lbl_1_data_1768[4] = {
    0x00010000, 0x435C0000, 0x00000000, 0x45160000,
};

u32 lbl_1_data_1778[4] = {
    0x00010000, 0x435C0000, 0x00000000, 0x45098000,
};

u32 lbl_1_data_1788[7] = {
    0x00020000, 0x43B40000, 0x00000000, 0x45098000, 0x43B40000, 0x00000000, 0x452D2000,
};

u32 lbl_1_data_17A4[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x453FE000,
};

u32 lbl_1_data_17B4[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x453FE000,
};

u32 lbl_1_data_17C4[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x453FE000,
};

u32 lbl_1_data_17D4[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x453FE000,
};

u32 lbl_1_data_17E4[4] = {
    0x00010000, 0xC3B40000, 0x00000000, 0x454A8000,
};

u32 lbl_1_data_17F4[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x4558E000,
};

u32 lbl_1_data_1804[4] = {
    0x00010000, 0xC3910000, 0x00000000, 0x4558E000,
};

u32 lbl_1_data_1814[4] = {
    0x00010000, 0x41F00000, 0x00000000, 0x43B40000,
};

u32 lbl_1_data_1824[4] = {
    0x00010000, 0xC35C0000, 0x00000000, 0x43B40000,
};

u32 lbl_1_data_1834[4] = {
    0x00010000, 0xC35C0000, 0x00000000, 0x43B40000,
};

u32 lbl_1_data_1844[4] = {
    0x00010000, 0xC35C0000, 0x00000000, 0x44228000,
};

u32 lbl_1_data_1854[4] = {
    0x00010000, 0xC35C0000, 0x00000000, 0x44228000,
};

u32 lbl_1_data_1864[4] = {
    0x00010000, 0x42F00000, 0x00000000, 0x444F8000,
};

u32 lbl_1_data_1874[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x44688000,
};

u32 lbl_1_data_1884[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x449EC000,
};

u32 lbl_1_data_1894[4] = {
    0x00010000, 0x428C0000, 0x00000000, 0x449EC000,
};

u32 lbl_1_data_18A4[4] = {
    0x00010000, 0x43A00000, 0x00000000, 0x44B90000,
};

u32 lbl_1_data_18B4[4] = {
    0x00010000, 0x43160000, 0x00000000, 0x44D5C000,
};

u32 lbl_1_data_18C4[4] = {
    0x00010000, 0xC2B40000, 0x00000000, 0x44D5C000,
};

u32 lbl_1_data_18D4[4] = {
    0x00010000, 0xC39B0000, 0x00000000, 0x44C30000,
};

u32 lbl_1_data_18E4[4] = {
    0x00010000, 0xC3660000, 0x00000000, 0x44F00000,
};

u32 lbl_1_data_18F4[4] = {
    0x00010000, 0xC2200000, 0x00000000, 0x44FB4000,
};

u32 lbl_1_data_1904[4] = {
    0x00010000, 0xC3660000, 0x00000000, 0x45066000,
};

u32 lbl_1_data_1914[4] = {
    0x00010000, 0xC3B40000, 0x00000000, 0x45110000,
};

u32 lbl_1_data_1924[4] = {
    0x00010000, 0xC30C0000, 0x00000000, 0x451C4000,
};

u32 lbl_1_data_1934[4] = {
    0x00010000, 0x435C0000, 0x00000000, 0x45142000,
};

u32 lbl_1_data_1944[4] = {
    0x00010000, 0x435C0000, 0x00000000, 0x4523C000,
};

u32 lbl_1_data_1954[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x45318000,
};

u32 lbl_1_data_1964[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x45444000,
};

u32 lbl_1_data_1974[4] = {
    0x00010000, 0x428C0000, 0x00000000, 0x455AC000,
};

u32 lbl_1_data_1984[4] = {
    0x00010000, 0xC33E0000, 0x00000000, 0x455AC000,
};

u32 lbl_1_data_1994[7] = {
    0x00020000, 0x41A00000, 0x00000000, 0x43B40000, 0xC3B40000, 0x00000000, 0x44098000,
};

u32 lbl_1_data_19B0[4] = {
    0x00010000, 0xC35C0000, 0x00000000, 0x44660000,
};

u32 lbl_1_data_19C0[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x447F0000,
};

u32 lbl_1_data_19D0[4] = {
    0x00010000, 0x43200000, 0x00000000, 0x4499C000,
};

u32 lbl_1_data_19E0[4] = {
    0x00010000, 0x43200000, 0x00000000, 0x44CD0000,
};

u32 lbl_1_data_19F0[4] = {
    0x00010000, 0xC28C0000, 0x00000000, 0x44CD0000,
};

u32 lbl_1_data_1A00[4] = {
    0x00010000, 0xC3AF0000, 0x00000000, 0x44C80000,
};

u32 lbl_1_data_1A10[4] = {
    0x00010000, 0xC3AF0000, 0x00000000, 0x44AA0000,
};

u32 lbl_1_data_1A20[19] = {
    0x00060000, 0xC3AF0000, 0x00000000, 0x44AA0000, 0xC3AF0000, 0x00000000, 0x44750000, 
    0x42B40000, 0x00000000, 0x44750000, 0x42B40000, 0x00000000, 0x44CBC000, 0xC28C0000, 
    0x00000000, 0x44CBC000, 0xC28C0000, 0x00000000, 0x4503E000,
};

u32 lbl_1_data_1A6C[4] = {
    0x00010000, 0xC28C0000, 0x00000000, 0x45160000,
};

u32 lbl_1_data_1A7C[4] = {
    0x00010000, 0xC3B40000, 0x00000000, 0x45160000,
};

u32 lbl_1_data_1A8C[4] = {
    0x00010000, 0xC2A00000, 0x00000000, 0x451E2000,
};

u32 lbl_1_data_1A9C[4] = {
    0x00010000, 0x43570000, 0x00000000, 0x45138000,
};

u32 lbl_1_data_1AAC[4] = {
    0x00010000, 0x43570000, 0x00000000, 0x45228000,
};

u32 lbl_1_data_1ABC[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x452C8000,
};

u32 lbl_1_data_1ACC[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x453F4000,
};

u32 lbl_1_data_1ADC[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x454E4000,
};

u32 lbl_1_data_1AEC[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x454E4000,
};

u32 lbl_1_data_1AFC[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x45674000,
};

u32 lbl_1_data_1B0C[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x4573C000,
};

u32 lbl_1_data_1B1C[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x454E4000,
};

u32 lbl_1_data_1B2C[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x453E0000,
};

u32 lbl_1_data_1B3C[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x455AC000,
};

u32 lbl_1_data_1B4C[4] = {
    0x00010000, 0x43960000, 0x00000000, 0x454E4000,
};

u32 lbl_1_data_1B5C[10] = {
    0x00030000, 0x43B40000, 0x00000000, 0x43B40000, 0x43B40000, 0x00000000, 0x442A0000, 
    0xC3960000, 0x00000000, 0x442A0000,
};

u32 lbl_1_data_1B84[4] = {
    0x00010000, 0xC2C80000, 0x00000000, 0x44728000,
};

u32 lbl_1_data_1B94[4] = {
    0x00010000, 0xC2C80000, 0x00000000, 0x44728000,
};

u32 lbl_1_data_1BA4[4] = {
    0x00010000, 0x43480000, 0x00000000, 0x44728000,
};

u32 lbl_1_data_1BB4[4] = {
    0x00010000, 0x43960000, 0x00000000, 0x44728000,
};

u32 lbl_1_data_1BC4[4] = {
    0x00010000, 0x43960000, 0x00000000, 0x44988000,
};

u32 lbl_1_data_1BD4[4] = {
    0x00010000, 0x43960000, 0x00000000, 0x44CA8000,
};

u32 lbl_1_data_1BE4[4] = {
    0x00010000, 0x43960000, 0x00000000, 0x44F14000,
};

u32 lbl_1_data_1BF4[10] = {
    0x00030000, 0x43960000, 0x00000000, 0x450B6000, 0xC3340000, 0x00000000, 0x450B6000, 
    0xC3340000, 0x00000000, 0x44CA8000,
};

u32 lbl_1_data_1C1C[4] = {
    0x00010000, 0xC3340000, 0x00000000, 0x44EB0000,
};

u32 lbl_1_data_1C2C[4] = {
    0x00010000, 0xC2200000, 0x00000000, 0x4503E000,
};

u32 lbl_1_data_1C3C[4] = {
    0x00010000, 0xC2200000, 0x00000000, 0x45124000,
};

u32 lbl_1_data_1C4C[4] = {
    0x00010000, 0xC3660000, 0x00000000, 0x45174000,
};

u32 lbl_1_data_1C5C[4] = {
    0x00010000, 0x41F00000, 0x00000000, 0x451D8000,
};

u32 lbl_1_data_1C6C[4] = {
    0x00010000, 0x41F00000, 0x00000000, 0x450F2000,
};

u32 lbl_1_data_1C7C[4] = {
    0x00010000, 0x43700000, 0x00000000, 0x451C4000,
};

u32 lbl_1_data_1C8C[4] = {
    0x00010000, 0x43AF0000, 0x00000000, 0x45296000,
};

u32 lbl_1_data_1C9C[4] = {
    0x00010000, 0x43AF0000, 0x00000000, 0x45430000,
};

u32 lbl_1_data_1CAC[4] = {
    0x00010000, 0x43AF0000, 0x00000000, 0x45430000,
};

u32 lbl_1_data_1CBC[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x453EA000,
};

u32 lbl_1_data_1CCC[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x453EA000,
};

u32 lbl_1_data_1CDC[4] = {
    0x00010000, 0xC3480000, 0x00000000, 0x45520000,
};

u32 lbl_1_data_1CEC[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x45656000,
};

u32 lbl_1_data_1CFC[4] = {
    0x00010000, 0x43820000, 0x00000000, 0x45548000,
};

u32 lbl_1_data_1D0C[13] = {
    0x00040000, 0x43B40000, 0x00000000, 0x43B40000, 0x43B40000, 0x00000000, 0x442A0000, 
    0xC3700000, 0x00000000, 0x442A0000, 0xC3700000, 0x00000000, 0x43DC0000,
};

u32 lbl_1_data_1D40[4] = {
    0x00010000, 0xC3700000, 0x00000000, 0x44598000,
};

u32 lbl_1_data_1D50[7] = {
    0x00020000, 0x43480000, 0x00000000, 0x44598000, 0x43480000, 0x00000000, 0x44848000,
};

u32 lbl_1_data_1D6C[4] = {
    0x00010000, 0x43480000, 0x00000000, 0x44B68000,
};

u32 lbl_1_data_1D7C[4] = {
    0x00010000, 0xC3700000, 0x00000000, 0x44D70000,
};

u32 lbl_1_data_1D8C[4] = {
    0x00010000, 0xC3700000, 0x00000000, 0x44EB0000,
};

u32 lbl_1_data_1D9C[7] = {
    0x00020000, 0xC3700000, 0x00000000, 0x44CE4000, 0x43160000, 0x00000000, 0x44CE4000,
};

u32 lbl_1_data_1DB8[4] = {
    0x00010000, 0xC33E0000, 0x00000000, 0x44CE4000,
};

u32 lbl_1_data_1DC8[4] = {
    0x00010000, 0xC33E0000, 0x00000000, 0x44BB8000,
};

u32 lbl_1_data_1DD8[4] = {
    0x00010000, 0xC33E0000, 0x00000000, 0x44ED8000,
};

u32 lbl_1_data_1DE8[4] = {
    0x00010000, 0xC3870000, 0x00000000, 0x450AC000,
};

u32 lbl_1_data_1DF8[4] = {
    0x00010000, 0xC28C0000, 0x00000000, 0x451B0000,
};

u32 lbl_1_data_1E08[4] = {
    0x00010000, 0xC2480000, 0x00000000, 0x451F6000,
};

u32 lbl_1_data_1E18[4] = {
    0x00010000, 0x435C0000, 0x00000000, 0x4512E000,
};

u32 lbl_1_data_1E28[4] = {
    0x00010000, 0x435C0000, 0x00000000, 0x4512E000,
};

u32 lbl_1_data_1E38[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x4528C000,
};

u32 lbl_1_data_1E48[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x4528C000,
};

u32 lbl_1_data_1E58[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x453FE000,
};

u32 lbl_1_data_1E68[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x454E4000,
};

u32 lbl_1_data_1E78[4] = {
    0x00010000, 0x42480000, 0x00000000, 0x454E4000,
};

u32 lbl_1_data_1E88[4] = {
    0x00010000, 0x42480000, 0x00000000, 0x454E4000,
};

u32 lbl_1_data_1E98[4] = {
    0x00010000, 0x42480000, 0x00000000, 0x453E0000,
};

u32 lbl_1_data_1EA8[4] = {
    0x00010000, 0x42480000, 0x00000000, 0x45674000,
};

u32 lbl_1_data_1EB8[4] = {
    0x00010000, 0x437A0000, 0x00000000, 0x45480000,
};

u32 lbl_1_data_1EC8[10] = {
    0x00030000, 0x43B40000, 0x00000000, 0x43B40000, 0x43B40000, 0x00000000, 0x442A0000, 
    0xC3870000, 0x00000000, 0x445E8000,
};

u32 lbl_1_data_1EF0[4] = {
    0x00010000, 0x41200000, 0x00000000, 0x44598000,
};

u32 lbl_1_data_1F00[4] = {
    0x00010000, 0x43910000, 0x00000000, 0x44638000,
};

u32 lbl_1_data_1F10[4] = {
    0x00010000, 0x43910000, 0x00000000, 0x44974000,
};

u32 lbl_1_data_1F20[4] = {
    0x00010000, 0x43200000, 0x00000000, 0x44D5C000,
};

u32 lbl_1_data_1F30[4] = {
    0x00010000, 0xC2A00000, 0x00000000, 0x44D5C000,
};

u32 lbl_1_data_1F40[4] = {
    0x00010000, 0xC3AF0000, 0x00000000, 0x44B40000,
};

u32 lbl_1_data_1F50[4] = {
    0x00010000, 0xC3AF0000, 0x00000000, 0x44B40000,
};

u32 lbl_1_data_1F60[4] = {
    0x00010000, 0xC3960000, 0x00000000, 0x44E4C000,
};

u32 lbl_1_data_1F70[4] = {
    0x00010000, 0xC3960000, 0x00000000, 0x4500C000,
};

u32 lbl_1_data_1F80[4] = {
    0x00010000, 0xC2200000, 0x00000000, 0x4500C000,
};

u32 lbl_1_data_1F90[4] = {
    0x00010000, 0xC2200000, 0x00000000, 0x4512E000,
};

u32 lbl_1_data_1FA0[4] = {
    0x00010000, 0xC2200000, 0x00000000, 0x451F6000,
};

u32 lbl_1_data_1FB0[4] = {
    0x00010000, 0xC3A50000, 0x00000000, 0x451F6000,
};

u32 lbl_1_data_1FC0[7] = {
    0x00020000, 0xC3A50000, 0x00000000, 0x450CA000, 0x43B40000, 0x00000000, 0x450CA000,
};

u32 lbl_1_data_1FDC[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x450CA000,
};

u32 lbl_1_data_1FEC[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x452C8000,
};

u32 lbl_1_data_1FFC[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x453CC000,
};

u32 lbl_1_data_200C[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x45494000,
};

u32 lbl_1_data_201C[4] = {
    0x00010000, 0x42C80000, 0x00000000, 0x45494000,
};

u32 lbl_1_data_202C[4] = {
    0x00010000, 0xC3340000, 0x00000000, 0x45494000,
};

u32 lbl_1_data_203C[4] = {
    0x00010000, 0xC3340000, 0x00000000, 0x45598000,
};

u32 lbl_1_data_204C[4] = {
    0x00010000, 0x42DC0000, 0x00000000, 0x45642000,
};

u32 lbl_1_data_205C[4] = {
    0x00010000, 0x43820000, 0x00000000, 0x4550C000,
};

u32 lbl_1_data_206C[7] = {
    0x00020000, 0xC1A00000, 0x00000000, 0x43F00000, 0xC3B40000, 0x00000000, 0x43F00000,
};

u32 lbl_1_data_2088[4] = {
    0x00010000, 0xC1700000, 0x00000000, 0x44660000,
};

u32 lbl_1_data_2098[4] = {
    0x00010000, 0xC3020000, 0x00000000, 0x447C8000,
};

u32 lbl_1_data_20A8[4] = {
    0x00010000, 0x42C80000, 0x00000000, 0x444D0000,
};

u32 lbl_1_data_20B8[4] = {
    0x00010000, 0xC3020000, 0x00000000, 0x447C8000,
};

u32 lbl_1_data_20C8[4] = {
    0x00010000, 0xC2C80000, 0x00000000, 0x444D0000,
};

u32 lbl_1_data_20D8[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x444F8000,
};

u32 lbl_1_data_20E8[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x44098000,
};

u32 lbl_1_data_20F8[16] = {
    0x00050000, 0x43B40000, 0x00000000, 0x43910000, 0xC2C80000, 0x00000000, 0x43910000, 
    0xC2C80000, 0x00000000, 0x447C8000, 0x42A00000, 0x00000000, 0x447C8000, 0x42A00000, 
    0x00000000, 0x449C4000,
};

u32 lbl_1_data_2138[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x449C4000,
};

u32 lbl_1_data_2148[4] = {
    0x00010000, 0x43660000, 0x00000000, 0x44CB2000,
};

u32 lbl_1_data_2158[4] = {
    0x00010000, 0xC2480000, 0x00000000, 0x44CB2000,
};

u32 lbl_1_data_2168[4] = {
    0x00010000, 0x43660000, 0x00000000, 0x44CB2000,
};

u32 lbl_1_data_2178[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x44CB2000,
};

u32 lbl_1_data_2188[7] = {
    0x00020000, 0xC2700000, 0x00000000, 0x44CB2000, 0xC3A00000, 0x00000000, 0x44C9E000,
};

u32 lbl_1_data_21A4[4] = {
    0x00010000, 0xC3AF0000, 0x00000000, 0x44A78000,
};

u32 lbl_1_data_21B4[16] = {
    0x00050000, 0xC3AF0000, 0x00000000, 0x447A0000, 0x42820000, 0x00000000, 0x447A0000, 
    0x42820000, 0x00000000, 0x44D5C000, 0xC2A00000, 0x00000000, 0x44D5C000, 0xC2A00000, 
    0x00000000, 0x44BA4000,
};

u32 lbl_1_data_21F4[4] = {
    0x00010000, 0x43160000, 0x00000000, 0x44BA4000,
};

u32 lbl_1_data_2204[10] = {
    0x00030000, 0x43160000, 0x00000000, 0x44D5C000, 0xC25C0000, 0x00000000, 0x44D5C000, 
    0xC3070000, 0x00000000, 0x45110000,
};

u32 lbl_1_data_222C[4] = {
    0x00010000, 0xC3B40000, 0x00000000, 0x45110000,
};

u32 lbl_1_data_223C[4] = {
    0x00010000, 0x435C0000, 0x00000000, 0x4508E000,
};

u32 lbl_1_data_224C[4] = {
    0x00010000, 0x435C0000, 0x00000000, 0x45232000,
};

u32 lbl_1_data_225C[4] = {
    0x00010000, 0x435C0000, 0x00000000, 0x4508E000,
};

u32 lbl_1_data_226C[4] = {
    0x00010000, 0x435C0000, 0x00000000, 0x45124000,
};

u32 lbl_1_data_227C[4] = {
    0x00010000, 0x42480000, 0x00000000, 0x45200000,
};

u32 lbl_1_data_228C[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x45200000,
};

u32 lbl_1_data_229C[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x45200000,
};

u32 lbl_1_data_22AC[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x4541C000,
};

u32 lbl_1_data_22BC[4] = {
    0x00010000, 0x43830000, 0x00000000, 0x4549E000,
};

u32 lbl_1_data_22CC[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x4549E000,
};

u32 lbl_1_data_22DC[4] = {
    0x00010000, 0xC2480000, 0x00000000, 0x453EA000,
};

u32 lbl_1_data_22EC[4] = {
    0x00010000, 0xC3820000, 0x00000000, 0x453EA000,
};

u32 lbl_1_data_22FC[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x455AC000,
};

u32 lbl_1_data_230C[4] = {
    0x00010000, 0x43A00000, 0x00000000, 0x4555C000,
};

u32 lbl_1_data_231C[10] = {
    0x00030000, 0x43960000, 0x00000000, 0x44340000, 0xC3B40000, 0x00000000, 0x44340000, 
    0xC3B40000, 0x00000000, 0x43160000,
};

u32 lbl_1_data_2344[4] = {
    0x00010000, 0xC3B40000, 0x00000000, 0x43D70000,
};

u32 lbl_1_data_2354[4] = {
    0x00010000, 0xC3AF0000, 0x00000000, 0x449D8000,
};

u32 lbl_1_data_2364[4] = {
    0x00010000, 0xC3AF0000, 0x00000000, 0x44A28000,
};

u32 lbl_1_data_2374[10] = {
    0x00030000, 0xC3AF0000, 0x00000000, 0x44C08000, 0x42C80000, 0x00000000, 0x44C08000, 
    0x42C80000, 0x00000000, 0x446D8000,
};

u32 lbl_1_data_239C[4] = {
    0x00010000, 0x42C80000, 0x00000000, 0x44938000,
};

u32 lbl_1_data_23AC[10] = {
    0x00030000, 0xC3AF0000, 0x00000000, 0x44C08000, 0x42C80000, 0x00000000, 0x44C08000, 
    0x42C80000, 0x00000000, 0x446D8000,
};

u32 lbl_1_data_23D4[4] = {
    0x00010000, 0x43960000, 0x00000000, 0x447C8000,
};

u32 lbl_1_data_23E4[4] = {
    0x00010000, 0x42C80000, 0x00000000, 0x44AF0000,
};

u32 lbl_1_data_23F4[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x44AF0000,
};

u32 lbl_1_data_2404[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x44C30000,
};

u32 lbl_1_data_2414[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x44C30000,
};

u32 lbl_1_data_2424[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x44D34000,
};

u32 lbl_1_data_2434[4] = {
    0x00010000, 0xC33E0000, 0x00000000, 0x44BA4000,
};

u32 lbl_1_data_2444[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x44D34000,
};

u32 lbl_1_data_2454[4] = {
    0x00010000, 0x432A0000, 0x00000000, 0x44D34000,
};

u32 lbl_1_data_2464[4] = {
    0x00010000, 0xC3480000, 0x00000000, 0x44BA4000,
};

u32 lbl_1_data_2474[4] = {
    0x00010000, 0xC3B40000, 0x00000000, 0x44BA4000,
};

u32 lbl_1_data_2484[4] = {
    0x00010000, 0xC3960000, 0x00000000, 0x44ED8000,
};

u32 lbl_1_data_2494[4] = {
    0x00010000, 0xC2340000, 0x00000000, 0x44ED8000,
};

u32 lbl_1_data_24A4[4] = {
    0x00010000, 0xC2340000, 0x00000000, 0x45034000,
};

u32 lbl_1_data_24B4[4] = {
    0x00010000, 0xC3700000, 0x00000000, 0x4511A000,
};

u32 lbl_1_data_24C4[4] = {
    0x00010000, 0xC3B40000, 0x00000000, 0x4512E000,
};

u32 lbl_1_data_24D4[4] = {
    0x00010000, 0xC2200000, 0x00000000, 0x451F6000,
};

u32 lbl_1_data_24E4[4] = {
    0x00010000, 0x43340000, 0x00000000, 0x45124000,
};

u32 lbl_1_data_24F4[4] = {
    0x00010000, 0x43480000, 0x00000000, 0x45246000,
};

u32 lbl_1_data_2504[4] = {
    0x00010000, 0x43960000, 0x00000000, 0x4541C000,
};

u32 lbl_1_data_2514[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x4541C000,
};

u32 lbl_1_data_2524[4] = {
    0x00010000, 0x43960000, 0x00000000, 0x4541C000,
};

u32 lbl_1_data_2534[4] = {
    0x00010000, 0x433C0000, 0x00000000, 0x454C6000,
};

u32 lbl_1_data_2544[4] = {
    0x00010000, 0xC2480000, 0x00000000, 0x453EA000,
};

u32 lbl_1_data_2554[4] = {
    0x00010000, 0x43820000, 0x00000000, 0x453EA000,
};

u32 lbl_1_data_2564[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x455AC000,
};

u32 lbl_1_data_2574[4] = {
    0x00010000, 0xC3A00000, 0x00000000, 0x4555C000,
};

u32 lbl_1_data_2584[7] = {
    0x00020000, 0xC2C80000, 0x00000000, 0x43A50000, 0xC3910000, 0x00000000, 0x42D20000,
};

u32 lbl_1_data_25A0[4] = {
    0x00010000, 0xC3910000, 0x00000000, 0x43DC0000,
};

u32 lbl_1_data_25B0[4] = {
    0x00010000, 0xC3AF0000, 0x00000000, 0x44A28000,
};

u32 lbl_1_data_25C0[4] = {
    0x00010000, 0xC3AF0000, 0x00000000, 0x44B68000,
};

u32 lbl_1_data_25D0[10] = {
    0x00030000, 0xC3AF0000, 0x00000000, 0x44C1C000, 0x43AF0000, 0x00000000, 0x44C1C000, 
    0x43AF0000, 0x00000000, 0x446D8000,
};

u32 lbl_1_data_25F8[4] = {
    0x00010000, 0x43AF0000, 0x00000000, 0x44318000,
};

u32 lbl_1_data_2608[7] = {
    0x00020000, 0x428C0000, 0x00000000, 0x44318000, 0x428C0000, 0x00000000, 0x44AB4000,
};

u32 lbl_1_data_2624[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x44AB4000,
};

u32 lbl_1_data_2634[7] = {
    0x00020000, 0x428C0000, 0x00000000, 0x44318000, 0x428C0000, 0x00000000, 0x44AB4000,
};

u32 lbl_1_data_2650[4] = {
    0x00010000, 0x43340000, 0x00000000, 0x44AA0000,
};

u32 lbl_1_data_2660[4] = {
    0x00010000, 0x43A00000, 0x00000000, 0x44ED8000,
};

u32 lbl_1_data_2670[4] = {
    0x00010000, 0x43A00000, 0x00000000, 0x44FDC000,
};

u32 lbl_1_data_2680[13] = {
    0x00040000, 0x43A00000, 0x00000000, 0x450C0000, 0xC2F00000, 0x00000000, 0x450C0000, 
    0xC2F00000, 0x00000000, 0x44C80000, 0xC3B40000, 0x00000000, 0x44C80000,
};

u32 lbl_1_data_26B4[4] = {
    0x00010000, 0xC3480000, 0x00000000, 0x44E38000,
};

u32 lbl_1_data_26C4[4] = {
    0x00010000, 0xC25C0000, 0x00000000, 0x4500C000,
};

u32 lbl_1_data_26D4[4] = {
    0x00010000, 0xC25C0000, 0x00000000, 0x45110000,
};

u32 lbl_1_data_26E4[4] = {
    0x00010000, 0xC3820000, 0x00000000, 0x45278000,
};

u32 lbl_1_data_26F4[4] = {
    0x00010000, 0xC3960000, 0x00000000, 0x453F4000,
};

u32 lbl_1_data_2704[13] = {
    0x00040000, 0xC3480000, 0x00000000, 0x4544E000, 0x43480000, 0x00000000, 0x4544E000, 
    0x43480000, 0x00000000, 0x451D8000, 0x42B40000, 0x00000000, 0x45098000,
};

u32 lbl_1_data_2738[4] = {
    0x00010000, 0x43AF0000, 0x00000000, 0x4528C000,
};

u32 lbl_1_data_2748[4] = {
    0x00010000, 0x43AF0000, 0x00000000, 0x4528C000,
};

u32 lbl_1_data_2758[4] = {
    0x00010000, 0x43200000, 0x00000000, 0x453EA000,
};

u32 lbl_1_data_2768[4] = {
    0x00010000, 0x43AF0000, 0x00000000, 0x454E4000,
};

u32 lbl_1_data_2778[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x454E4000,
};

u32 lbl_1_data_2788[4] = {
    0x00010000, 0xC2480000, 0x00000000, 0x453EA000,
};

u32 lbl_1_data_2798[4] = {
    0x00010000, 0x43820000, 0x00000000, 0x453EA000,
};

u32 lbl_1_data_27A8[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x456D8000,
};

u32 lbl_1_data_27B8[4] = {
    0x00010000, 0xC3480000, 0x00000000, 0x45674000,
};

u32 lbl_1_data_27C8[7] = {
    0x00020000, 0x41F00000, 0x00000000, 0x43B40000, 0xC3B40000, 0x00000000, 0x42C80000,
};

u32 lbl_1_data_27E4[4] = {
    0x00010000, 0xC3B40000, 0x00000000, 0x43DC0000,
};

u32 lbl_1_data_27F4[4] = {
    0x00010000, 0xC2F00000, 0x00000000, 0x44750000,
};

u32 lbl_1_data_2804[4] = {
    0x00010000, 0x432A0000, 0x00000000, 0x44750000,
};

u32 lbl_1_data_2814[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x44750000,
};

u32 lbl_1_data_2824[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x44A00000,
};

u32 lbl_1_data_2834[4] = {
    0x00010000, 0x430C0000, 0x00000000, 0x44D5C000,
};

u32 lbl_1_data_2844[4] = {
    0x00010000, 0xC1C80000, 0x00000000, 0x44BA4000,
};

u32 lbl_1_data_2854[4] = {
    0x00010000, 0x430C0000, 0x00000000, 0x44D5C000,
};

u32 lbl_1_data_2864[4] = {
    0x00010000, 0x43070000, 0x00000000, 0x44B68000,
};

u32 lbl_1_data_2874[7] = {
    0x00020000, 0xC1C80000, 0x00000000, 0x44BA4000, 0xC3480000, 0x00000000, 0x44BA4000,
};

u32 lbl_1_data_2890[4] = {
    0x00010000, 0xC35C0000, 0x00000000, 0x44E74000,
};

u32 lbl_1_data_28A0[4] = {
    0x00010000, 0xC3960000, 0x00000000, 0x44F3C000,
};

u32 lbl_1_data_28B0[4] = {
    0x00010000, 0xC2480000, 0x00000000, 0x4507A000,
};

u32 lbl_1_data_28C0[4] = {
    0x00010000, 0xC2480000, 0x00000000, 0x4507A000,
};

u32 lbl_1_data_28D0[4] = {
    0x00010000, 0xC38C0000, 0x00000000, 0x45142000,
};

u32 lbl_1_data_28E0[4] = {
    0x00010000, 0xC2480000, 0x00000000, 0x4507A000,
};

u32 lbl_1_data_28F0[4] = {
    0x00010000, 0xC33E0000, 0x00000000, 0x4507A000,
};

u32 lbl_1_data_2900[7] = {
    0x00020000, 0xC38C0000, 0x00000000, 0x45142000, 0xC2480000, 0x00000000, 0x45200000,
};

u32 lbl_1_data_291C[4] = {
    0x00010000, 0x43480000, 0x00000000, 0x45200000,
};

u32 lbl_1_data_292C[4] = {
    0x00010000, 0x435C0000, 0x00000000, 0x4512E000,
};

u32 lbl_1_data_293C[4] = {
    0x00010000, 0x435C0000, 0x00000000, 0x4528C000,
};

u32 lbl_1_data_294C[4] = {
    0x00010000, 0x43870000, 0x00000000, 0x454B2000,
};

u32 lbl_1_data_295C[4] = {
    0x00010000, 0xC1F00000, 0x00000000, 0x45480000,
};

u32 lbl_1_data_296C[4] = {
    0x00010000, 0xC3700000, 0x00000000, 0x45458000,
};

u32 lbl_1_data_297C[4] = {
    0x00010000, 0xC3700000, 0x00000000, 0x45570000,
};

u32 lbl_1_data_298C[4] = {
    0x00010000, 0x42EC0000, 0x00000000, 0x4556D000,
};

u32 lbl_1_data_299C[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x456A6000,
};

u32 lbl_1_data_29AC[4] = {
    0x00010000, 0xC2480000, 0x00000000, 0x453EA000,
};

u32 lbl_1_data_29BC[4] = {
    0x00010000, 0x43820000, 0x00000000, 0x453EA000,
};

u32 lbl_1_data_29CC[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x455AC000,
};

u32 lbl_1_data_29DC[4] = {
    0x00010000, 0xC3480000, 0x00000000, 0x455AC000,
};

u32 lbl_1_data_29EC[4] = {
    0x00010000, 0x435C0000, 0x00000000, 0x437A0000,
};

u32 lbl_1_data_29FC[4] = {
    0x00010000, 0x43AC8000, 0x00000000, 0x44070000,
};

u32 lbl_1_data_2A0C[10] = {
    0x00030000, 0x43AC8000, 0x00000000, 0x44318000, 0xC3200000, 0x00000000, 0x44318000, 
    0xC3200000, 0x00000000, 0x43960000,
};

u32 lbl_1_data_2A34[4] = {
    0x00010000, 0xC3200000, 0x00000000, 0x44098000,
};

u32 lbl_1_data_2A44[4] = {
    0x00010000, 0xC3020000, 0x00000000, 0x446D8000,
};

u32 lbl_1_data_2A54[4] = {
    0x00010000, 0x43200000, 0x00000000, 0x446D8000,
};

u32 lbl_1_data_2A64[4] = {
    0x00010000, 0x437A0000, 0x00000000, 0x446D8000,
};

u32 lbl_1_data_2A74[4] = {
    0x00010000, 0x437A0000, 0x00000000, 0x449C4000,
};

u32 lbl_1_data_2A84[4] = {
    0x00010000, 0x428C0000, 0x00000000, 0x44ADC000,
};

u32 lbl_1_data_2A94[4] = {
    0x00010000, 0x438C0000, 0x00000000, 0x44C58000,
};

u32 lbl_1_data_2AA4[4] = {
    0x00010000, 0x428C0000, 0x00000000, 0x44ADC000,
};

u32 lbl_1_data_2AB4[4] = {
    0x00010000, 0x435C0000, 0x00000000, 0x44ADC000,
};

u32 lbl_1_data_2AC4[4] = {
    0x00010000, 0x43AA0000, 0x00000000, 0x44CF8000,
};

u32 lbl_1_data_2AD4[4] = {
    0x00010000, 0x43AA0000, 0x00000000, 0x44ED8000,
};

u32 lbl_1_data_2AE4[10] = {
    0x00030000, 0x43AA0000, 0x00000000, 0x45098000, 0xC37A0000, 0x00000000, 0x45098000, 
    0xC37A0000, 0x00000000, 0x44C58000,
};

u32 lbl_1_data_2B0C[4] = {
    0x00010000, 0xC37A0000, 0x00000000, 0x44E60000,
};

u32 lbl_1_data_2B1C[4] = {
    0x00010000, 0xC2200000, 0x00000000, 0x4502A000,
};

u32 lbl_1_data_2B2C[4] = {
    0x00010000, 0xC3160000, 0x00000000, 0x450FC000,
};

u32 lbl_1_data_2B3C[4] = {
    0x00010000, 0xC3B40000, 0x00000000, 0x450FC000,
};

u32 lbl_1_data_2B4C[4] = {
    0x00010000, 0xC2A00000, 0x00000000, 0x45192000,
};

u32 lbl_1_data_2B5C[4] = {
    0x00010000, 0xC3B40000, 0x00000000, 0x450FC000,
};

u32 lbl_1_data_2B6C[4] = {
    0x00010000, 0xC3700000, 0x00000000, 0x450FC000,
};

u32 lbl_1_data_2B7C[7] = {
    0x00020000, 0x42700000, 0x00000000, 0x45133000, 0x43480000, 0x00000000, 0x45133000,
};

u32 lbl_1_data_2B98[4] = {
    0x00010000, 0x43A28000, 0x00000000, 0x45098000,
};

u32 lbl_1_data_2BA8[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x454B2000,
};

u32 lbl_1_data_2BB8[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x454B2000,
};

u32 lbl_1_data_2BC8[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x454B2000,
};

u32 lbl_1_data_2BD8[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x4566A000,
};

u32 lbl_1_data_2BE8[4] = {
    0x00010000, 0x432A0000, 0x00000000, 0x45624000,
};

u32 lbl_1_data_2BF8[4] = {
    0x00010000, 0xC1A00000, 0x00000000, 0x456EC000,
};

u32 lbl_1_data_2C08[16] = {
    0x00050000, 0x43AF0000, 0x00000000, 0x43960000, 0x43AF0000, 0x00000000, 0x44200000, 
    0x43020000, 0x00000000, 0x444D0000, 0xC30C0000, 0x00000000, 0x44390000, 0xC3960000, 
    0x00000000, 0x43C08000,
};

u32 lbl_1_data_2C48[4] = {
    0x00010000, 0xC28C0000, 0x00000000, 0x440FC000,
};

u32 lbl_1_data_2C58[4] = {
    0x00010000, 0xC28C0000, 0x00000000, 0x447C8000,
};

u32 lbl_1_data_2C68[4] = {
    0x00010000, 0x43480000, 0x00000000, 0x447C8000,
};

u32 lbl_1_data_2C78[4] = {
    0x00010000, 0x43A00000, 0x00000000, 0x447C8000,
};

u32 lbl_1_data_2C88[4] = {
    0x00010000, 0x43A00000, 0x00000000, 0x449EC000,
};

u32 lbl_1_data_2C98[4] = {
    0x00010000, 0x43160000, 0x00000000, 0x44D5C000,
};

u32 lbl_1_data_2CA8[4] = {
    0x00010000, 0xC2A00000, 0x00000000, 0x44BF4000,
};

u32 lbl_1_data_2CB8[4] = {
    0x00010000, 0x43160000, 0x00000000, 0x44D5C000,
};

u32 lbl_1_data_2CC8[4] = {
    0x00010000, 0x438E8000, 0x00000000, 0x44CD0000,
};

u32 lbl_1_data_2CD8[4] = {
    0x00010000, 0xC3480000, 0x00000000, 0x44BA4000,
};

u32 lbl_1_data_2CE8[4] = {
    0x00010000, 0xC3480000, 0x00000000, 0x44DFC000,
};

u32 lbl_1_data_2CF8[4] = {
    0x00010000, 0xC3960000, 0x00000000, 0x4502A000,
};

u32 lbl_1_data_2D08[4] = {
    0x00010000, 0xC2C80000, 0x00000000, 0x45142000,
};

u32 lbl_1_data_2D18[4] = {
    0x00010000, 0xC2480000, 0x00000000, 0x45200000,
};

u32 lbl_1_data_2D28[4] = {
    0x00010000, 0xC3A00000, 0x00000000, 0x45200000,
};

u32 lbl_1_data_2D38[7] = {
    0x00020000, 0xC3A00000, 0x00000000, 0x450FC000, 0x435C0000, 0x00000000, 0x450FC000,
};

u32 lbl_1_data_2D54[4] = {
    0x00010000, 0x43AF0000, 0x00000000, 0x45048000,
};

u32 lbl_1_data_2D64[4] = {
    0x00010000, 0x43B40000, 0x00000000, 0x453B8000,
};

u32 lbl_1_data_2D74[4] = {
    0x00010000, 0x42C80000, 0x00000000, 0x45480000,
};

u32 lbl_1_data_2D84[4] = {
    0x00010000, 0xC1200000, 0x00000000, 0x4549E000,
};

u32 lbl_1_data_2D94[4] = {
    0x00010000, 0x433E0000, 0x00000000, 0x45502000,
};

u32 lbl_1_data_2DA4[4] = {
    0x00010000, 0x41A00000, 0x00000000, 0x4569C000,
};

u32 lbl_1_data_2DB4[4] = {
    0x00010000, 0x43070000, 0x00000000, 0x45624000,
};

u32 lbl_1_data_2DC4[4] = {
    0x00010000, 0x43960000, 0x00000000, 0x453CC000,
};

u32 lbl_1_data_2DD4[4] = {
    0x00010000, 0x43960000, 0x00000000, 0x45516000,
};

u32 lbl_1_data_2DE4[4] = {
    0x00010000, 0x00000000, 0x00000000, 0x456D8000,
};

u32 lbl_1_data_2DF4[4] = {
    0x00010000, 0xC3320000, 0x00000000, 0x45680000,
};

u32 lbl_1_data_2E04[4] = {
    (u32 )&lbl_1_data_528, 0x00000000, 0x00010000, (u32 )&lbl_1_data_4040,
};

void *lbl_1_data_2E14[9] = {
    (void *)&lbl_1_data_528, 0x00000000, (void *)0x00060000, (void *)&lbl_1_data_2ED4, 
    (void *)&lbl_1_data_2F60, (void *)&lbl_1_data_2FF8, (void *)&lbl_1_data_3094, 
    (void *)&lbl_1_data_3124, (void *)&lbl_1_data_31A4,
};

void *lbl_1_data_2E38[15] = {
    (void *)&lbl_1_data_528, 0x00000000, (void *)0x000C0000, (void *)&lbl_1_data_3224, 
    (void *)&lbl_1_data_32E4, (void *)&lbl_1_data_33A4, (void *)&lbl_1_data_3464, 
    (void *)&lbl_1_data_3524, (void *)&lbl_1_data_35E4, (void *)&lbl_1_data_36A4, 
    (void *)&lbl_1_data_3764, (void *)&lbl_1_data_3824, (void *)&lbl_1_data_38E4, 
    (void *)&lbl_1_data_39A4, (void *)&lbl_1_data_3A64,
};

void *lbl_1_data_2E74[15] = {
    (void *)&lbl_1_data_528, 0x00000000, (void *)0x000C0000, (void *)&lbl_1_data_3224, 
    (void *)&lbl_1_data_32E4, (void *)&lbl_1_data_33A4, (void *)&lbl_1_data_3464, 
    (void *)&lbl_1_data_3524, (void *)&lbl_1_data_35E4, (void *)&lbl_1_data_36A4, 
    (void *)&lbl_1_data_3764, (void *)&lbl_1_data_3824, (void *)&lbl_1_data_38E4, 
    (void *)&lbl_1_data_39A4, (void *)&lbl_1_data_3A64,
};

void *lbl_1_data_2EB0[9] = {
    (void *)&lbl_1_data_528, 0x00000000, (void *)0x00060000, (void *)&lbl_1_data_3B24, 
    (void *)&lbl_1_data_3C40, (void *)&lbl_1_data_3D5C, (void *)&lbl_1_data_3E40, 
    (void *)&lbl_1_data_3F48, (void *)&lbl_1_data_4040,
};

u32 lbl_1_data_2ED4[4] = {
    (u32 )&lbl_1_data_548, (u32 )&lbl_1_data_564, 0x00010000, (u32 )&lbl_1_data_2EE4,
};

u32 lbl_1_data_2EE4[6] = {
    (u32 )&lbl_1_data_574, (u32 )&lbl_1_data_584, 0x00030000, (u32 )&lbl_1_data_2EFC, 
    (u32 )&lbl_1_data_2F88, (u32 )&lbl_1_data_3020,
};

u32 lbl_1_data_2EFC[4] = {
    (u32 )&lbl_1_data_594, (u32 )&lbl_1_data_5A4, 0x00010000, (u32 )&lbl_1_data_2F0C,
};

u32 lbl_1_data_2F0C[5] = {
    (u32 )&lbl_1_data_5B4, (u32 )&lbl_1_data_5C4, 0x00020000, (u32 )&lbl_1_data_2F20, 
    (u32 )&lbl_1_data_3044,
};

u32 lbl_1_data_2F20[4] = {
    (u32 )&lbl_1_data_5D4, (u32 )&lbl_1_data_5E4, 0x00010000, (u32 )&lbl_1_data_2F30,
};

u32 lbl_1_data_2F30[4] = {
    (u32 )&lbl_1_data_5F4, (u32 )&lbl_1_data_604, 0x00010000, (u32 )&lbl_1_data_2F40,
};

u32 lbl_1_data_2F40[4] = {
    (u32 )&lbl_1_data_614, (u32 )&lbl_1_data_624, 0x00010000, (u32 )&lbl_1_data_2F50,
};

u32 lbl_1_data_2F50[4] = {
    (u32 )&lbl_1_data_634, (u32 )&lbl_1_data_644, 0x00010000, (u32 )&lbl_1_data_4124,
};

u32 lbl_1_data_2F60[4] = {
    (u32 )&lbl_1_data_654, (u32 )&lbl_1_data_670, 0x00010000, (u32 )&lbl_1_data_2F70,
};

u32 lbl_1_data_2F70[6] = {
    (u32 )&lbl_1_data_680, (u32 )&lbl_1_data_6A8, 0x00030000, (u32 )&lbl_1_data_2F88, 
    (u32 )&lbl_1_data_2EFC, (u32 )&lbl_1_data_3020,
};

u32 lbl_1_data_2F88[4] = {
    (u32 )&lbl_1_data_6B8, (u32 )&lbl_1_data_6C8, 0x00010000, (u32 )&lbl_1_data_2F98,
};

u32 lbl_1_data_2F98[4] = {
    (u32 )&lbl_1_data_6D8, (u32 )&lbl_1_data_700, 0x00010000, (u32 )&lbl_1_data_2FA8,
};

u32 lbl_1_data_2FA8[4] = {
    (u32 )&lbl_1_data_710, (u32 )&lbl_1_data_720, 0x00010000, (u32 )&lbl_1_data_2FB8,
};

u32 lbl_1_data_2FB8[4] = {
    (u32 )&lbl_1_data_730, (u32 )&lbl_1_data_740, 0x00010000, (u32 )&lbl_1_data_2FC8,
};

u32 lbl_1_data_2FC8[4] = {
    (u32 )&lbl_1_data_750, (u32 )&lbl_1_data_760, 0x00010000, (u32 )&lbl_1_data_2FD8,
};

u32 lbl_1_data_2FD8[4] = {
    (u32 )&lbl_1_data_770, (u32 )&lbl_1_data_780, 0x00010000, (u32 )&lbl_1_data_2FE8,
};

u32 lbl_1_data_2FE8[4] = {
    (u32 )&lbl_1_data_790, (u32 )&lbl_1_data_7A0, 0x00010000, (u32 )&lbl_1_data_4124,
};

u32 lbl_1_data_2FF8[4] = {
    (u32 )&lbl_1_data_7B0, (u32 )&lbl_1_data_7CC, 0x00010000, (u32 )&lbl_1_data_3008,
};

u32 lbl_1_data_3008[6] = {
    (u32 )&lbl_1_data_7DC, (u32 )&lbl_1_data_7EC, 0x00030000, (u32 )&lbl_1_data_3020, 
    (u32 )&lbl_1_data_2EFC, (u32 )&lbl_1_data_2F88,
};

u32 lbl_1_data_3020[4] = {
    (u32 )&lbl_1_data_7FC, (u32 )&lbl_1_data_80C, 0x00010000, (u32 )&lbl_1_data_3030,
};

u32 lbl_1_data_3030[5] = {
    (u32 )&lbl_1_data_81C, (u32 )&lbl_1_data_838, 0x00020000, (u32 )&lbl_1_data_3044, 
    (u32 )&lbl_1_data_2F20,
};

u32 lbl_1_data_3044[4] = {
    (u32 )&lbl_1_data_848, (u32 )&lbl_1_data_858, 0x00010000, (u32 )&lbl_1_data_3054,
};

u32 lbl_1_data_3054[4] = {
    (u32 )&lbl_1_data_868, (u32 )&lbl_1_data_890, 0x00010000, (u32 )&lbl_1_data_3064,
};

u32 lbl_1_data_3064[4] = {
    (u32 )&lbl_1_data_8A0, (u32 )&lbl_1_data_8B0, 0x00010000, (u32 )&lbl_1_data_3074,
};

u32 lbl_1_data_3074[4] = {
    (u32 )&lbl_1_data_8C0, (u32 )&lbl_1_data_8D0, 0x00010000, (u32 )&lbl_1_data_3084,
};

u32 lbl_1_data_3084[4] = {
    (u32 )&lbl_1_data_8E0, (u32 )&lbl_1_data_8F0, 0x00010000, (u32 )&lbl_1_data_4124,
};

u32 lbl_1_data_3094[4] = {
    (u32 )&lbl_1_data_900, (u32 )&lbl_1_data_91C, 0x00010000, (u32 )&lbl_1_data_30A4,
};

u32 lbl_1_data_30A4[4] = {
    (u32 )&lbl_1_data_92C, (u32 )&lbl_1_data_93C, 0x00010000, (u32 )&lbl_1_data_30B4,
};

u32 lbl_1_data_30B4[4] = {
    (u32 )&lbl_1_data_94C, (u32 )&lbl_1_data_95C, 0x00010000, (u32 )&lbl_1_data_30C4,
};

u32 lbl_1_data_30C4[4] = {
    (u32 )&lbl_1_data_96C, (u32 )&lbl_1_data_97C, 0x00010000, (u32 )&lbl_1_data_30D4,
};

u32 lbl_1_data_30D4[4] = {
    (u32 )&lbl_1_data_98C, (u32 )&lbl_1_data_99C, 0x00010000, (u32 )&lbl_1_data_30E4,
};

u32 lbl_1_data_30E4[4] = {
    (u32 )&lbl_1_data_9AC, (u32 )&lbl_1_data_9BC, 0x00010000, (u32 )&lbl_1_data_30F4,
};

u32 lbl_1_data_30F4[4] = {
    (u32 )&lbl_1_data_9CC, (u32 )&lbl_1_data_9DC, 0x00010000, (u32 )&lbl_1_data_3104,
};

u32 lbl_1_data_3104[4] = {
    (u32 )&lbl_1_data_9EC, (u32 )&lbl_1_data_9FC, 0x00010000, (u32 )&lbl_1_data_3114,
};

u32 lbl_1_data_3114[4] = {
    (u32 )&lbl_1_data_A0C, (u32 )&lbl_1_data_A1C, 0x00010000, (u32 )&lbl_1_data_4124,
};

u32 lbl_1_data_3124[4] = {
    (u32 )&lbl_1_data_A2C, (u32 )&lbl_1_data_A48, 0x00010000, (u32 )&lbl_1_data_3134,
};

u32 lbl_1_data_3134[4] = {
    (u32 )&lbl_1_data_A58, (u32 )&lbl_1_data_A68, 0x00010000, (u32 )&lbl_1_data_3144,
};

u32 lbl_1_data_3144[4] = {
    (u32 )&lbl_1_data_A78, (u32 )&lbl_1_data_A88, 0x00010000, (u32 )&lbl_1_data_3154,
};

u32 lbl_1_data_3154[4] = {
    (u32 )&lbl_1_data_A98, (u32 )&lbl_1_data_AC0, 0x00010000, (u32 )&lbl_1_data_3164,
};

u32 lbl_1_data_3164[4] = {
    (u32 )&lbl_1_data_AD0, (u32 )&lbl_1_data_AE0, 0x00010000, (u32 )&lbl_1_data_3174,
};

u32 lbl_1_data_3174[4] = {
    (u32 )&lbl_1_data_AF0, (u32 )&lbl_1_data_B00, 0x00010000, (u32 )&lbl_1_data_3184,
};

u32 lbl_1_data_3184[4] = {
    (u32 )&lbl_1_data_B10, (u32 )&lbl_1_data_B20, 0x00010000, (u32 )&lbl_1_data_3194,
};

u32 lbl_1_data_3194[4] = {
    (u32 )&lbl_1_data_B30, (u32 )&lbl_1_data_B40, 0x00010000, (u32 )&lbl_1_data_4124,
};

u32 lbl_1_data_31A4[4] = {
    (u32 )&lbl_1_data_B50, (u32 )&lbl_1_data_B78, 0x00010000, (u32 )&lbl_1_data_31B4,
};

u32 lbl_1_data_31B4[4] = {
    (u32 )&lbl_1_data_B88, (u32 )&lbl_1_data_B98, 0x00010000, (u32 )&lbl_1_data_31C4,
};

u32 lbl_1_data_31C4[4] = {
    (u32 )&lbl_1_data_BA8, (u32 )&lbl_1_data_BC4, 0x00010000, (u32 )&lbl_1_data_31D4,
};

u32 lbl_1_data_31D4[4] = {
    (u32 )&lbl_1_data_BD4, (u32 )&lbl_1_data_BE4, 0x00010000, (u32 )&lbl_1_data_31E4,
};

u32 lbl_1_data_31E4[4] = {
    (u32 )&lbl_1_data_BF4, (u32 )&lbl_1_data_C04, 0x00010000, (u32 )&lbl_1_data_31F4,
};

u32 lbl_1_data_31F4[4] = {
    (u32 )&lbl_1_data_C14, (u32 )&lbl_1_data_C24, 0x00010000, (u32 )&lbl_1_data_3204,
};

u32 lbl_1_data_3204[4] = {
    (u32 )&lbl_1_data_C34, (u32 )&lbl_1_data_C44, 0x00010000, (u32 )&lbl_1_data_3214,
};

u32 lbl_1_data_3214[4] = {
    (u32 )&lbl_1_data_C54, (u32 )&lbl_1_data_C64, 0x00010000, (u32 )&lbl_1_data_4124,
};

u32 lbl_1_data_3224[4] = {
    (u32 )&lbl_1_data_C74, (u32 )&lbl_1_data_C90, 0x00010000, (u32 )&lbl_1_data_3234,
};

u32 lbl_1_data_3234[4] = {
    (u32 )&lbl_1_data_CA0, (u32 )&lbl_1_data_CB0, 0x00010000, (u32 )&lbl_1_data_3244,
};

u32 lbl_1_data_3244[4] = {
    (u32 )&lbl_1_data_CC0, (u32 )&lbl_1_data_CD0, 0x00010000, (u32 )&lbl_1_data_3254,
};

u32 lbl_1_data_3254[4] = {
    (u32 )&lbl_1_data_CE0, (u32 )&lbl_1_data_CF0, 0x00010000, (u32 )&lbl_1_data_3264,
};

u32 lbl_1_data_3264[4] = {
    (u32 )&lbl_1_data_D00, (u32 )&lbl_1_data_D10, 0x00010000, (u32 )&lbl_1_data_3274,
};

u32 lbl_1_data_3274[4] = {
    (u32 )&lbl_1_data_D20, (u32 )&lbl_1_data_D30, 0x00010000, (u32 )&lbl_1_data_3284,
};

u32 lbl_1_data_3284[4] = {
    (u32 )&lbl_1_data_D40, (u32 )&lbl_1_data_D50, 0x00010000, (u32 )&lbl_1_data_3294,
};

u32 lbl_1_data_3294[4] = {
    (u32 )&lbl_1_data_D60, (u32 )&lbl_1_data_D70, 0x00010000, (u32 )&lbl_1_data_32A4,
};

u32 lbl_1_data_32A4[4] = {
    (u32 )&lbl_1_data_D80, (u32 )&lbl_1_data_D90, 0x00010000, (u32 )&lbl_1_data_32B4,
};

u32 lbl_1_data_32B4[4] = {
    (u32 )&lbl_1_data_DA0, (u32 )&lbl_1_data_DB0, 0x00010000, (u32 )&lbl_1_data_32C4,
};

u32 lbl_1_data_32C4[4] = {
    (u32 )&lbl_1_data_DC0, (u32 )&lbl_1_data_DD0, 0x00010000, (u32 )&lbl_1_data_32D4,
};

u32 lbl_1_data_32D4[4] = {
    (u32 )&lbl_1_data_DE0, (u32 )&lbl_1_data_DF0, 0x00010000, (u32 )&lbl_1_data_4124,
};

u32 lbl_1_data_32E4[4] = {
    (u32 )&lbl_1_data_E00, (u32 )&lbl_1_data_E1C, 0x00010000, (u32 )&lbl_1_data_32F4,
};

u32 lbl_1_data_32F4[4] = {
    (u32 )&lbl_1_data_E2C, (u32 )&lbl_1_data_E3C, 0x00010000, (u32 )&lbl_1_data_3304,
};

u32 lbl_1_data_3304[4] = {
    (u32 )&lbl_1_data_E4C, (u32 )&lbl_1_data_E74, 0x00010000, (u32 )&lbl_1_data_3314,
};

u32 lbl_1_data_3314[4] = {
    (u32 )&lbl_1_data_E84, (u32 )&lbl_1_data_E94, 0x00010000, (u32 )&lbl_1_data_3324,
};

u32 lbl_1_data_3324[4] = {
    (u32 )&lbl_1_data_EA4, (u32 )&lbl_1_data_EB4, 0x00010000, (u32 )&lbl_1_data_3334,
};

u32 lbl_1_data_3334[4] = {
    (u32 )&lbl_1_data_EC4, (u32 )&lbl_1_data_ED4, 0x00010000, (u32 )&lbl_1_data_3344,
};

u32 lbl_1_data_3344[4] = {
    (u32 )&lbl_1_data_EE4, (u32 )&lbl_1_data_EF4, 0x00010000, (u32 )&lbl_1_data_3354,
};

u32 lbl_1_data_3354[4] = {
    (u32 )&lbl_1_data_F04, (u32 )&lbl_1_data_F14, 0x00010000, (u32 )&lbl_1_data_3364,
};

u32 lbl_1_data_3364[4] = {
    (u32 )&lbl_1_data_F24, (u32 )&lbl_1_data_F34, 0x00010000, (u32 )&lbl_1_data_3374,
};

u32 lbl_1_data_3374[4] = {
    (u32 )&lbl_1_data_F44, (u32 )&lbl_1_data_F54, 0x00010000, (u32 )&lbl_1_data_3384,
};

u32 lbl_1_data_3384[4] = {
    (u32 )&lbl_1_data_F64, (u32 )&lbl_1_data_F74, 0x00010000, (u32 )&lbl_1_data_3394,
};

u32 lbl_1_data_3394[4] = {
    (u32 )&lbl_1_data_F84, (u32 )&lbl_1_data_F94, 0x00010000, (u32 )&lbl_1_data_4124,
};

u32 lbl_1_data_33A4[4] = {
    (u32 )&lbl_1_data_FA4, (u32 )&lbl_1_data_FB4, 0x00010000, (u32 )&lbl_1_data_33B4,
};

u32 lbl_1_data_33B4[4] = {
    (u32 )&lbl_1_data_FC4, (u32 )&lbl_1_data_FD4, 0x00010000, (u32 )&lbl_1_data_33C4,
};

u32 lbl_1_data_33C4[4] = {
    (u32 )&lbl_1_data_FE4, (u32 )&lbl_1_data_FF4, 0x00010000, (u32 )&lbl_1_data_33D4,
};

u32 lbl_1_data_33D4[4] = {
    (u32 )&lbl_1_data_1004, (u32 )&lbl_1_data_1014, 0x00010000, (u32 )&lbl_1_data_33E4,
};

u32 lbl_1_data_33E4[4] = {
    (u32 )&lbl_1_data_1024, (u32 )&lbl_1_data_1034, 0x00010000, (u32 )&lbl_1_data_33F4,
};

u32 lbl_1_data_33F4[4] = {
    (u32 )&lbl_1_data_1050, (u32 )&lbl_1_data_1078, 0x00010000, (u32 )&lbl_1_data_3404,
};

u32 lbl_1_data_3404[4] = {
    (u32 )&lbl_1_data_1088, (u32 )&lbl_1_data_1098, 0x00010000, (u32 )&lbl_1_data_3414,
};

u32 lbl_1_data_3414[4] = {
    (u32 )&lbl_1_data_10A8, (u32 )&lbl_1_data_10B8, 0x00010000, (u32 )&lbl_1_data_3424,
};

u32 lbl_1_data_3424[4] = {
    (u32 )&lbl_1_data_10C8, (u32 )&lbl_1_data_10D8, 0x00010000, (u32 )&lbl_1_data_3434,
};

u32 lbl_1_data_3434[4] = {
    (u32 )&lbl_1_data_10E8, (u32 )&lbl_1_data_10F8, 0x00010000, (u32 )&lbl_1_data_3444,
};

u32 lbl_1_data_3444[4] = {
    (u32 )&lbl_1_data_1108, (u32 )&lbl_1_data_1118, 0x00010000, (u32 )&lbl_1_data_3454,
};

u32 lbl_1_data_3454[4] = {
    (u32 )&lbl_1_data_1128, (u32 )&lbl_1_data_1138, 0x00010000, (u32 )&lbl_1_data_4124,
};

u32 lbl_1_data_3464[4] = {
    (u32 )&lbl_1_data_1148, (u32 )&lbl_1_data_117C, 0x00010000, (u32 )&lbl_1_data_3474,
};

u32 lbl_1_data_3474[4] = {
    (u32 )&lbl_1_data_118C, (u32 )&lbl_1_data_119C, 0x00010000, (u32 )&lbl_1_data_3484,
};

u32 lbl_1_data_3484[4] = {
    (u32 )&lbl_1_data_11AC, (u32 )&lbl_1_data_11BC, 0x00010000, (u32 )&lbl_1_data_3494,
};

u32 lbl_1_data_3494[4] = {
    (u32 )&lbl_1_data_11CC, (u32 )&lbl_1_data_11DC, 0x00010000, (u32 )&lbl_1_data_34A4,
};

u32 lbl_1_data_34A4[4] = {
    (u32 )&lbl_1_data_11EC, (u32 )&lbl_1_data_11FC, 0x00010000, (u32 )&lbl_1_data_34B4,
};

u32 lbl_1_data_34B4[4] = {
    (u32 )&lbl_1_data_120C, (u32 )&lbl_1_data_121C, 0x00010000, (u32 )&lbl_1_data_34C4,
};

u32 lbl_1_data_34C4[4] = {
    (u32 )&lbl_1_data_122C, (u32 )&lbl_1_data_123C, 0x00010000, (u32 )&lbl_1_data_34D4,
};

u32 lbl_1_data_34D4[4] = {
    (u32 )&lbl_1_data_124C, (u32 )&lbl_1_data_125C, 0x00010000, (u32 )&lbl_1_data_34E4,
};

u32 lbl_1_data_34E4[4] = {
    (u32 )&lbl_1_data_126C, (u32 )&lbl_1_data_12A0, 0x00010000, (u32 )&lbl_1_data_34F4,
};

u32 lbl_1_data_34F4[4] = {
    (u32 )&lbl_1_data_12B0, (u32 )&lbl_1_data_12C0, 0x00010000, (u32 )&lbl_1_data_3504,
};

u32 lbl_1_data_3504[4] = {
    (u32 )&lbl_1_data_12D0, (u32 )&lbl_1_data_12E0, 0x00010000, (u32 )&lbl_1_data_3514,
};

u32 lbl_1_data_3514[4] = {
    (u32 )&lbl_1_data_12F0, (u32 )&lbl_1_data_1300, 0x00010000, (u32 )&lbl_1_data_4124,
};

u32 lbl_1_data_3524[4] = {
    (u32 )&lbl_1_data_1310, (u32 )&lbl_1_data_1350, 0x00010000, (u32 )&lbl_1_data_3534,
};

u32 lbl_1_data_3534[4] = {
    (u32 )&lbl_1_data_1360, (u32 )&lbl_1_data_1370, 0x00010000, (u32 )&lbl_1_data_3544,
};

u32 lbl_1_data_3544[4] = {
    (u32 )&lbl_1_data_1380, (u32 )&lbl_1_data_1390, 0x00010000, (u32 )&lbl_1_data_3554,
};

u32 lbl_1_data_3554[4] = {
    (u32 )&lbl_1_data_13A0, (u32 )&lbl_1_data_13B0, 0x00010000, (u32 )&lbl_1_data_3564,
};

u32 lbl_1_data_3564[4] = {
    (u32 )&lbl_1_data_13C0, (u32 )&lbl_1_data_13D0, 0x00010000, (u32 )&lbl_1_data_3574,
};

u32 lbl_1_data_3574[4] = {
    (u32 )&lbl_1_data_13E0, (u32 )&lbl_1_data_13F0, 0x00010000, (u32 )&lbl_1_data_3584,
};

u32 lbl_1_data_3584[4] = {
    (u32 )&lbl_1_data_1400, (u32 )&lbl_1_data_1428, 0x00010000, (u32 )&lbl_1_data_3594,
};

u32 lbl_1_data_3594[4] = {
    (u32 )&lbl_1_data_1438, (u32 )&lbl_1_data_1448, 0x00010000, (u32 )&lbl_1_data_35A4,
};

u32 lbl_1_data_35A4[4] = {
    (u32 )&lbl_1_data_1458, (u32 )&lbl_1_data_1468, 0x00010000, (u32 )&lbl_1_data_35B4,
};

u32 lbl_1_data_35B4[4] = {
    (u32 )&lbl_1_data_1478, (u32 )&lbl_1_data_1488, 0x00010000, (u32 )&lbl_1_data_35C4,
};

u32 lbl_1_data_35C4[4] = {
    (u32 )&lbl_1_data_1498, (u32 )&lbl_1_data_14A8, 0x00010000, (u32 )&lbl_1_data_35D4,
};

u32 lbl_1_data_35D4[4] = {
    (u32 )&lbl_1_data_14B8, (u32 )&lbl_1_data_14C8, 0x00010000, (u32 )&lbl_1_data_4124,
};

u32 lbl_1_data_35E4[4] = {
    (u32 )&lbl_1_data_14D8, (u32 )&lbl_1_data_14E8, 0x00010000, (u32 )&lbl_1_data_35F4,
};

u32 lbl_1_data_35F4[4] = {
    (u32 )&lbl_1_data_14F8, (u32 )&lbl_1_data_1520, 0x00010000, (u32 )&lbl_1_data_3604,
};

u32 lbl_1_data_3604[4] = {
    (u32 )&lbl_1_data_1530, (u32 )&lbl_1_data_1540, 0x00010000, (u32 )&lbl_1_data_3614,
};

u32 lbl_1_data_3614[4] = {
    (u32 )&lbl_1_data_1550, (u32 )&lbl_1_data_1560, 0x00010000, (u32 )&lbl_1_data_3624,
};

u32 lbl_1_data_3624[4] = {
    (u32 )&lbl_1_data_1570, (u32 )&lbl_1_data_1580, 0x00010000, (u32 )&lbl_1_data_3634,
};

u32 lbl_1_data_3634[4] = {
    (u32 )&lbl_1_data_1590, (u32 )&lbl_1_data_15A0, 0x00010000, (u32 )&lbl_1_data_3644,
};

u32 lbl_1_data_3644[4] = {
    (u32 )&lbl_1_data_15B0, (u32 )&lbl_1_data_15C0, 0x00010000, (u32 )&lbl_1_data_3654,
};

u32 lbl_1_data_3654[4] = {
    (u32 )&lbl_1_data_15D0, (u32 )&lbl_1_data_15E0, 0x00010000, (u32 )&lbl_1_data_3664,
};

u32 lbl_1_data_3664[4] = {
    (u32 )&lbl_1_data_15F0, (u32 )&lbl_1_data_1600, 0x00010000, (u32 )&lbl_1_data_3674,
};

u32 lbl_1_data_3674[4] = {
    (u32 )&lbl_1_data_1610, (u32 )&lbl_1_data_1620, 0x00010000, (u32 )&lbl_1_data_3684,
};

u32 lbl_1_data_3684[4] = {
    (u32 )&lbl_1_data_1630, (u32 )&lbl_1_data_1640, 0x00010000, (u32 )&lbl_1_data_3694,
};

u32 lbl_1_data_3694[4] = {
    (u32 )&lbl_1_data_1650, (u32 )&lbl_1_data_166C, 0x00010000, (u32 )&lbl_1_data_4124,
};

u32 lbl_1_data_36A4[4] = {
    (u32 )&lbl_1_data_167C, (u32 )&lbl_1_data_1698, 0x00010000, (u32 )&lbl_1_data_36B4,
};

u32 lbl_1_data_36B4[4] = {
    (u32 )&lbl_1_data_16A8, (u32 )&lbl_1_data_16B8, 0x00010000, (u32 )&lbl_1_data_36C4,
};

u32 lbl_1_data_36C4[4] = {
    (u32 )&lbl_1_data_16C8, (u32 )&lbl_1_data_16D8, 0x00010000, (u32 )&lbl_1_data_36D4,
};

u32 lbl_1_data_36D4[4] = {
    (u32 )&lbl_1_data_16E8, (u32 )&lbl_1_data_16F8, 0x00010000, (u32 )&lbl_1_data_36E4,
};

u32 lbl_1_data_36E4[4] = {
    (u32 )&lbl_1_data_1708, (u32 )&lbl_1_data_1718, 0x00010000, (u32 )&lbl_1_data_36F4,
};

u32 lbl_1_data_36F4[4] = {
    (u32 )&lbl_1_data_1728, (u32 )&lbl_1_data_1738, 0x00010000, (u32 )&lbl_1_data_3704,
};

u32 lbl_1_data_3704[4] = {
    (u32 )&lbl_1_data_1748, (u32 )&lbl_1_data_1758, 0x00010000, (u32 )&lbl_1_data_3714,
};

u32 lbl_1_data_3714[4] = {
    (u32 )&lbl_1_data_1768, (u32 )&lbl_1_data_1778, 0x00010000, (u32 )&lbl_1_data_3724,
};

u32 lbl_1_data_3724[4] = {
    (u32 )&lbl_1_data_1788, (u32 )&lbl_1_data_17A4, 0x00010000, (u32 )&lbl_1_data_3734,
};

u32 lbl_1_data_3734[4] = {
    (u32 )&lbl_1_data_17B4, (u32 )&lbl_1_data_17C4, 0x00010000, (u32 )&lbl_1_data_3744,
};

u32 lbl_1_data_3744[4] = {
    (u32 )&lbl_1_data_17D4, (u32 )&lbl_1_data_17E4, 0x00010000, (u32 )&lbl_1_data_3754,
};

u32 lbl_1_data_3754[4] = {
    (u32 )&lbl_1_data_17F4, (u32 )&lbl_1_data_1804, 0x00010000, (u32 )&lbl_1_data_4124,
};

u32 lbl_1_data_3764[4] = {
    (u32 )&lbl_1_data_1814, (u32 )&lbl_1_data_1824, 0x00010000, (u32 )&lbl_1_data_3774,
};

u32 lbl_1_data_3774[4] = {
    (u32 )&lbl_1_data_1834, (u32 )&lbl_1_data_1844, 0x00010000, (u32 )&lbl_1_data_3784,
};

u32 lbl_1_data_3784[4] = {
    (u32 )&lbl_1_data_1854, (u32 )&lbl_1_data_1864, 0x00010000, (u32 )&lbl_1_data_3794,
};

u32 lbl_1_data_3794[4] = {
    (u32 )&lbl_1_data_1874, (u32 )&lbl_1_data_1884, 0x00010000, (u32 )&lbl_1_data_37A4,
};

u32 lbl_1_data_37A4[4] = {
    (u32 )&lbl_1_data_1894, (u32 )&lbl_1_data_18A4, 0x00010000, (u32 )&lbl_1_data_37B4,
};

u32 lbl_1_data_37B4[4] = {
    (u32 )&lbl_1_data_18B4, (u32 )&lbl_1_data_18C4, 0x00010000, (u32 )&lbl_1_data_37C4,
};

u32 lbl_1_data_37C4[4] = {
    (u32 )&lbl_1_data_18D4, (u32 )&lbl_1_data_18E4, 0x00010000, (u32 )&lbl_1_data_37D4,
};

u32 lbl_1_data_37D4[4] = {
    (u32 )&lbl_1_data_18F4, (u32 )&lbl_1_data_1904, 0x00010000, (u32 )&lbl_1_data_37E4,
};

u32 lbl_1_data_37E4[4] = {
    (u32 )&lbl_1_data_1914, (u32 )&lbl_1_data_1924, 0x00010000, (u32 )&lbl_1_data_37F4,
};

u32 lbl_1_data_37F4[4] = {
    (u32 )&lbl_1_data_1934, (u32 )&lbl_1_data_1944, 0x00010000, (u32 )&lbl_1_data_3804,
};

u32 lbl_1_data_3804[4] = {
    (u32 )&lbl_1_data_1954, (u32 )&lbl_1_data_1964, 0x00010000, (u32 )&lbl_1_data_3814,
};

u32 lbl_1_data_3814[4] = {
    (u32 )&lbl_1_data_1974, (u32 )&lbl_1_data_1984, 0x00010000, (u32 )&lbl_1_data_4124,
};

u32 lbl_1_data_3824[4] = {
    (u32 )&lbl_1_data_1994, (u32 )&lbl_1_data_19B0, 0x00010000, (u32 )&lbl_1_data_3834,
};

u32 lbl_1_data_3834[4] = {
    (u32 )&lbl_1_data_19C0, (u32 )&lbl_1_data_19D0, 0x00010000, (u32 )&lbl_1_data_3844,
};

u32 lbl_1_data_3844[4] = {
    (u32 )&lbl_1_data_19E0, (u32 )&lbl_1_data_19F0, 0x00010000, (u32 )&lbl_1_data_3854,
};

u32 lbl_1_data_3854[4] = {
    (u32 )&lbl_1_data_1A00, (u32 )&lbl_1_data_1A10, 0x00010000, (u32 )&lbl_1_data_3864,
};

u32 lbl_1_data_3864[4] = {
    (u32 )&lbl_1_data_1A20, (u32 )&lbl_1_data_1A6C, 0x00010000, (u32 )&lbl_1_data_3874,
};

u32 lbl_1_data_3874[4] = {
    (u32 )&lbl_1_data_1A7C, (u32 )&lbl_1_data_1A8C, 0x00010000, (u32 )&lbl_1_data_3884,
};

u32 lbl_1_data_3884[4] = {
    (u32 )&lbl_1_data_1A9C, (u32 )&lbl_1_data_1AAC, 0x00010000, (u32 )&lbl_1_data_3894,
};

u32 lbl_1_data_3894[4] = {
    (u32 )&lbl_1_data_1ABC, (u32 )&lbl_1_data_1ACC, 0x00010000, (u32 )&lbl_1_data_38A4,
};

u32 lbl_1_data_38A4[4] = {
    (u32 )&lbl_1_data_1ADC, (u32 )&lbl_1_data_1AEC, 0x00010000, (u32 )&lbl_1_data_38B4,
};

u32 lbl_1_data_38B4[4] = {
    (u32 )&lbl_1_data_1AFC, (u32 )&lbl_1_data_1B0C, 0x00010000, (u32 )&lbl_1_data_38C4,
};

u32 lbl_1_data_38C4[4] = {
    (u32 )&lbl_1_data_1B1C, (u32 )&lbl_1_data_1B2C, 0x00010000, (u32 )&lbl_1_data_38D4,
};

u32 lbl_1_data_38D4[4] = {
    (u32 )&lbl_1_data_1B3C, (u32 )&lbl_1_data_1B4C, 0x00010000, (u32 )&lbl_1_data_4124,
};

u32 lbl_1_data_38E4[4] = {
    (u32 )&lbl_1_data_1B5C, (u32 )&lbl_1_data_1B84, 0x00010000, (u32 )&lbl_1_data_38F4,
};

u32 lbl_1_data_38F4[4] = {
    (u32 )&lbl_1_data_1B94, (u32 )&lbl_1_data_1BA4, 0x00010000, (u32 )&lbl_1_data_3904,
};

u32 lbl_1_data_3904[4] = {
    (u32 )&lbl_1_data_1BB4, (u32 )&lbl_1_data_1BC4, 0x00010000, (u32 )&lbl_1_data_3914,
};

u32 lbl_1_data_3914[4] = {
    (u32 )&lbl_1_data_1BD4, (u32 )&lbl_1_data_1BE4, 0x00010000, (u32 )&lbl_1_data_3924,
};

u32 lbl_1_data_3924[4] = {
    (u32 )&lbl_1_data_1BF4, (u32 )&lbl_1_data_1C1C, 0x00010000, (u32 )&lbl_1_data_3934,
};

u32 lbl_1_data_3934[4] = {
    (u32 )&lbl_1_data_1C2C, (u32 )&lbl_1_data_1C3C, 0x00010000, (u32 )&lbl_1_data_3944,
};

u32 lbl_1_data_3944[4] = {
    (u32 )&lbl_1_data_1C4C, (u32 )&lbl_1_data_1C5C, 0x00010000, (u32 )&lbl_1_data_3954,
};

u32 lbl_1_data_3954[4] = {
    (u32 )&lbl_1_data_1C6C, (u32 )&lbl_1_data_1C7C, 0x00010000, (u32 )&lbl_1_data_3964,
};

u32 lbl_1_data_3964[4] = {
    (u32 )&lbl_1_data_1C8C, (u32 )&lbl_1_data_1C9C, 0x00010000, (u32 )&lbl_1_data_3974,
};

u32 lbl_1_data_3974[4] = {
    (u32 )&lbl_1_data_1CAC, (u32 )&lbl_1_data_1CBC, 0x00010000, (u32 )&lbl_1_data_3984,
};

u32 lbl_1_data_3984[4] = {
    (u32 )&lbl_1_data_1CCC, (u32 )&lbl_1_data_1CDC, 0x00010000, (u32 )&lbl_1_data_3994,
};

u32 lbl_1_data_3994[4] = {
    (u32 )&lbl_1_data_1CEC, (u32 )&lbl_1_data_1CFC, 0x00010000, (u32 )&lbl_1_data_4124,
};

u32 lbl_1_data_39A4[4] = {
    (u32 )&lbl_1_data_1D0C, (u32 )&lbl_1_data_1D40, 0x00010000, (u32 )&lbl_1_data_39B4,
};

u32 lbl_1_data_39B4[4] = {
    (u32 )&lbl_1_data_1D50, (u32 )&lbl_1_data_1D6C, 0x00010000, (u32 )&lbl_1_data_39C4,
};

u32 lbl_1_data_39C4[4] = {
    (u32 )&lbl_1_data_1D7C, (u32 )&lbl_1_data_1D8C, 0x00010000, (u32 )&lbl_1_data_39D4,
};

u32 lbl_1_data_39D4[4] = {
    (u32 )&lbl_1_data_1D9C, (u32 )&lbl_1_data_1DB8, 0x00010000, (u32 )&lbl_1_data_39E4,
};

u32 lbl_1_data_39E4[4] = {
    (u32 )&lbl_1_data_1DC8, (u32 )&lbl_1_data_1DD8, 0x00010000, (u32 )&lbl_1_data_39F4,
};

u32 lbl_1_data_39F4[4] = {
    (u32 )&lbl_1_data_1DE8, (u32 )&lbl_1_data_1DF8, 0x00010000, (u32 )&lbl_1_data_3A04,
};

u32 lbl_1_data_3A04[4] = {
    (u32 )&lbl_1_data_1E08, (u32 )&lbl_1_data_1E18, 0x00010000, (u32 )&lbl_1_data_3A14,
};

u32 lbl_1_data_3A14[4] = {
    (u32 )&lbl_1_data_1E28, (u32 )&lbl_1_data_1E38, 0x00010000, (u32 )&lbl_1_data_3A24,
};

u32 lbl_1_data_3A24[4] = {
    (u32 )&lbl_1_data_1E48, (u32 )&lbl_1_data_1E58, 0x00010000, (u32 )&lbl_1_data_3A34,
};

u32 lbl_1_data_3A34[4] = {
    (u32 )&lbl_1_data_1E68, (u32 )&lbl_1_data_1E78, 0x00010000, (u32 )&lbl_1_data_3A44,
};

u32 lbl_1_data_3A44[4] = {
    (u32 )&lbl_1_data_1E88, (u32 )&lbl_1_data_1E98, 0x00010000, (u32 )&lbl_1_data_3A54,
};

u32 lbl_1_data_3A54[4] = {
    (u32 )&lbl_1_data_1EA8, (u32 )&lbl_1_data_1EB8, 0x00010000, (u32 )&lbl_1_data_4124,
};

u32 lbl_1_data_3A64[4] = {
    (u32 )&lbl_1_data_1EC8, (u32 )&lbl_1_data_1EF0, 0x00010000, (u32 )&lbl_1_data_3A74,
};

u32 lbl_1_data_3A74[4] = {
    (u32 )&lbl_1_data_1F00, (u32 )&lbl_1_data_1F10, 0x00010000, (u32 )&lbl_1_data_3A84,
};

u32 lbl_1_data_3A84[4] = {
    (u32 )&lbl_1_data_1F20, (u32 )&lbl_1_data_1F30, 0x00010000, (u32 )&lbl_1_data_3A94,
};

u32 lbl_1_data_3A94[4] = {
    (u32 )&lbl_1_data_1F40, (u32 )&lbl_1_data_1F50, 0x00010000, (u32 )&lbl_1_data_3AA4,
};

u32 lbl_1_data_3AA4[4] = {
    (u32 )&lbl_1_data_1F60, (u32 )&lbl_1_data_1F70, 0x00010000, (u32 )&lbl_1_data_3AB4,
};

u32 lbl_1_data_3AB4[4] = {
    (u32 )&lbl_1_data_1F80, (u32 )&lbl_1_data_1F90, 0x00010000, (u32 )&lbl_1_data_3AC4,
};

u32 lbl_1_data_3AC4[4] = {
    (u32 )&lbl_1_data_1FA0, (u32 )&lbl_1_data_1FB0, 0x00010000, (u32 )&lbl_1_data_3AD4,
};

u32 lbl_1_data_3AD4[4] = {
    (u32 )&lbl_1_data_1FC0, (u32 )&lbl_1_data_1FDC, 0x00010000, (u32 )&lbl_1_data_3AE4,
};

u32 lbl_1_data_3AE4[4] = {
    (u32 )&lbl_1_data_1FEC, (u32 )&lbl_1_data_1FFC, 0x00010000, (u32 )&lbl_1_data_3AF4,
};

u32 lbl_1_data_3AF4[4] = {
    (u32 )&lbl_1_data_200C, (u32 )&lbl_1_data_201C, 0x00010000, (u32 )&lbl_1_data_3B04,
};

u32 lbl_1_data_3B04[4] = {
    (u32 )&lbl_1_data_202C, (u32 )&lbl_1_data_203C, 0x00010000, (u32 )&lbl_1_data_3B14,
};

u32 lbl_1_data_3B14[4] = {
    (u32 )&lbl_1_data_204C, (u32 )&lbl_1_data_205C, 0x00010000, (u32 )&lbl_1_data_4124,
};

u32 lbl_1_data_3B24[5] = {
    (u32 )&lbl_1_data_206C, (u32 )&lbl_1_data_2088, 0x00020000, (u32 )&lbl_1_data_3B38, 
    (u32 )&lbl_1_data_3B48,
};

u32 lbl_1_data_3B38[4] = {
    (u32 )&lbl_1_data_2098, (u32 )&lbl_1_data_20A8, 0x00010000, (u32 )&lbl_1_data_3B58,
};

u32 lbl_1_data_3B48[4] = {
    (u32 )&lbl_1_data_20B8, (u32 )&lbl_1_data_20C8, 0x00010000, (u32 )&lbl_1_data_3B58,
};

u32 lbl_1_data_3B58[4] = {
    (u32 )&lbl_1_data_20D8, (u32 )&lbl_1_data_20E8, 0x00010000, (u32 )&lbl_1_data_3B68,
};

u32 lbl_1_data_3B68[5] = {
    (u32 )&lbl_1_data_20F8, (u32 )&lbl_1_data_2138, 0x00020000, (u32 )&lbl_1_data_3B7C, 
    (u32 )&lbl_1_data_3B8C,
};

u32 lbl_1_data_3B7C[4] = {
    (u32 )&lbl_1_data_2148, (u32 )&lbl_1_data_2158, 0x00010000, (u32 )&lbl_1_data_3B9C,
};

u32 lbl_1_data_3B8C[4] = {
    (u32 )&lbl_1_data_2168, (u32 )&lbl_1_data_2178, 0x00010000, (u32 )&lbl_1_data_3B9C,
};

u32 lbl_1_data_3B9C[4] = {
    (u32 )&lbl_1_data_2188, (u32 )&lbl_1_data_21A4, 0x00010000, (u32 )&lbl_1_data_3BAC,
};

u32 lbl_1_data_3BAC[4] = {
    (u32 )&lbl_1_data_21B4, (u32 )&lbl_1_data_21F4, 0x00010000, (u32 )&lbl_1_data_3BBC,
};

u32 lbl_1_data_3BBC[5] = {
    (u32 )&lbl_1_data_2204, (u32 )&lbl_1_data_222C, 0x00020000, (u32 )&lbl_1_data_3BD0, 
    (u32 )&lbl_1_data_3BE0,
};

u32 lbl_1_data_3BD0[4] = {
    (u32 )&lbl_1_data_223C, (u32 )&lbl_1_data_224C, 0x00010000, (u32 )&lbl_1_data_3BF0,
};

u32 lbl_1_data_3BE0[4] = {
    (u32 )&lbl_1_data_225C, (u32 )&lbl_1_data_226C, 0x00010000, (u32 )&lbl_1_data_3BF0,
};

u32 lbl_1_data_3BF0[4] = {
    (u32 )&lbl_1_data_227C, (u32 )&lbl_1_data_228C, 0x00010000, (u32 )&lbl_1_data_3C00,
};

u32 lbl_1_data_3C00[4] = {
    (u32 )&lbl_1_data_229C, (u32 )&lbl_1_data_22AC, 0x00010000, (u32 )&lbl_1_data_3C10,
};

u32 lbl_1_data_3C10[4] = {
    (u32 )&lbl_1_data_22BC, (u32 )&lbl_1_data_22CC, 0x00010000, (u32 )&lbl_1_data_3C20,
};

u32 lbl_1_data_3C20[4] = {
    (u32 )&lbl_1_data_22DC, (u32 )&lbl_1_data_22EC, 0x00010000, (u32 )&lbl_1_data_3C30,
};

u32 lbl_1_data_3C30[4] = {
    (u32 )&lbl_1_data_22FC, (u32 )&lbl_1_data_230C, 0x00010000, (u32 )&lbl_1_data_4124,
};

u32 lbl_1_data_3C40[4] = {
    (u32 )&lbl_1_data_231C, (u32 )&lbl_1_data_2344, 0x00010000, (u32 )&lbl_1_data_3C50,
};

u32 lbl_1_data_3C50[5] = {
    (u32 )&lbl_1_data_2354, (u32 )&lbl_1_data_2364, 0x00020000, (u32 )&lbl_1_data_3C64, 
    (u32 )&lbl_1_data_3C74,
};

u32 lbl_1_data_3C64[4] = {
    (u32 )&lbl_1_data_2374, (u32 )&lbl_1_data_239C, 0x00010000, (u32 )&lbl_1_data_3C84,
};

u32 lbl_1_data_3C74[4] = {
    (u32 )&lbl_1_data_23AC, (u32 )&lbl_1_data_23D4, 0x00010000, (u32 )&lbl_1_data_3C84,
};

u32 lbl_1_data_3C84[4] = {
    (u32 )&lbl_1_data_23E4, (u32 )&lbl_1_data_23F4, 0x00010000, (u32 )&lbl_1_data_3C94,
};

u32 lbl_1_data_3C94[5] = {
    (u32 )&lbl_1_data_2404, (u32 )&lbl_1_data_2414, 0x00020000, (u32 )&lbl_1_data_3CA8, 
    (u32 )&lbl_1_data_3CB8,
};

u32 lbl_1_data_3CA8[4] = {
    (u32 )&lbl_1_data_2424, (u32 )&lbl_1_data_2434, 0x00010000, (u32 )&lbl_1_data_3CC8,
};

u32 lbl_1_data_3CB8[4] = {
    (u32 )&lbl_1_data_2444, (u32 )&lbl_1_data_2454, 0x00010000, (u32 )&lbl_1_data_3CC8,
};

u32 lbl_1_data_3CC8[4] = {
    (u32 )&lbl_1_data_2464, (u32 )&lbl_1_data_2474, 0x00010000, (u32 )&lbl_1_data_3CD8,
};

u32 lbl_1_data_3CD8[4] = {
    (u32 )&lbl_1_data_2484, (u32 )&lbl_1_data_2494, 0x00010000, (u32 )&lbl_1_data_3CE8,
};

u32 lbl_1_data_3CE8[4] = {
    (u32 )&lbl_1_data_24A4, (u32 )&lbl_1_data_24B4, 0x00010000, (u32 )&lbl_1_data_3CF8,
};

u32 lbl_1_data_3CF8[4] = {
    (u32 )&lbl_1_data_24C4, (u32 )&lbl_1_data_24D4, 0x00010000, (u32 )&lbl_1_data_3D08,
};

u32 lbl_1_data_3D08[5] = {
    (u32 )&lbl_1_data_24E4, (u32 )&lbl_1_data_24F4, 0x00020000, (u32 )&lbl_1_data_3D1C, 
    (u32 )&lbl_1_data_3D2C,
};

u32 lbl_1_data_3D1C[4] = {
    (u32 )&lbl_1_data_2504, (u32 )&lbl_1_data_2514, 0x00010000, (u32 )&lbl_1_data_3D3C,
};

u32 lbl_1_data_3D2C[4] = {
    (u32 )&lbl_1_data_2524, (u32 )&lbl_1_data_2534, 0x00010000, (u32 )&lbl_1_data_3D3C,
};

u32 lbl_1_data_3D3C[4] = {
    (u32 )&lbl_1_data_2544, (u32 )&lbl_1_data_2554, 0x00010000, (u32 )&lbl_1_data_3D4C,
};

u32 lbl_1_data_3D4C[4] = {
    (u32 )&lbl_1_data_2564, (u32 )&lbl_1_data_2574, 0x00010000, (u32 )&lbl_1_data_4124,
};

u32 lbl_1_data_3D5C[4] = {
    (u32 )&lbl_1_data_2584, (u32 )&lbl_1_data_25A0, 0x00010000, (u32 )&lbl_1_data_3D6C,
};

u32 lbl_1_data_3D6C[4] = {
    (u32 )&lbl_1_data_25B0, (u32 )&lbl_1_data_25C0, 0x00010000, (u32 )&lbl_1_data_3D7C,
};

u32 lbl_1_data_3D7C[5] = {
    (u32 )&lbl_1_data_25D0, (u32 )&lbl_1_data_25F8, 0x00020000, (u32 )&lbl_1_data_3D90, 
    (u32 )&lbl_1_data_3DA0,
};

u32 lbl_1_data_3D90[4] = {
    (u32 )&lbl_1_data_2608, (u32 )&lbl_1_data_2624, 0x00010000, (u32 )&lbl_1_data_3DB0,
};

u32 lbl_1_data_3DA0[4] = {
    (u32 )&lbl_1_data_2634, (u32 )&lbl_1_data_2650, 0x00010000, (u32 )&lbl_1_data_3DB0,
};

u32 lbl_1_data_3DB0[4] = {
    (u32 )&lbl_1_data_2660, (u32 )&lbl_1_data_2670, 0x00010000, (u32 )&lbl_1_data_3DC0,
};

u32 lbl_1_data_3DC0[4] = {
    (u32 )&lbl_1_data_2680, (u32 )&lbl_1_data_26B4, 0x00010000, (u32 )&lbl_1_data_3DD0,
};

u32 lbl_1_data_3DD0[4] = {
    (u32 )&lbl_1_data_26C4, (u32 )&lbl_1_data_26D4, 0x00010000, (u32 )&lbl_1_data_3DE0,
};

u32 lbl_1_data_3DE0[4] = {
    (u32 )&lbl_1_data_26E4, (u32 )&lbl_1_data_26F4, 0x00010000, (u32 )&lbl_1_data_3DF0,
};

u32 lbl_1_data_3DF0[4] = {
    (u32 )&lbl_1_data_2704, (u32 )&lbl_1_data_2738, 0x00010000, (u32 )&lbl_1_data_3E00,
};

u32 lbl_1_data_3E00[4] = {
    (u32 )&lbl_1_data_2748, (u32 )&lbl_1_data_2758, 0x00010000, (u32 )&lbl_1_data_3E10,
};

u32 lbl_1_data_3E10[4] = {
    (u32 )&lbl_1_data_2768, (u32 )&lbl_1_data_2778, 0x00010000, (u32 )&lbl_1_data_3E20,
};

u32 lbl_1_data_3E20[4] = {
    (u32 )&lbl_1_data_2788, (u32 )&lbl_1_data_2798, 0x00010000, (u32 )&lbl_1_data_3E30,
};

u32 lbl_1_data_3E30[4] = {
    (u32 )&lbl_1_data_27A8, (u32 )&lbl_1_data_27B8, 0x00010000, (u32 )&lbl_1_data_4124,
};

u32 lbl_1_data_3E40[4] = {
    (u32 )&lbl_1_data_27C8, (u32 )&lbl_1_data_27E4, 0x00010000, (u32 )&lbl_1_data_3E50,
};

u32 lbl_1_data_3E50[4] = {
    (u32 )&lbl_1_data_27F4, (u32 )&lbl_1_data_2804, 0x00010000, (u32 )&lbl_1_data_3E60,
};

u32 lbl_1_data_3E60[5] = {
    (u32 )&lbl_1_data_2814, (u32 )&lbl_1_data_2824, 0x00020000, (u32 )&lbl_1_data_3E74, 
    (u32 )&lbl_1_data_3E84,
};

u32 lbl_1_data_3E74[4] = {
    (u32 )&lbl_1_data_2834, (u32 )&lbl_1_data_2844, 0x00010000, (u32 )&lbl_1_data_3E94,
};

u32 lbl_1_data_3E84[4] = {
    (u32 )&lbl_1_data_2854, (u32 )&lbl_1_data_2864, 0x00010000, (u32 )&lbl_1_data_3E94,
};

u32 lbl_1_data_3E94[4] = {
    (u32 )&lbl_1_data_2874, (u32 )&lbl_1_data_2890, 0x00010000, (u32 )&lbl_1_data_3EA4,
};

u32 lbl_1_data_3EA4[5] = {
    (u32 )&lbl_1_data_28A0, (u32 )&lbl_1_data_28B0, 0x00020000, (u32 )&lbl_1_data_3EB8, 
    (u32 )&lbl_1_data_3EC8,
};

u32 lbl_1_data_3EB8[4] = {
    (u32 )&lbl_1_data_28C0, (u32 )&lbl_1_data_28D0, 0x00010000, (u32 )&lbl_1_data_3ED8,
};

u32 lbl_1_data_3EC8[4] = {
    (u32 )&lbl_1_data_28E0, (u32 )&lbl_1_data_28F0, 0x00010000, (u32 )&lbl_1_data_3ED8,
};

u32 lbl_1_data_3ED8[4] = {
    (u32 )&lbl_1_data_2900, (u32 )&lbl_1_data_291C, 0x00010000, (u32 )&lbl_1_data_3EE8,
};

u32 lbl_1_data_3EE8[4] = {
    (u32 )&lbl_1_data_292C, (u32 )&lbl_1_data_293C, 0x00010000, (u32 )&lbl_1_data_3EF8,
};

u32 lbl_1_data_3EF8[4] = {
    (u32 )&lbl_1_data_294C, (u32 )&lbl_1_data_295C, 0x00010000, (u32 )&lbl_1_data_3F08,
};

u32 lbl_1_data_3F08[4] = {
    (u32 )&lbl_1_data_296C, (u32 )&lbl_1_data_297C, 0x00010000, (u32 )&lbl_1_data_3F18,
};

u32 lbl_1_data_3F18[4] = {
    (u32 )&lbl_1_data_298C, (u32 )&lbl_1_data_299C, 0x00010000, (u32 )&lbl_1_data_3F28,
};

u32 lbl_1_data_3F28[4] = {
    (u32 )&lbl_1_data_29AC, (u32 )&lbl_1_data_29BC, 0x00010000, (u32 )&lbl_1_data_3F38,
};

u32 lbl_1_data_3F38[4] = {
    (u32 )&lbl_1_data_29CC, (u32 )&lbl_1_data_29DC, 0x00010000, (u32 )&lbl_1_data_4124,
};

u32 lbl_1_data_3F48[4] = {
    (u32 )&lbl_1_data_29EC, (u32 )&lbl_1_data_29FC, 0x00010000, (u32 )&lbl_1_data_3F58,
};

u32 lbl_1_data_3F58[4] = {
    (u32 )&lbl_1_data_2A0C, (u32 )&lbl_1_data_2A34, 0x00010000, (u32 )&lbl_1_data_3F68,
};

u32 lbl_1_data_3F68[4] = {
    (u32 )&lbl_1_data_2A44, (u32 )&lbl_1_data_2A54, 0x00010000, (u32 )&lbl_1_data_3F78,
};

u32 lbl_1_data_3F78[5] = {
    (u32 )&lbl_1_data_2A64, (u32 )&lbl_1_data_2A74, 0x00020000, (u32 )&lbl_1_data_3F8C, 
    (u32 )&lbl_1_data_3F9C,
};

u32 lbl_1_data_3F8C[4] = {
    (u32 )&lbl_1_data_2A84, (u32 )&lbl_1_data_2A94, 0x00010000, (u32 )&lbl_1_data_3FAC,
};

u32 lbl_1_data_3F9C[4] = {
    (u32 )&lbl_1_data_2AA4, (u32 )&lbl_1_data_2AB4, 0x00010000, (u32 )&lbl_1_data_3FAC,
};

u32 lbl_1_data_3FAC[4] = {
    (u32 )&lbl_1_data_2AC4, (u32 )&lbl_1_data_2AD4, 0x00010000, (u32 )&lbl_1_data_3FBC,
};

u32 lbl_1_data_3FBC[4] = {
    (u32 )&lbl_1_data_2AE4, (u32 )&lbl_1_data_2B0C, 0x00010000, (u32 )&lbl_1_data_3FCC,
};

u32 lbl_1_data_3FCC[5] = {
    (u32 )&lbl_1_data_2B1C, (u32 )&lbl_1_data_2B2C, 0x00010000, (u32 )&lbl_1_data_3FE0, 
    (u32 )&lbl_1_data_3FF0,
};

u32 lbl_1_data_3FE0[4] = {
    (u32 )&lbl_1_data_2B3C, (u32 )&lbl_1_data_2B4C, 0x00010000, (u32 )&lbl_1_data_4000,
};

u32 lbl_1_data_3FF0[4] = {
    (u32 )&lbl_1_data_2B5C, (u32 )&lbl_1_data_2B6C, 0x00010000, (u32 )&lbl_1_data_4000,
};

u32 lbl_1_data_4000[4] = {
    (u32 )&lbl_1_data_2B7C, (u32 )&lbl_1_data_2B98, 0x00010000, (u32 )&lbl_1_data_4010,
};

u32 lbl_1_data_4010[4] = {
    (u32 )&lbl_1_data_2BA8, (u32 )&lbl_1_data_2BB8, 0x00010000, (u32 )&lbl_1_data_4020,
};

u32 lbl_1_data_4020[4] = {
    (u32 )&lbl_1_data_2BC8, (u32 )&lbl_1_data_2BD8, 0x00010000, (u32 )&lbl_1_data_4030,
};

u32 lbl_1_data_4030[4] = {
    (u32 )&lbl_1_data_2BE8, (u32 )&lbl_1_data_2BF8, 0x00010000, (u32 )&lbl_1_data_4124,
};

u32 lbl_1_data_4040[4] = {
    (u32 )&lbl_1_data_2C08, (u32 )&lbl_1_data_2C48, 0x00010000, (u32 )&lbl_1_data_4050,
};

u32 lbl_1_data_4050[4] = {
    (u32 )&lbl_1_data_2C58, (u32 )&lbl_1_data_2C68, 0x00010000, (u32 )&lbl_1_data_4060,
};

u32 lbl_1_data_4060[5] = {
    (u32 )&lbl_1_data_2C78, (u32 )&lbl_1_data_2C88, 0x00020000, (u32 )&lbl_1_data_4074, 
    (u32 )&lbl_1_data_4084,
};

u32 lbl_1_data_4074[4] = {
    (u32 )&lbl_1_data_2C98, (u32 )&lbl_1_data_2CA8, 0x00010000, (u32 )&lbl_1_data_4094,
};

u32 lbl_1_data_4084[4] = {
    (u32 )&lbl_1_data_2CB8, (u32 )&lbl_1_data_2CC8, 0x00010000, (u32 )&lbl_1_data_4094,
};

u32 lbl_1_data_4094[4] = {
    (u32 )&lbl_1_data_2CD8, (u32 )&lbl_1_data_2CE8, 0x00010000, (u32 )&lbl_1_data_40A4,
};

u32 lbl_1_data_40A4[4] = {
    (u32 )&lbl_1_data_2CF8, (u32 )&lbl_1_data_2D08, 0x00010000, (u32 )&lbl_1_data_40B4,
};

u32 lbl_1_data_40B4[4] = {
    (u32 )&lbl_1_data_2D18, (u32 )&lbl_1_data_2D28, 0x00010000, (u32 )&lbl_1_data_40C4,
};

u32 lbl_1_data_40C4[4] = {
    (u32 )&lbl_1_data_2D38, (u32 )&lbl_1_data_2D54, 0x00010000, (u32 )&lbl_1_data_40D4,
};

u32 lbl_1_data_40D4[4] = {
    (u32 )&lbl_1_data_2D64, (u32 )&lbl_1_data_2D74, 0x00010000, (u32 )&lbl_1_data_40E4,
};

u32 lbl_1_data_40E4[4] = {
    (u32 )&lbl_1_data_2D84, (u32 )&lbl_1_data_2D94, 0x00010000, (u32 )&lbl_1_data_40F4,
};

u32 lbl_1_data_40F4[4] = {
    (u32 )&lbl_1_data_2DA4, (u32 )&lbl_1_data_2DB4, 0x00010000, (u32 )&lbl_1_data_4104,
};

u32 lbl_1_data_4104[4] = {
    (u32 )&lbl_1_data_2DC4, (u32 )&lbl_1_data_2DD4, 0x00010000, (u32 )&lbl_1_data_4114,
};

u32 lbl_1_data_4114[4] = {
    (u32 )&lbl_1_data_2DE4, (u32 )&lbl_1_data_2DF4, 0x00010000, (u32 )&lbl_1_data_4124,
};

u32 lbl_1_data_4124[4] = {
    (u32 )&lbl_1_data_538, 0x00000000, 0x00010000, 0x00000000,
};

u8 *lbl_1_data_4134[290] = {
    (u8 *)&lbl_1_bss_7810, (u8 *)&lbl_1_data_2ED4, (u8 *)&lbl_1_data_2EE4, 
    (u8 *)&lbl_1_data_2EFC, (u8 *)&lbl_1_data_2F0C, (u8 *)&lbl_1_data_2F20, 
    (u8 *)&lbl_1_data_2F30, (u8 *)&lbl_1_data_2F40, (u8 *)&lbl_1_data_2F50, 
    (u8 *)&lbl_1_data_2F60, (u8 *)&lbl_1_data_2F70, (u8 *)&lbl_1_data_2F88, 
    (u8 *)&lbl_1_data_2F98, (u8 *)&lbl_1_data_2FA8, (u8 *)&lbl_1_data_2FB8, 
    (u8 *)&lbl_1_data_2FC8, (u8 *)&lbl_1_data_2FD8, (u8 *)&lbl_1_data_2FE8, 
    (u8 *)&lbl_1_data_2FF8, (u8 *)&lbl_1_data_3008, (u8 *)&lbl_1_data_3020, 
    (u8 *)&lbl_1_data_3030, (u8 *)&lbl_1_data_3044, (u8 *)&lbl_1_data_3054, 
    (u8 *)&lbl_1_data_3064, (u8 *)&lbl_1_data_3074, (u8 *)&lbl_1_data_3084, 
    (u8 *)&lbl_1_data_3094, (u8 *)&lbl_1_data_30A4, (u8 *)&lbl_1_data_30B4, 
    (u8 *)&lbl_1_data_30C4, (u8 *)&lbl_1_data_30D4, (u8 *)&lbl_1_data_30E4, 
    (u8 *)&lbl_1_data_30F4, (u8 *)&lbl_1_data_3104, (u8 *)&lbl_1_data_3114, 
    (u8 *)&lbl_1_data_3124, (u8 *)&lbl_1_data_3134, (u8 *)&lbl_1_data_3144, 
    (u8 *)&lbl_1_data_3154, (u8 *)&lbl_1_data_3164, (u8 *)&lbl_1_data_3174, 
    (u8 *)&lbl_1_data_3184, (u8 *)&lbl_1_data_3194, (u8 *)&lbl_1_data_31A4, 
    (u8 *)&lbl_1_data_31B4, (u8 *)&lbl_1_data_31C4, (u8 *)&lbl_1_data_31D4, 
    (u8 *)&lbl_1_data_31E4, (u8 *)&lbl_1_data_31F4, (u8 *)&lbl_1_data_3204, 
    (u8 *)&lbl_1_data_3214, (u8 *)&lbl_1_data_3224, (u8 *)&lbl_1_data_3234, 
    (u8 *)&lbl_1_data_3244, (u8 *)&lbl_1_data_3254, (u8 *)&lbl_1_data_3264, 
    (u8 *)&lbl_1_data_3274, (u8 *)&lbl_1_data_3284, (u8 *)&lbl_1_data_3294, 
    (u8 *)&lbl_1_data_32A4, (u8 *)&lbl_1_data_32B4, (u8 *)&lbl_1_data_32C4, 
    (u8 *)&lbl_1_data_32D4, (u8 *)&lbl_1_data_32E4, (u8 *)&lbl_1_data_32F4, 
    (u8 *)&lbl_1_data_3304, (u8 *)&lbl_1_data_3314, (u8 *)&lbl_1_data_3324, 
    (u8 *)&lbl_1_data_3334, (u8 *)&lbl_1_data_3344, (u8 *)&lbl_1_data_3354, 
    (u8 *)&lbl_1_data_3364, (u8 *)&lbl_1_data_3374, (u8 *)&lbl_1_data_3384, 
    (u8 *)&lbl_1_data_3394, (u8 *)&lbl_1_data_33A4, (u8 *)&lbl_1_data_33B4, 
    (u8 *)&lbl_1_data_33C4, (u8 *)&lbl_1_data_33D4, (u8 *)&lbl_1_data_33E4, 
    (u8 *)&lbl_1_data_33F4, (u8 *)&lbl_1_data_3404, (u8 *)&lbl_1_data_3414, 
    (u8 *)&lbl_1_data_3424, (u8 *)&lbl_1_data_3434, (u8 *)&lbl_1_data_3444, 
    (u8 *)&lbl_1_data_3454, (u8 *)&lbl_1_data_3464, (u8 *)&lbl_1_data_3474, 
    (u8 *)&lbl_1_data_3484, (u8 *)&lbl_1_data_3494, (u8 *)&lbl_1_data_34A4, 
    (u8 *)&lbl_1_data_34B4, (u8 *)&lbl_1_data_34C4, (u8 *)&lbl_1_data_34D4, 
    (u8 *)&lbl_1_data_34E4, (u8 *)&lbl_1_data_34F4, (u8 *)&lbl_1_data_3504, 
    (u8 *)&lbl_1_data_3514, (u8 *)&lbl_1_data_3524, (u8 *)&lbl_1_data_3534, 
    (u8 *)&lbl_1_data_3544, (u8 *)&lbl_1_data_3554, (u8 *)&lbl_1_data_3564, 
    (u8 *)&lbl_1_data_3574, (u8 *)&lbl_1_data_3584, (u8 *)&lbl_1_data_3594, 
    (u8 *)&lbl_1_data_35A4, (u8 *)&lbl_1_data_35B4, (u8 *)&lbl_1_data_35C4, 
    (u8 *)&lbl_1_data_35D4, (u8 *)&lbl_1_data_35E4, (u8 *)&lbl_1_data_35F4, 
    (u8 *)&lbl_1_data_3604, (u8 *)&lbl_1_data_3614, (u8 *)&lbl_1_data_3624, 
    (u8 *)&lbl_1_data_3634, (u8 *)&lbl_1_data_3644, (u8 *)&lbl_1_data_3654, 
    (u8 *)&lbl_1_data_3664, (u8 *)&lbl_1_data_3674, (u8 *)&lbl_1_data_3684, 
    (u8 *)&lbl_1_data_3694, (u8 *)&lbl_1_data_36A4, (u8 *)&lbl_1_data_36B4, 
    (u8 *)&lbl_1_data_36C4, (u8 *)&lbl_1_data_36D4, (u8 *)&lbl_1_data_36E4, 
    (u8 *)&lbl_1_data_36F4, (u8 *)&lbl_1_data_3704, (u8 *)&lbl_1_data_3714, 
    (u8 *)&lbl_1_data_3724, (u8 *)&lbl_1_data_3734, (u8 *)&lbl_1_data_3744, 
    (u8 *)&lbl_1_data_3754, (u8 *)&lbl_1_data_3764, (u8 *)&lbl_1_data_3774, 
    (u8 *)&lbl_1_data_3784, (u8 *)&lbl_1_data_3794, (u8 *)&lbl_1_data_37A4, 
    (u8 *)&lbl_1_data_37B4, (u8 *)&lbl_1_data_37C4, (u8 *)&lbl_1_data_37D4, 
    (u8 *)&lbl_1_data_37E4, (u8 *)&lbl_1_data_37F4, (u8 *)&lbl_1_data_3804, 
    (u8 *)&lbl_1_data_3814, (u8 *)&lbl_1_data_3824, (u8 *)&lbl_1_data_3834, 
    (u8 *)&lbl_1_data_3844, (u8 *)&lbl_1_data_3854, (u8 *)&lbl_1_data_3864, 
    (u8 *)&lbl_1_data_3874, (u8 *)&lbl_1_data_3884, (u8 *)&lbl_1_data_3894, 
    (u8 *)&lbl_1_data_38A4, (u8 *)&lbl_1_data_38B4, (u8 *)&lbl_1_data_38C4, 
    (u8 *)&lbl_1_data_38D4, (u8 *)&lbl_1_data_38E4, (u8 *)&lbl_1_data_38F4, 
    (u8 *)&lbl_1_data_3904, (u8 *)&lbl_1_data_3914, (u8 *)&lbl_1_data_3924, 
    (u8 *)&lbl_1_data_3934, (u8 *)&lbl_1_data_3944, (u8 *)&lbl_1_data_3954, 
    (u8 *)&lbl_1_data_3964, (u8 *)&lbl_1_data_3974, (u8 *)&lbl_1_data_3984, 
    (u8 *)&lbl_1_data_3994, (u8 *)&lbl_1_data_39A4, (u8 *)&lbl_1_data_39B4, 
    (u8 *)&lbl_1_data_39C4, (u8 *)&lbl_1_data_39D4, (u8 *)&lbl_1_data_39E4, 
    (u8 *)&lbl_1_data_39F4, (u8 *)&lbl_1_data_3A04, (u8 *)&lbl_1_data_3A14, 
    (u8 *)&lbl_1_data_3A24, (u8 *)&lbl_1_data_3A34, (u8 *)&lbl_1_data_3A44, 
    (u8 *)&lbl_1_data_3A54, (u8 *)&lbl_1_data_3A64, (u8 *)&lbl_1_data_3A74, 
    (u8 *)&lbl_1_data_3A84, (u8 *)&lbl_1_data_3A94, (u8 *)&lbl_1_data_3AA4, 
    (u8 *)&lbl_1_data_3AB4, (u8 *)&lbl_1_data_3AC4, (u8 *)&lbl_1_data_3AD4, 
    (u8 *)&lbl_1_data_3AE4, (u8 *)&lbl_1_data_3AF4, (u8 *)&lbl_1_data_3B04, 
    (u8 *)&lbl_1_data_3B14, (u8 *)&lbl_1_data_3B24, (u8 *)&lbl_1_data_3B38, 
    (u8 *)&lbl_1_data_3B58, (u8 *)&lbl_1_data_3B68, (u8 *)&lbl_1_data_3B7C, 
    (u8 *)&lbl_1_data_3B9C, (u8 *)&lbl_1_data_3BAC, (u8 *)&lbl_1_data_3BBC, 
    (u8 *)&lbl_1_data_3BD0, (u8 *)&lbl_1_data_3BF0, (u8 *)&lbl_1_data_3C00, 
    (u8 *)&lbl_1_data_3C10, (u8 *)&lbl_1_data_3C20, (u8 *)&lbl_1_data_3C30, 
    (u8 *)&lbl_1_data_3B48, (u8 *)&lbl_1_data_3B8C, (u8 *)&lbl_1_data_3BE0, 
    (u8 *)&lbl_1_data_3C40, (u8 *)&lbl_1_data_3C50, (u8 *)&lbl_1_data_3C64, 
    (u8 *)&lbl_1_data_3C84, (u8 *)&lbl_1_data_3C94, (u8 *)&lbl_1_data_3CA8, 
    (u8 *)&lbl_1_data_3CC8, (u8 *)&lbl_1_data_3CD8, (u8 *)&lbl_1_data_3CE8, 
    (u8 *)&lbl_1_data_3CF8, (u8 *)&lbl_1_data_3D08, (u8 *)&lbl_1_data_3D1C, 
    (u8 *)&lbl_1_data_3D3C, (u8 *)&lbl_1_data_3D4C, (u8 *)&lbl_1_data_3C74, 
    (u8 *)&lbl_1_data_3CB8, (u8 *)&lbl_1_data_3D2C, (u8 *)&lbl_1_data_3D5C, 
    (u8 *)&lbl_1_data_3D6C, (u8 *)&lbl_1_data_3D7C, (u8 *)&lbl_1_data_3D90, 
    (u8 *)&lbl_1_data_3DB0, (u8 *)&lbl_1_data_3DC0, (u8 *)&lbl_1_data_3DD0, 
    (u8 *)&lbl_1_data_3DE0, (u8 *)&lbl_1_data_3DF0, (u8 *)&lbl_1_data_3E00, 
    (u8 *)&lbl_1_data_3E10, (u8 *)&lbl_1_data_3E20, (u8 *)&lbl_1_data_3E30, 
    (u8 *)&lbl_1_data_3DA0, (u8 *)&lbl_1_data_3E40, (u8 *)&lbl_1_data_3E50, 
    (u8 *)&lbl_1_data_3E60, (u8 *)&lbl_1_data_3E74, (u8 *)&lbl_1_data_3E94, 
    (u8 *)&lbl_1_data_3EA4, (u8 *)&lbl_1_data_3EB8, (u8 *)&lbl_1_data_3ED8, 
    (u8 *)&lbl_1_data_3EE8, (u8 *)&lbl_1_data_3EF8, (u8 *)&lbl_1_data_3F08, 
    (u8 *)&lbl_1_data_3F18, (u8 *)&lbl_1_data_3F28, (u8 *)&lbl_1_data_3F38, 
    (u8 *)&lbl_1_data_3E84, (u8 *)&lbl_1_data_3EC8, (u8 *)&lbl_1_data_3F48, 
    (u8 *)&lbl_1_data_3F58, (u8 *)&lbl_1_data_3F68, (u8 *)&lbl_1_data_3F78, 
    (u8 *)&lbl_1_data_3F8C, (u8 *)&lbl_1_data_3FAC, (u8 *)&lbl_1_data_3FBC, 
    (u8 *)&lbl_1_data_3FCC, (u8 *)&lbl_1_data_3FE0, (u8 *)&lbl_1_data_4000, 
    (u8 *)&lbl_1_data_4010, (u8 *)&lbl_1_data_4020, (u8 *)&lbl_1_data_4030, 
    (u8 *)&lbl_1_data_3F9C, (u8 *)&lbl_1_data_3FF0, (u8 *)&lbl_1_data_4040, 
    (u8 *)&lbl_1_data_4050, (u8 *)&lbl_1_data_4060, (u8 *)&lbl_1_data_4074, 
    (u8 *)&lbl_1_data_4094, (u8 *)&lbl_1_data_40A4, (u8 *)&lbl_1_data_40B4, 
    (u8 *)&lbl_1_data_40C4, (u8 *)&lbl_1_data_40D4, (u8 *)&lbl_1_data_40E4, 
    (u8 *)&lbl_1_data_40F4, (u8 *)&lbl_1_data_4104, (u8 *)&lbl_1_data_4114, 
    (u8 *)&lbl_1_data_4084, (u8 *)&lbl_1_data_4124,
};

u32 gap_04_000045BC_data[1] = {
    0x00000000,
};


void fn_1_B828(INPUTMAIN *main, s32 padNo, PADOWNER *playerConf)
{
    memset(main, 0, sizeof(INPUTMAIN));
    main->playerConf = playerConf;
    main->padId = GwPlayerConf[padNo].padNo;
    fn_1_BAC4(&main->work, main);
}

void fn_1_B898(INPUTMAIN *main)
{
    fn_1_BC54(&main->work);
    memset(main, 0, sizeof(INPUTMAIN));
}

void fn_1_B8D8(INPUTMAIN *main)
{
    register s32 absStkX;
    register s32 stkXv;
    register s32 absStkY;
    register s32 stkYv;

    if (main->work.type != 0) {
        fn_1_BC84(&main->work);
        return;
    }

    main->btn = HuPadBtn[main->padId];
    main->btnDown = HuPadBtnDown[main->padId];
    main->btnRep = HuPadBtnRep[main->padId];

    main->stkX = HuPadStkX[main->padId];
    if (main->stkX >= 0) {
        absStkX = main->stkX;
    } else {
        absStkX = -main->stkX;
    }
    if (absStkX < 10) {
        stkXv = 0;
    } else {
        stkXv = main->stkX;
    }
    main->stkX = stkXv;

    main->stkY = HuPadStkY[main->padId];
    if (main->stkY >= 0) {
        absStkY = main->stkY;
    } else {
        absStkY = -main->stkY;
    }
    if (absStkY < 10) {
        stkYv = 0;
    } else {
        stkYv = main->stkY;
    }
    main->stkY = stkYv;

    main->subStkX = HuPadSubStkX[main->padId];
    main->subStkY = HuPadSubStkY[main->padId];
    main->trigL = HuPadTrigL[main->padId];
    main->trigR = HuPadTrigR[main->padId];
    main->dStk = HuPadDStk[main->padId];
    main->dStkRep = HuPadDStkRep[main->padId];
}

void fn_1_BAC4(INPUTWORK *work, INPUTMAIN *main)
{
    register s32 i;
    register s32 playerNo;
    register s32 v1;
    register s32 v2;
    register s32 v3;
    register void **t1;
    register void **t2;
    register void **t3;

    playerNo = main->playerConf->unk0->playerNo;

    memset(work, 0, 0x24);
    work->main = main;

    work->type = GwPlayerConf[playerNo].type;

    if (GwPlayerConf[playerNo].dif == 3) {
        v1 = 0;
    } else {
        if (GwPlayerConf[playerNo].dif == 2) {
            v2 = 1;
        } else {
            if (GwPlayerConf[playerNo].dif == 1) {
                v3 = 2;
            } else {
                v3 = 3;
            }
            v2 = v3;
        }
        v1 = v2;
    }
    work->dif = v1;

    if (work->dif == 0) {
        t1 = lbl_1_data_2E14;
    } else {
        if (work->dif == 1) {
            t2 = lbl_1_data_2E38;
        } else {
            if (work->dif == 2) {
                t3 = lbl_1_data_2E74;
            } else {
                t3 = lbl_1_data_2EB0;
            }
            t2 = t3;
        }
        t1 = t2;
    }
    work->dataTable = t1;

    for (i = 0; lbl_1_data_4134[i] != NULL; i++) {
        *(u16 *)(lbl_1_data_4134[i] + 0xA) = 0;
    }
}
