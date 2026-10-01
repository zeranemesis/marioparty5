#ifndef DATANUM_M513_H
#define DATANUM_M513_H

#include "datadir_enum.h"

enum {
    M513_HSF_stage = DATANUM(DATA_m513, 0),
    M513_HSF_mame = DATANUM(DATA_m513, 1),
    M513_HSF_mame_small = DATANUM(DATA_m513, 2),
    M513_ANM_camera = DATANUM(DATA_m513, 3),
    M513_ANM_ind_a = DATANUM(DATA_m513, 0x15),
    M513_ANM_ind_b = DATANUM(DATA_m513, 0x16),
    M513_ANM_ind_c = DATANUM(DATA_m513, 0x17),
    M513_ANM_ind_d = DATANUM(DATA_m513, 0x18),
    M513_ANM_ind_num = DATANUM(DATA_m513, 0x19),
};

#endif