//----------------------------------------------------------------------------//
#include "dlmsListObj.h"
#include "globals.h"
#include "lcd.h"
#include "log.h"
#include <string.h>

extern bool dlmsClockAkros; // AKROS: date/time from clock 0.0.1.0.0.255
                            // (thread_dataRequest.c)
//----------------------------------------------------------------------------//
const list_dlms_OBIS_codes_t list_dlms_OBIS_codes[DLMS_MAX_OBJ] = {
    // ˜˜˜˜˜˜ ˜ OBIS-˜˜˜˜˜˜, ˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜˜, ˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜
    //				Name
    // OBIS-Code			Accurecy	fKilo	˜˜˜˜˜˜
    /*	DLMS_ENERGY_I_IV	*/ 1,
    0,
    1,
    8,
    0,
    255,
    ACC_2,
    true,
    1, // Energy KWh - I+IV
    /*	DLMS_ENERGY_II_III	*/ 1,
    0,
    2,
    8,
    0,
    255,
    ACC_2,
    true,
    1, // Energy KWh  - II+III
    /*	DLMS_ENERGY_I_II	*/ 1,
    0,
    3,
    8,
    0,
    255,
    ACC_2,
    true,
    1, // Energy KVArh - I+II
    /*	DLMS_ENERGY_III_IV	*/ 1,
    0,
    4,
    8,
    0,
    255,
    ACC_2,
    true,
    1, // Energy KVArh - III+IV

    /*	DLMS_ACTIVE_ENERGY_T1	*/ 1,
    0,
    1,
    8,
    1,
    255,
    ACC_2,
    true,
    1, // 1-tariff of Active Energy (KWh)
    /*	DLMS_ACTIVE_ENERGY_T2	*/ 1,
    0,
    1,
    8,
    2,
    255,
    ACC_2,
    true,
    1, // 2-tariff of Active Energy (KWh)
    /*	DLMS_ACTIVE_ENERGY_T3	*/ 1,
    0,
    1,
    8,
    3,
    255,
    ACC_2,
    true,
    1, // 3-tariff of Active Energy (KWh)
    /*	DLMS_ACTIVE_ENERGY_T4	*/ 1,
    0,
    1,
    8,
    4,
    255,
    ACC_2,
    true,
    1, // 4-tariff of Active Energy (KWh)
    /*	DLMS_ACTIVE_ENERGY_T5	*/ 1,
    0,
    1,
    8,
    5,
    255,
    ACC_2,
    true,
    1, // 5-tariff of Active Energy (KWh)
    /*	DLMS_ACTIVE_ENERGY_T6	*/ 1,
    0,
    1,
    8,
    6,
    255,
    ACC_2,
    true,
    1, // 6-tariff of Active Energy (KWh)
    /*	DLMS_ACTIVE_ENERGY_T7	*/ 1,
    0,
    1,
    8,
    7,
    255,
    ACC_2,
    true,
    1, // 7-tariff of Active Energy (KWh)
    /*	DLMS_ACTIVE_ENERGY_T8	*/ 1,
    0,
    1,
    8,
    8,
    255,
    ACC_2,
    true,
    1, // 8-tariff of Active Energy (KWh)

    /*	DLMS_ENERGY_II_III_T1	*/ 1,
    0,
    2,
    8,
    1,
    255,
    ACC_2,
    true,
    1, // ˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ 1 ˜˜˜˜˜ KWh  - II+III
    /*	DLMS_ENERGY_II_III_T2	*/ 1,
    0,
    2,
    8,
    2,
    255,
    ACC_2,
    true,
    1, // ˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ 2 ˜˜˜˜˜ KWh  - II+III
    /*	DLMS_ENERGY_II_III_T3	*/ 1,
    0,
    2,
    8,
    3,
    255,
    ACC_2,
    true,
    1, // ˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ 3 ˜˜˜˜˜ KWh  - II+III
    /*	DLMS_ENERGY_II_III_T4	*/ 1,
    0,
    2,
    8,
    4,
    255,
    ACC_2,
    true,
    1, // ˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ 4 ˜˜˜˜˜ KWh  - II+III
    /*	DLMS_ENERGY_II_III_T5	*/ 1,
    0,
    2,
    8,
    5,
    255,
    ACC_2,
    true,
    1, // ˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ 5 ˜˜˜˜˜ KWh  - II+III
    /*	DLMS_ENERGY_II_III_T6	*/ 1,
    0,
    2,
    8,
    6,
    255,
    ACC_2,
    true,
    1, // ˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ 6 ˜˜˜˜˜ KWh  - II+III
    /*	DLMS_ENERGY_II_III_T7	*/ 1,
    0,
    2,
    8,
    7,
    255,
    ACC_2,
    true,
    1, // ˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ 7 ˜˜˜˜˜ KWh  - II+III
    /*	DLMS_ENERGY_II_III_T8	*/ 1,
    0,
    2,
    8,
    8,
    255,
    ACC_2,
    true,
    1, // ˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ 8 ˜˜˜˜˜ KWh  - II+III

    /*	DLMS_ENERGY_I_II_T1	*/ 1,
    0,
    3,
    8,
    1,
    255,
    ACC_2,
    true,
    1, // ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜ 1 ˜˜˜˜˜ KVArh - I+II
    /*	DLMS_ENERGY_I_II_T2	*/ 1,
    0,
    3,
    8,
    2,
    255,
    ACC_2,
    true,
    1, // ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜ 2 ˜˜˜˜˜ KVArh - I+II
    /*	DLMS_ENERGY_I_II_T3	*/ 1,
    0,
    3,
    8,
    3,
    255,
    ACC_2,
    true,
    1, // ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜ 3 ˜˜˜˜˜ KVArh - I+II
    /*	DLMS_ENERGY_I_II_T4	*/ 1,
    0,
    3,
    8,
    4,
    255,
    ACC_2,
    true,
    1, // ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜ 4 ˜˜˜˜˜ KVArh - I+II
    /*	DLMS_ENERGY_I_II_T5	*/ 1,
    0,
    3,
    8,
    5,
    255,
    ACC_2,
    true,
    1, // ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜ 5 ˜˜˜˜˜ KVArh - I+II
    /*	DLMS_ENERGY_I_II_T6	*/ 1,
    0,
    3,
    8,
    6,
    255,
    ACC_2,
    true,
    1, // ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜ 6 ˜˜˜˜˜ KVArh - I+II
    /*	DLMS_ENERGY_I_II_T7	*/ 1,
    0,
    3,
    8,
    7,
    255,
    ACC_2,
    true,
    1, // ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜ 7 ˜˜˜˜˜ KVArh - I+II
    /*	DLMS_ENERGY_I_II_T8	*/ 1,
    0,
    3,
    8,
    8,
    255,
    ACC_2,
    true,
    1, // ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜ 8 ˜˜˜˜˜ KVArh - I+II

    /*	DLMS_ENERGY_III_IV_T1	*/ 1,
    0,
    4,
    8,
    1,
    255,
    ACC_2,
    true,
    1, // ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ 1 ˜˜˜˜˜ KVArh - III+IV
    /*	DLMS_ENERGY_III_IV_T2	*/ 1,
    0,
    4,
    8,
    2,
    255,
    ACC_2,
    true,
    1, // ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ 2 ˜˜˜˜˜ KVArh - III+IV
    /*	DLMS_ENERGY_III_IV_T3	*/ 1,
    0,
    4,
    8,
    3,
    255,
    ACC_2,
    true,
    1, // ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ 3 ˜˜˜˜˜ KVArh - III+IV
    /*	DLMS_ENERGY_III_IV_T4	*/ 1,
    0,
    4,
    8,
    4,
    255,
    ACC_2,
    true,
    1, // ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ 4 ˜˜˜˜˜ KVArh - III+IV
    /*	DLMS_ENERGY_III_IV_T5	*/ 1,
    0,
    4,
    8,
    5,
    255,
    ACC_2,
    true,
    1, // ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ 5 ˜˜˜˜˜ KVArh - III+IV
    /*	DLMS_ENERGY_III_IV_T6	*/ 1,
    0,
    4,
    8,
    6,
    255,
    ACC_2,
    true,
    1, // ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ 6 ˜˜˜˜˜ KVArh - III+IV
    /*	DLMS_ENERGY_III_IV_T7	*/ 1,
    0,
    4,
    8,
    7,
    255,
    ACC_2,
    true,
    1, // ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ 7 ˜˜˜˜˜ KVArh - III+IV
    /*	DLMS_ENERGY_III_IV_T8	*/ 1,
    0,
    4,
    8,
    8,
    255,
    ACC_2,
    true,
    1, // ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ 8 ˜˜˜˜˜ KVArh - III+IV

    /*	DLMS_TIME	*/ 0,
    0,
    0,
    9,
    1,
    255,
    ACC_2,
    false,
    1, // Time
    /*	DLMS_DATE	*/ 0,
    0,
    0,
    9,
    2,
    255,
    ACC_2,
    false,
    1, // Date

    /*	DLMS_ACTIVE_POWER_PLUS	*/ 1,
    0,
    1,
    7,
    0,
    255,
    ACC_4,
    true,
    2,
    /*	DLMS_L1_ACTIVE_POWER_PLUS	*/ 1,
    0,
    21,
    7,
    0,
    255,
    ACC_4,
    true,
    2,
    /*	DLMS_L2_ACTIVE_POWER_PLUS	*/ 1,
    0,
    41,
    7,
    0,
    255,
    ACC_4,
    true,
    2,
    /*	DLMS_L3_ACTIVE_POWER_PLUS	*/ 1,
    0,
    61,
    7,
    0,
    255,
    ACC_4,
    true,
    2,
    /*	DLMS_REACTIVE_POWER_PLUS	*/ 1,
    0,
    3,
    7,
    0,
    255,
    ACC_4,
    true,
    2,
    /*	DLMS_L1_REACTIVE_POWER_PLUS	*/ 1,
    0,
    23,
    7,
    0,
    255,
    ACC_4,
    true,
    2,
    /*	DLMS_L2_REACTIVE_POWER_PLUS	*/ 1,
    0,
    43,
    7,
    0,
    255,
    ACC_4,
    true,
    2,
    /*	DLMS_L3_REACTIVE_POWER_PLUS	*/ 1,
    0,
    63,
    7,
    0,
    255,
    ACC_4,
    true,
    2,
    /*	DLMS_APPARENT_POWER_PLUS	*/ 1,
    0,
    9,
    7,
    0,
    255,
    ACC_4,
    true,
    2,
    /*	DLMS_L1_APPARENT_POWER_PLUS	*/ 1,
    0,
    29,
    7,
    0,
    255,
    ACC_4,
    true,
    2,
    /*	DLMS_L2_APPARENT_POWER_PLUS	*/ 1,
    0,
    49,
    7,
    0,
    255,
    ACC_4,
    true,
    2,
    /*	DLMS_L3_APPARENT_POWER_PLUS	*/ 1,
    0,
    69,
    7,
    0,
    255,
    ACC_4,
    true,
    2,

    /*	DLMS_VOLTAGE	*/ 1,
    0,
    12,
    7,
    0,
    255,
    ACC_2,
    false,
    2,
    /*	DLMS_L1_VOLTAGE	*/ 1,
    0,
    32,
    7,
    0,
    255,
    ACC_2,
    false,
    2,
    /*	DLMS_L2_VOLTAGE	*/ 1,
    0,
    52,
    7,
    0,
    255,
    ACC_2,
    false,
    2,
    /*	DLMS_L3_VOLTAGE	*/ 1,
    0,
    72,
    7,
    0,
    255,
    ACC_2,
    false,
    2,
    /*	DLMS_L1_L2_VOLTAGE_OLD */ 1,
    0,
    12,
    7,
    1,
    255,
    ACC_2,
    false,
    2, // ˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜ L1-L2
    /*	DLMS_L2_L3_VOLTAGE_OLD */ 1,
    0,
    12,
    7,
    2,
    255,
    ACC_2,
    false,
    2, // ˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜ L2-L3
    /*	DLMS_L1_L3_VOLTAGE_OLD */ 1,
    0,
    12,
    7,
    3,
    255,
    ACC_2,
    false,
    2, // ˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜ L1-L3
    /*	DLMS_L1_L2_VOLTAGE */ 1,
    0,
    124,
    7,
    0,
    255,
    ACC_2,
    false,
    2, // ˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜ L1-L2
    /*	DLMS_L2_L3_VOLTAGE */ 1,
    0,
    125,
    7,
    0,
    255,
    ACC_2,
    false,
    2, // ˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜ L2-L3
    /*	DLMS_L1_L3_VOLTAGE */ 1,
    0,
    126,
    7,
    0,
    255,
    ACC_2,
    false,
    2, // ˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜ L1-L3
    /*	DLMS_CURRENT	*/ 1,
    0,
    11,
    7,
    0,
    255,
    ACC_3,
    false,
    2,
    /*	DLMS_L1_CURRENT	*/ 1,
    0,
    31,
    7,
    0,
    255,
    ACC_3,
    false,
    2,
    /*	DLMS_L2_CURRENT	*/ 1,
    0,
    51,
    7,
    0,
    255,
    ACC_3,
    false,
    2,
    /*	DLMS_L3_CURRENT	*/ 1,
    0,
    71,
    7,
    0,
    255,
    ACC_3,
    false,
    2,
    /*	DLMS_NEUTRAL_CURRENT	*/ 1,
    0,
    91,
    7,
    0,
    255,
    ACC_3,
    false,
    2,
    /*	DLMS_SUPPLY_FREQUENCY	*/ 1,
    0,
    14,
    7,
    0,
    255,
    ACC_3,
    false,
    2,
    /*	DLMS_NOMINAL_VOLTAGE	*/ 1,
    0,
    0,
    6,
    0,
    255,
    ACC_2,
    false,
    2, // Nominal voltage
    /*	DLMS_NOMINAL_CURRENT	*/ 1,
    0,
    0,
    6,
    1,
    255,
    ACC_3,
    false,
    2, // Nominal current
    /*	DLMS_NOMINAL_FREQUENCY	*/ 1,
    0,
    0,
    6,
    2,
    255,
    ACC_3,
    false,
    2, // Nominal frequency (AKROS)
    /*	DLMS_L1_COS					*/ 1,
    0,
    33,
    7,
    0,
    255,
    ACC_3,
    false,
    2, // Cos(˜˜˜˜ A)
    /*	DLMS_L2_COS					*/ 1,
    0,
    53,
    7,
    0,
    255,
    ACC_3,
    false,
    2, // Cos(˜˜˜˜ B)
    /*	DLMS_L3_COS					*/ 1,
    0,
    73,
    7,
    0,
    255,
    ACC_3,
    false,
    2, // Cos(˜˜˜˜ C)
    /*	DLMS_COS					*/ 1,
    0,
    13,
    7,
    0,
    255,
    ACC_3,
    false,
    2, // Cos(˜˜˜˜˜)

    /*	DLMS_RDCH_ENERGY_I_IV_OLD	*/ 1,
    0,
    1,
    17,
    0,
    255,
    ACC_2,
    true,
    3, // Energy KWh - I+IV
    /*	DLMS_RDCH_ENERGY_II_III_OLD	*/ 1,
    0,
    2,
    17,
    0,
    255,
    ACC_2,
    true,
    3, // Energy KWh  - II+III
    /*	DLMS_RDCH_ENERGY_I_II_OLD	*/ 1,
    0,
    3,
    17,
    0,
    255,
    ACC_2,
    true,
    3, // Energy KVArh - I+II
    /*	DLMS_RDCH_ENERGY_III_IV_OLD	*/ 1,
    0,
    4,
    17,
    0,
    255,
    ACC_2,
    true,
    3, // Energy KVArh - III+IV
    /*	DLMS_RDCH_ENERGY_I_IV	*/ 1,
    0,
    1,
    8,
    0,
    101,
    ACC_2,
    true,
    3, // Energy KWh - I+IV
    /*	DLMS_RDCH_ENERGY_II_III	*/ 1,
    0,
    2,
    8,
    0,
    101,
    ACC_2,
    true,
    3, // Energy KWh  - II+III
    /*	DLMS_RDCH_ENERGY_I_II	*/ 1,
    0,
    3,
    8,
    0,
    101,
    ACC_2,
    true,
    3, // Energy KVArh - I+II
    /*	DLMS_RDCH_ENERGY_III_IV	*/ 1,
    0,
    4,
    8,
    0,
    101,
    ACC_2,
    true,
    3, // Energy KVArh - III+IV

    /*	DLMS_RDCH_ACTIVE_ENERGY_T1_OLD	*/ 1,
    0,
    1,
    17,
    1,
    255,
    ACC_2,
    true,
    3, // 1-tariff of Active Energy (KWh)
    /*	DLMS_RDCH_ACTIVE_ENERGY_T2_OLD	*/ 1,
    0,
    1,
    17,
    2,
    255,
    ACC_2,
    true,
    3, // 2-tariff of Active Energy (KWh)
    /*	DLMS_RDCH_ACTIVE_ENERGY_T3_OLD	*/ 1,
    0,
    1,
    17,
    3,
    255,
    ACC_2,
    true,
    3, // 3-tariff of Active Energy (KWh)
    /*	DLMS_RDCH_ACTIVE_ENERGY_T4_OLD	*/ 1,
    0,
    1,
    17,
    4,
    255,
    ACC_2,
    true,
    3, // 4-tariff of Active Energy (KWh)
    /*	DLMS_RDCH_ACTIVE_ENERGY_T5_OLD	*/ 1,
    0,
    1,
    17,
    5,
    255,
    ACC_2,
    true,
    3, // 5-tariff of Active Energy (KWh)
    /*	DLMS_RDCH_ACTIVE_ENERGY_T6_OLD	*/ 1,
    0,
    1,
    17,
    6,
    255,
    ACC_2,
    true,
    3, // 6-tariff of Active Energy (KWh)
    /*	DLMS_RDCH_ACTIVE_ENERGY_T7_OLD	*/ 1,
    0,
    1,
    17,
    7,
    255,
    ACC_2,
    true,
    3, // 7-tariff of Active Energy (KWh)
    /*	DLMS_RDCH_ACTIVE_ENERGY_T8_OLD	*/ 1,
    0,
    1,
    17,
    8,
    255,
    ACC_2,
    true,
    3, // 8-tariff of Active Energy (KWh)
    /*	DLMS_RDCH_ACTIVE_ENERGY_T1	*/ 1,
    0,
    1,
    8,
    1,
    101,
    ACC_2,
    true,
    3, // 1-tariff of Active Energy (KWh)
    /*	DLMS_RDCH_ACTIVE_ENERGY_T2	*/ 1,
    0,
    1,
    8,
    2,
    101,
    ACC_2,
    true,
    3, // 2-tariff of Active Energy (KWh)
    /*	DLMS_RDCH_ACTIVE_ENERGY_T3	*/ 1,
    0,
    1,
    8,
    3,
    101,
    ACC_2,
    true,
    3, // 3-tariff of Active Energy (KWh)
    /*	DLMS_RDCH_ACTIVE_ENERGY_T4	*/ 1,
    0,
    1,
    8,
    4,
    101,
    ACC_2,
    true,
    3, // 4-tariff of Active Energy (KWh)
    /*	DLMS_RDCH_ACTIVE_ENERGY_T5	*/ 1,
    0,
    1,
    8,
    5,
    101,
    ACC_2,
    true,
    3, // 5-tariff of Active Energy (KWh)
    /*	DLMS_RDCH_ACTIVE_ENERGY_T6	*/ 1,
    0,
    1,
    8,
    6,
    101,
    ACC_2,
    true,
    3, // 6-tariff of Active Energy (KWh)
    /*	DLMS_RDCH_ACTIVE_ENERGY_T7	*/ 1,
    0,
    1,
    8,
    7,
    101,
    ACC_2,
    true,
    3, // 7-tariff of Active Energy (KWh)
    /*	DLMS_RDCH_ACTIVE_ENERGY_T8	*/ 1,
    0,
    1,
    8,
    8,
    101,
    ACC_2,
    true,
    3, // 8-tariff of Active Energy (KWh)

    /*	DLMS_RDCH_ENERGY_II_III_T1_OLD	*/ 1,
    0,
    2,
    17,
    1,
    255,
    ACC_2,
    true,
    3, // 1-tariff of Energy KWh  - II+III rdch
    /*	DLMS_RDCH_ENERGY_II_III_T2_OLD	*/ 1,
    0,
    2,
    17,
    2,
    255,
    ACC_2,
    true,
    3, // 2-tariff of Energy KWh  - II+III rdch
    /*	DLMS_RDCH_ENERGY_II_III_T3_OLD	*/ 1,
    0,
    2,
    17,
    3,
    255,
    ACC_2,
    true,
    3, // 3-tariff of Energy KWh  - II+III rdch
    /*	DLMS_RDCH_ENERGY_II_III_T4_OLD	*/ 1,
    0,
    2,
    17,
    4,
    255,
    ACC_2,
    true,
    3, // 4-tariff of Energy KWh  - II+III rdch
    /*	DLMS_RDCH_ENERGY_II_III_T5_OLD	*/ 1,
    0,
    2,
    17,
    5,
    255,
    ACC_2,
    true,
    3, // 5-tariff of Energy KWh  - II+III rdch
    /*	DLMS_RDCH_ENERGY_II_III_T6_OLD	*/ 1,
    0,
    2,
    17,
    6,
    255,
    ACC_2,
    true,
    3, // 6-tariff of Energy KWh  - II+III rdch
    /*	DLMS_RDCH_ENERGY_II_III_T7_OLD	*/ 1,
    0,
    2,
    17,
    7,
    255,
    ACC_2,
    true,
    3, // 7-tariff of Energy KWh  - II+III rdch
    /*	DLMS_RDCH_ENERGY_II_III_T8_OLD	*/ 1,
    0,
    2,
    17,
    8,
    255,
    ACC_2,
    true,
    3, // 8-tariff of Energy KWh  - II+III rdch
    /*	DLMS_RDCH_ENERGY_II_III_T1	*/ 1,
    0,
    2,
    8,
    1,
    101,
    ACC_2,
    true,
    3, // 1-tariff of Energy KWh  - II+III rdch
    /*	DLMS_RDCH_ENERGY_II_III_T2	*/ 1,
    0,
    2,
    8,
    2,
    101,
    ACC_2,
    true,
    3, // 2-tariff of Energy KWh  - II+III rdch
    /*	DLMS_RDCH_ENERGY_II_III_T3	*/ 1,
    0,
    2,
    8,
    3,
    101,
    ACC_2,
    true,
    3, // 3-tariff of Energy KWh  - II+III rdch
    /*	DLMS_RDCH_ENERGY_II_III_T4	*/ 1,
    0,
    2,
    8,
    4,
    101,
    ACC_2,
    true,
    3, // 4-tariff of Energy KWh  - II+III rdch
    /*	DLMS_RDCH_ENERGY_II_III_T5	*/ 1,
    0,
    2,
    8,
    5,
    101,
    ACC_2,
    true,
    3, // 5-tariff of Energy KWh  - II+III rdch
    /*	DLMS_RDCH_ENERGY_II_III_T6	*/ 1,
    0,
    2,
    8,
    6,
    101,
    ACC_2,
    true,
    3, // 6-tariff of Energy KWh  - II+III rdch
    /*	DLMS_RDCH_ENERGY_II_III_T7	*/ 1,
    0,
    2,
    8,
    7,
    101,
    ACC_2,
    true,
    3, // 7-tariff of Energy KWh  - II+III rdch
    /*	DLMS_RDCH_ENERGY_II_III_T8	*/ 1,
    0,
    2,
    8,
    8,
    101,
    ACC_2,
    true,
    3, // 8-tariff of Energy KWh  - II+III rdch

    /*	DLMS_RDCH_ENERGY_I_II_T1_OLD	*/ 1,
    0,
    3,
    17,
    1,
    255,
    ACC_2,
    true,
    3, // 1-tariff of Energy KVArh - I+II rdch
    /*	DLMS_RDCH_ENERGY_I_II_T2_OLD	*/ 1,
    0,
    3,
    17,
    2,
    255,
    ACC_2,
    true,
    3, // 2-tariff of Energy KVArh - I+II rdch
    /*	DLMS_RDCH_ENERGY_I_II_T3_OLD	*/ 1,
    0,
    3,
    17,
    3,
    255,
    ACC_2,
    true,
    3, // 3-tariff of Energy KVArh - I+II rdch
    /*	DLMS_RDCH_ENERGY_I_II_T4_OLD	*/ 1,
    0,
    3,
    17,
    4,
    255,
    ACC_2,
    true,
    3, // 4-tariff of Energy KVArh - I+II rdch
    /*	DLMS_RDCH_ENERGY_I_II_T5_OLD	*/ 1,
    0,
    3,
    17,
    5,
    255,
    ACC_2,
    true,
    3, // 5-tariff of Energy KVArh - I+II rdch
    /*	DLMS_RDCH_ENERGY_I_II_T6_OLD	*/ 1,
    0,
    3,
    17,
    6,
    255,
    ACC_2,
    true,
    3, // 6-tariff of Energy KVArh - I+II rdch
    /*	DLMS_RDCH_ENERGY_I_II_T7_OLD	*/ 1,
    0,
    3,
    17,
    7,
    255,
    ACC_2,
    true,
    3, // 7-tariff of Energy KVArh - I+II rdch
    /*	DLMS_RDCH_ENERGY_I_II_T8_OLD	*/ 1,
    0,
    3,
    17,
    8,
    255,
    ACC_2,
    true,
    3, // 8-tariff of Energy KVArh - I+II rdch
    /*	DLMS_RDCH_ENERGY_I_II_T1	*/ 1,
    0,
    3,
    8,
    1,
    101,
    ACC_2,
    true,
    3, // 1-tariff of Energy KVArh - I+II rdch
    /*	DLMS_RDCH_ENERGY_I_II_T2	*/ 1,
    0,
    3,
    8,
    2,
    101,
    ACC_2,
    true,
    3, // 2-tariff of Energy KVArh - I+II rdch
    /*	DLMS_RDCH_ENERGY_I_II_T3	*/ 1,
    0,
    3,
    8,
    3,
    101,
    ACC_2,
    true,
    3, // 3-tariff of Energy KVArh - I+II rdch
    /*	DLMS_RDCH_ENERGY_I_II_T4	*/ 1,
    0,
    3,
    8,
    4,
    101,
    ACC_2,
    true,
    3, // 4-tariff of Energy KVArh - I+II rdch
    /*	DLMS_RDCH_ENERGY_I_II_T5	*/ 1,
    0,
    3,
    8,
    5,
    101,
    ACC_2,
    true,
    3, // 5-tariff of Energy KVArh - I+II rdch
    /*	DLMS_RDCH_ENERGY_I_II_T6	*/ 1,
    0,
    3,
    8,
    6,
    101,
    ACC_2,
    true,
    3, // 6-tariff of Energy KVArh - I+II rdch
    /*	DLMS_RDCH_ENERGY_I_II_T7	*/ 1,
    0,
    3,
    8,
    7,
    101,
    ACC_2,
    true,
    3, // 7-tariff of Energy KVArh - I+II rdch
    /*	DLMS_RDCH_ENERGY_I_II_T8	*/ 1,
    0,
    3,
    8,
    8,
    101,
    ACC_2,
    true,
    3, // 8-tariff of Energy KVArh - I+II rdch

    /*	DLMS_RDCH_ENERGY_III_IV_T1_OLD	*/ 1,
    0,
    4,
    17,
    1,
    255,
    ACC_2,
    true,
    3, // 1-tariff of Energy KVArh - III+IV rdch
    /*	DLMS_RDCH_ENERGY_III_IV_T2_OLD	*/ 1,
    0,
    4,
    17,
    2,
    255,
    ACC_2,
    true,
    3, // 2-tariff of Energy KVArh - III+IV rdch
    /*	DLMS_RDCH_ENERGY_III_IV_T3_OLD	*/ 1,
    0,
    4,
    17,
    3,
    255,
    ACC_2,
    true,
    3, // 3-tariff of Energy KVArh - III+IV rdch
    /*	DLMS_RDCH_ENERGY_III_IV_T4_OLD	*/ 1,
    0,
    4,
    17,
    4,
    255,
    ACC_2,
    true,
    3, // 4-tariff of Energy KVArh - III+IV rdch
    /*	DLMS_RDCH_ENERGY_III_IV_T5_OLD	*/ 1,
    0,
    4,
    17,
    5,
    255,
    ACC_2,
    true,
    3, // 5-tariff of Energy KVArh - III+IV rdch
    /*	DLMS_RDCH_ENERGY_III_IV_T6_OLD	*/ 1,
    0,
    4,
    17,
    6,
    255,
    ACC_2,
    true,
    3, // 6-tariff of Energy KVArh - III+IV rdch
    /*	DLMS_RDCH_ENERGY_III_IV_T7_OLD	*/ 1,
    0,
    4,
    17,
    7,
    255,
    ACC_2,
    true,
    3, // 7-tariff of Energy KVArh - III+IV rdch
    /*	DLMS_RDCH_ENERGY_III_IV_T8_OLD	*/ 1,
    0,
    4,
    17,
    8,
    255,
    ACC_2,
    true,
    3, // 8-tariff of Energy KVArh - III+IV rdch
    /*	DLMS_RDCH_ENERGY_III_IV_T1	*/ 1,
    0,
    4,
    8,
    1,
    101,
    ACC_2,
    true,
    3, // 1-tariff of Energy KVArh - III+IV rdch
    /*	DLMS_RDCH_ENERGY_III_IV_T2	*/ 1,
    0,
    4,
    8,
    2,
    101,
    ACC_2,
    true,
    3, // 2-tariff of Energy KVArh - III+IV rdch
    /*	DLMS_RDCH_ENERGY_III_IV_T3	*/ 1,
    0,
    4,
    8,
    3,
    101,
    ACC_2,
    true,
    3, // 3-tariff of Energy KVArh - III+IV rdch
    /*	DLMS_RDCH_ENERGY_III_IV_T4	*/ 1,
    0,
    4,
    8,
    4,
    101,
    ACC_2,
    true,
    3, // 4-tariff of Energy KVArh - III+IV rdch
    /*	DLMS_RDCH_ENERGY_III_IV_T5	*/ 1,
    0,
    4,
    8,
    5,
    101,
    ACC_2,
    true,
    3, // 5-tariff of Energy KVArh - III+IV rdch
    /*	DLMS_RDCH_ENERGY_III_IV_T6	*/ 1,
    0,
    4,
    8,
    6,
    101,
    ACC_2,
    true,
    3, // 6-tariff of Energy KVArh - III+IV rdch
    /*	DLMS_RDCH_ENERGY_III_IV_T7	*/ 1,
    0,
    4,
    8,
    7,
    101,
    ACC_2,
    true,
    3, // 7-tariff of Energy KVArh - III+IV rdch
    /*	DLMS_RDCH_ENERGY_III_IV_T8	*/ 1,
    0,
    4,
    8,
    8,
    101,
    ACC_2,
    true,
    3, // 8-tariff of Energy KVArh - III+IV rdch

    /*	DLMS_RDCH_TIME	*/ 0,
    0,
    0,
    9,
    1,
    101,
    ACC_2,
    false,
    3, // Time RDCH
    /*	DLMS_RDCH_DATE	*/ 0,
    0,
    0,
    9,
    2,
    101,
    ACC_2,
    false,
    3, // Date RDCH

    /*	DLMS_SERIAL_NUMBER_METER	*/ 0,
    0,
    96,
    1,
    0,
    255,
    ACC_2,
    false,
    4, // ˜˜˜˜˜˜˜˜ ˜˜˜˜˜ ˜˜˜˜˜˜˜˜
    /*	DLMS_TYPE_METER	*/ 0,
    0,
    96,
    1,
    1,
    255,
    ACC_2,
    false,
    4, // ˜˜˜ ˜˜˜˜˜˜˜˜
    /*	DLMS_FW_METER	*/ 0,
    0,
    96,
    1,
    2,
    255,
    ACC_2,
    false,
    4, // ˜˜˜˜˜˜ ˜˜ ˜˜˜˜˜˜˜˜
    /*	DLMS_OPERATING_TIME	*/ 0,
    0,
    96,
    8,
    0,
    255,
    ACC_2,
    false,
    4, // ˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜
    /*	DLMS_BATTERY_VOLTAGE	*/ 0,
    0,
    96,
    6,
    3,
    255,
    ACC_3,
    false,
    4, // Battary voltage
    /*	DLMS_TEMPERATURE	*/ 0,
    0,
    96,
    9,
    0,
    255,
    ACC_4,
    false,
    4, // ˜˜˜˜˜˜˜˜˜˜˜
    /*	DLMS_HDLC_SPEED_0	*/ 0,
    0,
    22,
    0,
    0,
    255,
    ACC_2,
    false,
    4, // HDLC setup - speed (AKROS)
    /*	DLMS_HDLC_ADDR_0	*/ 0,
    0,
    22,
    0,
    0,
    255,
    ACC_2,
    false,
    4, // HDLC setup - device address (AKROS)
    /*	DLMS_HDLC_SPEED_1	*/ 0,
    1,
    22,
    0,
    0,
    255,
    ACC_2,
    false,
    4, // HDLC setup - speed (AKROS)
    /*	DLMS_HDLC_ADDR_1	*/ 0,
    1,
    22,
    0,
    0,
    255,
    ACC_2,
    false,
    4, // HDLC setup - device address (AKROS)
    /*	DLMS_HDLC_SPEED_2	*/ 0,
    2,
    22,
    0,
    0,
    255,
    ACC_2,
    false,
    4, // HDLC setup - speed (AKROS)
    /*	DLMS_HDLC_ADDR_2	*/ 0,
    2,
    22,
    0,
    0,
    255,
    ACC_2,
    false,
    4, // HDLC setup - device address (AKROS)
    /*	DLMS_HDLC_SPEED_3	*/ 0,
    3,
    22,
    0,
    0,
    255,
    ACC_2,
    false,
    4, // HDLC setup - speed (AKROS)
    /*	DLMS_HDLC_ADDR_3	*/ 0,
    3,
    22,
    0,
    0,
    255,
    ACC_2,
    false,
    4, // HDLC setup - device address (AKROS)

    /*	DLMS_SERIAL_NUMBER_DISPLAY	*/ 0,
    0,
    96,
    1,
    150,
    255,
    ACC_2,
    false,
    4, // ˜˜˜˜˜˜˜˜ ˜˜˜˜˜ ˜˜˜˜˜˜
    /*	DLMS_TYPE_DISPLAY	*/ 0,
    0,
    96,
    1,
    151,
    255,
    ACC_2,
    false,
    4, // ˜˜˜ ˜˜˜˜˜˜
    /*	DLMS_FW_DISPLAY	*/ 0,
    0,
    96,
    1,
    152,
    255,
    ACC_2,
    false,
    4, // ˜˜˜˜˜˜ ˜˜ ˜˜˜˜˜˜

    /*	DLMS_LCD_DIAG	*/ 255,
    255,
    255,
    255,
    255,
    255,
    ACC_2,
    false,
    4, // ˜˜˜˜˜˜˜˜˜˜˜
    /*	DLMS_NO_DATA	*/ 255,
    255,
    255,
    255,
    255,
    255,
    ACC_2,
    false,
    4, // ˜˜˜ ˜˜˜˜˜˜
}; // END OBIS_codes
//----------------------------------------------------------------------------//
void ShowOBIS(objDLMS_t OBIS) { //
  dlms.fHz = false;
  dlms.fVolt = false;
  lcd.clear_OBIS_data();
  switch (OBIS) {
  case DLMS_BATTERY_VOLTAGE:
    lcd.show_val_pos(0, 17);
    lcd.show_val_pos(0, 18);
    lcd.show_val_pos(9, 19);
    lcd.show_val_pos(6, 20);
    lcd.show_val_pos(6, 21);
    lcd.show_val_pos(3, 22);
    lcd.text(SHOW, 4, OP1, OP2, OP4, OP5);
    break;
  case DLMS_SERIAL_NUMBER_METER:
    lcd.show_val_pos(0, 17);
    lcd.show_val_pos(0, 18);
    lcd.show_val_pos(9, 19);
    lcd.show_val_pos(6, 20);
    lcd.show_val_pos(1, 21);
    lcd.show_val_pos(0, 22);
    lcd.text(SHOW, 4, OP1, OP2, OP4, OP5);
    break;
  case DLMS_TYPE_METER:
    lcd.show_val_pos(0, 17);
    lcd.show_val_pos(0, 18);
    lcd.show_val_pos(9, 19);
    lcd.show_val_pos(6, 20);
    lcd.show_val_pos(1, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 4, OP1, OP2, OP4, OP5);
    break;
  case DLMS_FW_METER:
    lcd.show_val_pos(0, 17);
    lcd.show_val_pos(0, 18);
    lcd.show_val_pos(9, 19);
    lcd.show_val_pos(6, 20);
    lcd.show_val_pos(1, 21);
    lcd.show_val_pos(2, 22);
    lcd.text(SHOW, 4, OP1, OP2, OP4, OP5);
    break;
  case DLMS_OPERATING_TIME:
    lcd.showText("\xC2\xD0\xC5\xCC\xDF\xD0"); // VREMYAR
    lcd.show_val_pos(0, 17);
    lcd.show_val_pos(0, 18);
    lcd.show_val_pos(9, 19);
    lcd.show_val_pos(6, 20);
    lcd.show_val_pos(8, 21);
    lcd.show_val_pos(0, 22);
    lcd.text(SHOW, 4, OP1, OP2, OP4, OP5);
    break;
  case DLMS_ACTIVE_ENERGY_T1:
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(1, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_ACTIVE_ENERGY_T2:
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(2, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_ACTIVE_ENERGY_T3:
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(3, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_ACTIVE_ENERGY_T4:
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(4, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_ACTIVE_ENERGY_T5:
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(5, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_ACTIVE_ENERGY_T6:
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(6, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_ACTIVE_ENERGY_T7:
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(7, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_ACTIVE_ENERGY_T8:
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(8, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_ENERGY_I_IV:
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(0, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_ENERGY_II_III:
    lcd.show_val_pos(2, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(0, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_ENERGY_I_II:
    lcd.show_val_pos(3, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(0, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_ENERGY_III_IV:
    lcd.show_val_pos(4, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(0, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_ACTIVE_POWER_PLUS:
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(7, 18);
    lcd.show_val_pos(0, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_L1_ACTIVE_POWER_PLUS:
    lcd.show_val_pos(2, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(0, 20);
    lcd.text(SHOW, 2, OP2, OP3);
    break;
  case DLMS_L2_ACTIVE_POWER_PLUS:
    lcd.show_val_pos(4, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(0, 20);
    lcd.text(SHOW, 2, OP2, OP3);
    break;
  case DLMS_L3_ACTIVE_POWER_PLUS:
    lcd.show_val_pos(6, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(0, 20);
    lcd.text(SHOW, 2, OP2, OP3);
    break;
  case DLMS_REACTIVE_POWER_PLUS:
    lcd.show_val_pos(3, 17);
    lcd.show_val_pos(7, 18);
    lcd.show_val_pos(0, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_L1_REACTIVE_POWER_PLUS:
    lcd.show_val_pos(2, 17);
    lcd.show_val_pos(3, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(0, 20);
    lcd.text(SHOW, 2, OP2, OP3);
    break;
  case DLMS_L2_REACTIVE_POWER_PLUS:
    lcd.show_val_pos(4, 17);
    lcd.show_val_pos(3, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(0, 20);
    lcd.text(SHOW, 2, OP2, OP3);
    break;
  case DLMS_L3_REACTIVE_POWER_PLUS:
    lcd.show_val_pos(6, 17);
    lcd.show_val_pos(3, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(0, 20);
    lcd.text(SHOW, 2, OP2, OP3);
    break;
  case DLMS_APPARENT_POWER_PLUS:
    lcd.show_val_pos(9, 17);
    lcd.show_val_pos(7, 18);
    lcd.show_val_pos(0, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_L1_APPARENT_POWER_PLUS:
    lcd.show_val_pos(2, 17);
    lcd.show_val_pos(9, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(0, 20);
    lcd.text(SHOW, 2, OP2, OP3);
    break;
  case DLMS_L2_APPARENT_POWER_PLUS:
    lcd.show_val_pos(4, 17);
    lcd.show_val_pos(9, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(0, 20);
    lcd.text(SHOW, 2, OP2, OP3);
    break;
  case DLMS_L3_APPARENT_POWER_PLUS:
    lcd.show_val_pos(6, 17);
    lcd.show_val_pos(9, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(0, 20);
    lcd.text(SHOW, 2, OP2, OP3);
    break;
  case DLMS_VOLTAGE:
    dlms.fVolt = true;
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(2, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(0, 20);
    lcd.text(SHOW, 2, OP2, OP3);
    break;
  case DLMS_L1_VOLTAGE:
    dlms.fVolt = true;
    lcd.show_val_pos(3, 17);
    lcd.show_val_pos(2, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(0, 20);
    lcd.text(SHOW, 2, OP2, OP3);
    break;
  case DLMS_L2_VOLTAGE:
    dlms.fVolt = true;
    lcd.show_val_pos(5, 17);
    lcd.show_val_pos(2, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(0, 20);
    lcd.text(SHOW, 2, OP2, OP3);
    break;
  case DLMS_L3_VOLTAGE:
    dlms.fVolt = true;
    lcd.show_val_pos(7, 17);
    lcd.show_val_pos(2, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(0, 20);
    lcd.text(SHOW, 2, OP2, OP3);
    break;
  case DLMS_CURRENT:
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(0, 20);
    lcd.text(SHOW, 2, OP2, OP3);
    break;
  case DLMS_L1_CURRENT:
    lcd.show_val_pos(3, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(0, 20);
    lcd.text(SHOW, 2, OP2, OP3);
    break;
  case DLMS_L2_CURRENT:
    lcd.show_val_pos(5, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(0, 20);
    lcd.text(SHOW, 2, OP2, OP3);
    break;
  case DLMS_L3_CURRENT:
    lcd.show_val_pos(7, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(0, 20);
    lcd.text(SHOW, 2, OP2, OP3);
    break;
  case DLMS_NEUTRAL_CURRENT:
    lcd.show_val_pos(9, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(0, 20);
    lcd.text(SHOW, 2, OP2, OP3);
    break;
  case DLMS_SUPPLY_FREQUENCY:
    dlms.fHz = true;
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(4, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(0, 20);
    lcd.text(SHOW, 2, OP2, OP3);
    break;
  case DLMS_NOMINAL_VOLTAGE: // 1.0.0.6.0
    dlms.fVolt = true;
    lcd.showText("\xCD\xCE\xCC"); // NOM
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(0, 18);
    lcd.show_val_pos(0, 19);
    lcd.show_val_pos(6, 20);
    lcd.show_val_pos(0, 21);
    lcd.text(SHOW, 4, OP1, OP2, OP3, OP4);
    break;
  case DLMS_NOMINAL_CURRENT:      // 1.0.0.6.1
    lcd.showText("\xCD\xCE\xCC"); // NOM
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(0, 18);
    lcd.show_val_pos(0, 19);
    lcd.show_val_pos(6, 20);
    lcd.show_val_pos(1, 21);
    lcd.text(SHOW, 4, OP1, OP2, OP3, OP4);
    break;
  case DLMS_NOMINAL_FREQUENCY:
    lcd.showText("\xCD\xCE\xCC"); // NOM
    dlms.fHz = true;              // 1.0.0.6.2
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(0, 18);
    lcd.show_val_pos(0, 19);
    lcd.show_val_pos(6, 20);
    lcd.show_val_pos(2, 21);
    lcd.text(SHOW, 4, OP1, OP2, OP3, OP4);
    break;
  case DLMS_HDLC_SPEED_0:             // 0.0.22.0.0
    lcd.showText("\xD1\xCA\xCE\xD0"); // SKOR
    lcd.show_val_pos(0, 17);
    lcd.show_val_pos(0, 18);
    lcd.show_val_pos(2, 19);
    lcd.show_val_pos(2, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(0, 22);
    lcd.text(SHOW, 4, OP1, OP2, OP4, OP5);
    break;
  case DLMS_HDLC_SPEED_1:             // 0.1.22.0.0
    lcd.showText("\xD1\xCA\xCE\xD0"); // SKOR
    lcd.show_val_pos(0, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(2, 19);
    lcd.show_val_pos(2, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(0, 22);
    lcd.text(SHOW, 4, OP1, OP2, OP4, OP5);
    break;
  case DLMS_HDLC_SPEED_2:             // 0.2.22.0.0
    lcd.showText("\xD1\xCA\xCE\xD0"); // SKOR
    lcd.show_val_pos(0, 17);
    lcd.show_val_pos(2, 18);
    lcd.show_val_pos(2, 19);
    lcd.show_val_pos(2, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(0, 22);
    lcd.text(SHOW, 4, OP1, OP2, OP4, OP5);
    break;
  case DLMS_HDLC_SPEED_3:             // 0.3.22.0.0
    lcd.showText("\xD1\xCA\xCE\xD0"); // SKOR
    lcd.show_val_pos(0, 17);
    lcd.show_val_pos(3, 18);
    lcd.show_val_pos(2, 19);
    lcd.show_val_pos(2, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(0, 22);
    lcd.text(SHOW, 4, OP1, OP2, OP4, OP5);
    break;
  case DLMS_HDLC_ADDR_0:                  // 0.0.22.0.0 - address
    lcd.showText("\xC0\xC4\xD0\xC5\xD1"); // ADRES
    lcd.show_val_pos(0, 17);
    lcd.show_val_pos(0, 18);
    lcd.show_val_pos(2, 19);
    lcd.show_val_pos(2, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(0, 22);
    lcd.text(SHOW, 4, OP1, OP2, OP4, OP5);
    break;
  case DLMS_HDLC_ADDR_1:                  // 0.1.22.0.0 - address
    lcd.showText("\xC0\xC4\xD0\xC5\xD1"); // ADRES
    lcd.show_val_pos(0, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(2, 19);
    lcd.show_val_pos(2, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(0, 22);
    lcd.text(SHOW, 4, OP1, OP2, OP4, OP5);
    break;
  case DLMS_HDLC_ADDR_2:                  // 0.2.22.0.0 - address
    lcd.showText("\xC0\xC4\xD0\xC5\xD1"); // ADRES
    lcd.show_val_pos(0, 17);
    lcd.show_val_pos(2, 18);
    lcd.show_val_pos(2, 19);
    lcd.show_val_pos(2, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(0, 22);
    lcd.text(SHOW, 4, OP1, OP2, OP4, OP5);
    break;
  case DLMS_HDLC_ADDR_3:                  // 0.3.22.0.0 - address
    lcd.showText("\xC0\xC4\xD0\xC5\xD1"); // ADRES
    lcd.show_val_pos(0, 17);
    lcd.show_val_pos(3, 18);
    lcd.show_val_pos(2, 19);
    lcd.show_val_pos(2, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(0, 22);
    lcd.text(SHOW, 4, OP1, OP2, OP4, OP5);
    break;
  case DLMS_TIME:
    lcd.showText("\xC2\xD0\xC5\xCC\xDF"); // VREMYA
    if (dlmsClockAkros) {                 // AKROS: Clock 0.0.1.0.0
      lcd.show_val_pos(0, 17);
      lcd.show_val_pos(0, 18);
      lcd.show_val_pos(1, 19);
      lcd.show_val_pos(0, 20);
      lcd.show_val_pos(0, 21);
    } else {
      lcd.show_val_pos(0, 17);
      lcd.show_val_pos(0, 18);
      lcd.show_val_pos(0, 19);
      lcd.show_val_pos(9, 20);
      lcd.show_val_pos(1, 21);
    }
    lcd.text(SHOW, 4, OP1, OP2, OP3, OP4);
    break;
  case DLMS_DATE:
    lcd.showText("\xC4\xC0\xD2\xC0"); // DATA
    if (dlmsClockAkros) {             // AKROS: Clock 0.0.1.0.0
      lcd.show_val_pos(0, 17);
      lcd.show_val_pos(0, 18);
      lcd.show_val_pos(1, 19);
      lcd.show_val_pos(0, 20);
      lcd.show_val_pos(0, 21);
    } else {
      lcd.show_val_pos(0, 17);
      lcd.show_val_pos(0, 18);
      lcd.show_val_pos(0, 19);
      lcd.show_val_pos(9, 20);
      lcd.show_val_pos(2, 21);
    }
    lcd.text(SHOW, 4, OP1, OP2, OP3, OP4);
    break;
  case DLMS_RDCH_ACTIVE_ENERGY_T1_OLD:
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(1, 20);
    lcd.text(SHOW, 2, OP1, OP3);
    break;
  case DLMS_RDCH_ACTIVE_ENERGY_T2_OLD:
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(2, 20);
    lcd.text(SHOW, 2, OP1, OP3);
    break;
  case DLMS_RDCH_ACTIVE_ENERGY_T3_OLD:
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(3, 20);
    lcd.text(SHOW, 2, OP1, OP3);
    break;
  case DLMS_RDCH_ACTIVE_ENERGY_T4_OLD:
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(4, 20);
    lcd.text(SHOW, 2, OP1, OP3);
    break;
  case DLMS_RDCH_ACTIVE_ENERGY_T5_OLD:
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(5, 20);
    lcd.text(SHOW, 2, OP1, OP3);
    break;
  case DLMS_RDCH_ACTIVE_ENERGY_T6_OLD:
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(6, 20);
    lcd.text(SHOW, 2, OP1, OP3);
    break;
  case DLMS_RDCH_ACTIVE_ENERGY_T7_OLD:
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(7, 20);
    lcd.text(SHOW, 2, OP1, OP3);
    break;
  case DLMS_RDCH_ACTIVE_ENERGY_T8_OLD:
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(8, 20);
    lcd.text(SHOW, 2, OP1, OP3);
    break;
  case DLMS_RDCH_ACTIVE_ENERGY_T1:
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(1, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 3, OP1, OP2, OP3);
    break;
  case DLMS_RDCH_ACTIVE_ENERGY_T2:
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(2, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 3, OP1, OP2, OP3);
    break;
  case DLMS_RDCH_ACTIVE_ENERGY_T3:
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(3, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 3, OP1, OP2, OP3);
    break;
  case DLMS_RDCH_ACTIVE_ENERGY_T4:
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(4, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 3, OP1, OP2, OP3);
    break;
  case DLMS_RDCH_ACTIVE_ENERGY_T5:
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(5, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 3, OP1, OP2, OP3);
    break;
  case DLMS_RDCH_ACTIVE_ENERGY_T6:
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(6, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 3, OP1, OP2, OP3);
    break;
  case DLMS_RDCH_ACTIVE_ENERGY_T7:
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 3, OP1, OP2, OP3);
    break;
  case DLMS_RDCH_ACTIVE_ENERGY_T8:
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(8, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 3, OP1, OP2, OP3);
    break;
  case DLMS_RDCH_ENERGY_I_IV_OLD:
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(0, 20);
    lcd.text(SHOW, 2, OP1, OP3);
    break;
  case DLMS_RDCH_ENERGY_II_III_OLD:
    lcd.show_val_pos(2, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(0, 20);
    lcd.text(SHOW, 2, OP1, OP3);
    break;
  case DLMS_RDCH_ENERGY_I_II_OLD:
    lcd.show_val_pos(3, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(0, 20);
    lcd.text(SHOW, 2, OP1, OP3);
    break;
  case DLMS_RDCH_ENERGY_III_IV_OLD:
    lcd.show_val_pos(4, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(0, 20);
    lcd.text(SHOW, 2, OP1, OP3);
    break;
  case DLMS_RDCH_ENERGY_I_IV:
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(0, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 3, OP1, OP2, OP3);
    break;
  case DLMS_RDCH_ENERGY_II_III:
    lcd.show_val_pos(2, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(0, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 3, OP1, OP2, OP3);
    break;
  case DLMS_RDCH_ENERGY_I_II:
    lcd.show_val_pos(3, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(0, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 3, OP1, OP2, OP3);
    break;
  case DLMS_RDCH_ENERGY_III_IV:
    lcd.show_val_pos(4, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(0, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 3, OP1, OP2, OP3);
    break;
  case DLMS_TEMPERATURE:
    lcd.showText("\xD2\xC5\xCC\xCF\xC5\xD0"); // TEMPER
    lcd.show_val_pos(0, 17);
    lcd.show_val_pos(0, 18);
    lcd.show_val_pos(9, 19);
    lcd.show_val_pos(6, 20);
    lcd.show_val_pos(9, 21);
    lcd.show_val_pos(0, 22);
    lcd.text(SHOW, 4, OP1, OP2, OP4, OP5);
    break;
  case DLMS_ENERGY_II_III_T1:
    lcd.show_val_pos(2, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(1, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_ENERGY_II_III_T2:
    lcd.show_val_pos(2, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(2, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_ENERGY_II_III_T3:
    lcd.show_val_pos(2, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(3, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_ENERGY_II_III_T4:
    lcd.show_val_pos(2, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(4, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_ENERGY_II_III_T5:
    lcd.show_val_pos(2, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(5, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_ENERGY_II_III_T6:
    lcd.show_val_pos(2, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(6, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_ENERGY_II_III_T7:
    lcd.show_val_pos(2, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(7, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_ENERGY_II_III_T8:
    lcd.show_val_pos(2, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(8, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_ENERGY_I_II_T1:
    lcd.show_val_pos(3, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(1, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_ENERGY_I_II_T2:
    lcd.show_val_pos(3, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(2, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_ENERGY_I_II_T3:
    lcd.show_val_pos(3, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(3, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_ENERGY_I_II_T4:
    lcd.show_val_pos(3, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(4, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_ENERGY_I_II_T5:
    lcd.show_val_pos(3, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(5, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_ENERGY_I_II_T6:
    lcd.show_val_pos(3, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(6, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_ENERGY_I_II_T7:
    lcd.show_val_pos(3, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(7, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_ENERGY_I_II_T8:
    lcd.show_val_pos(3, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(8, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_ENERGY_III_IV_T1:
    lcd.show_val_pos(4, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(1, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_ENERGY_III_IV_T2:
    lcd.show_val_pos(4, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(2, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_ENERGY_III_IV_T3:
    lcd.show_val_pos(4, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(3, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_ENERGY_III_IV_T4:
    lcd.show_val_pos(4, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(4, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_ENERGY_III_IV_T5:
    lcd.show_val_pos(4, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(5, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_ENERGY_III_IV_T6:
    lcd.show_val_pos(4, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(6, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_ENERGY_III_IV_T7:
    lcd.show_val_pos(4, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(7, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_ENERGY_III_IV_T8:
    lcd.show_val_pos(4, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(8, 19);
    lcd.text(SHOW, 2, OP1, OP2);
    break;
  case DLMS_RDCH_ENERGY_II_III_T1_OLD:
    lcd.show_val_pos(2, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(1, 20);
    lcd.text(SHOW, 2, OP1, OP3);
    break;
  case DLMS_RDCH_ENERGY_II_III_T2_OLD:
    lcd.show_val_pos(2, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(2, 20);
    lcd.text(SHOW, 2, OP1, OP3);
    break;
  case DLMS_RDCH_ENERGY_II_III_T3_OLD:
    lcd.show_val_pos(2, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(3, 20);
    lcd.text(SHOW, 2, OP1, OP3);
    break;
  case DLMS_RDCH_ENERGY_II_III_T4_OLD:
    lcd.show_val_pos(2, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(4, 20);
    lcd.text(SHOW, 2, OP1, OP3);
    break;
  case DLMS_RDCH_ENERGY_II_III_T5_OLD:
    lcd.show_val_pos(2, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(5, 20);
    lcd.text(SHOW, 2, OP1, OP3);
    break;
  case DLMS_RDCH_ENERGY_II_III_T6_OLD:
    lcd.show_val_pos(2, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(6, 20);
    lcd.text(SHOW, 2, OP1, OP3);
    break;
  case DLMS_RDCH_ENERGY_II_III_T7_OLD:
    lcd.show_val_pos(2, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(7, 20);
    lcd.text(SHOW, 2, OP1, OP3);
    break;
  case DLMS_RDCH_ENERGY_II_III_T8_OLD:
    lcd.show_val_pos(2, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(8, 20);
    lcd.text(SHOW, 2, OP1, OP3);
    break;
  case DLMS_RDCH_ENERGY_II_III_T1:
    lcd.show_val_pos(2, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(1, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 3, OP1, OP2, OP3);
    break;
  case DLMS_RDCH_ENERGY_II_III_T2:
    lcd.show_val_pos(2, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(2, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 3, OP1, OP2, OP3);
    break;
  case DLMS_RDCH_ENERGY_II_III_T3:
    lcd.show_val_pos(2, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(3, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 3, OP1, OP2, OP3);
    break;
  case DLMS_RDCH_ENERGY_II_III_T4:
    lcd.show_val_pos(2, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(4, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 3, OP1, OP2, OP3);
    break;
  case DLMS_RDCH_ENERGY_II_III_T5:
    lcd.show_val_pos(2, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(5, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 3, OP1, OP2, OP3);
    break;
  case DLMS_RDCH_ENERGY_II_III_T6:
    lcd.show_val_pos(2, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(6, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 3, OP1, OP2, OP3);
    break;
  case DLMS_RDCH_ENERGY_II_III_T7:
    lcd.show_val_pos(2, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 3, OP1, OP2, OP3);
    break;
  case DLMS_RDCH_ENERGY_II_III_T8:
    lcd.show_val_pos(2, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(8, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 3, OP1, OP2, OP3);
    break;
  case DLMS_RDCH_ENERGY_I_II_T1_OLD:
    lcd.show_val_pos(3, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(1, 20);
    lcd.text(SHOW, 2, OP1, OP3);
    break;
  case DLMS_RDCH_ENERGY_I_II_T2_OLD:
    lcd.show_val_pos(3, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(2, 20);
    lcd.text(SHOW, 2, OP1, OP3);
    break;
  case DLMS_RDCH_ENERGY_I_II_T3_OLD:
    lcd.show_val_pos(3, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(3, 20);
    lcd.text(SHOW, 2, OP1, OP3);
    break;
  case DLMS_RDCH_ENERGY_I_II_T4_OLD:
    lcd.show_val_pos(3, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(4, 20);
    lcd.text(SHOW, 2, OP1, OP3);
    break;
  case DLMS_RDCH_ENERGY_I_II_T5_OLD:
    lcd.show_val_pos(3, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(5, 20);
    lcd.text(SHOW, 2, OP1, OP3);
    break;
  case DLMS_RDCH_ENERGY_I_II_T6_OLD:
    lcd.show_val_pos(3, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(6, 20);
    lcd.text(SHOW, 2, OP1, OP3);
    break;
  case DLMS_RDCH_ENERGY_I_II_T7_OLD:
    lcd.show_val_pos(3, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(7, 20);
    lcd.text(SHOW, 2, OP1, OP3);
    break;
  case DLMS_RDCH_ENERGY_I_II_T8_OLD:
    lcd.show_val_pos(3, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(8, 20);
    lcd.text(SHOW, 2, OP1, OP3);
    break;
  case DLMS_RDCH_ENERGY_I_II_T1:
    lcd.show_val_pos(3, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(1, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 3, OP1, OP2, OP3);
    break;
  case DLMS_RDCH_ENERGY_I_II_T2:
    lcd.show_val_pos(3, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(2, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 3, OP1, OP2, OP3);
    break;
  case DLMS_RDCH_ENERGY_I_II_T3:
    lcd.show_val_pos(3, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(3, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 3, OP1, OP2, OP3);
    break;
  case DLMS_RDCH_ENERGY_I_II_T4:
    lcd.show_val_pos(3, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(4, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 3, OP1, OP2, OP3);
    break;
  case DLMS_RDCH_ENERGY_I_II_T5:
    lcd.show_val_pos(3, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(5, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 3, OP1, OP2, OP3);
    break;
  case DLMS_RDCH_ENERGY_I_II_T6:
    lcd.show_val_pos(3, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(6, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 3, OP1, OP2, OP3);
    break;
  case DLMS_RDCH_ENERGY_I_II_T7:
    lcd.show_val_pos(3, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 3, OP1, OP2, OP3);
    break;
  case DLMS_RDCH_ENERGY_I_II_T8:
    lcd.show_val_pos(3, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(8, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 3, OP1, OP2, OP3);
    break;
  case DLMS_RDCH_ENERGY_III_IV_T1_OLD:
    lcd.show_val_pos(4, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(1, 20);
    lcd.text(SHOW, 2, OP1, OP3);
    break;
  case DLMS_RDCH_ENERGY_III_IV_T2_OLD:
    lcd.show_val_pos(4, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(2, 20);
    lcd.text(SHOW, 2, OP1, OP3);
    break;
  case DLMS_RDCH_ENERGY_III_IV_T3_OLD:
    lcd.show_val_pos(4, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(3, 20);
    lcd.text(SHOW, 2, OP1, OP3);
    break;
  case DLMS_RDCH_ENERGY_III_IV_T4_OLD:
    lcd.show_val_pos(4, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(4, 20);
    lcd.text(SHOW, 2, OP1, OP3);
    break;
  case DLMS_RDCH_ENERGY_III_IV_T5_OLD:
    lcd.show_val_pos(4, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(5, 20);
    lcd.text(SHOW, 2, OP1, OP3);
    break;
  case DLMS_RDCH_ENERGY_III_IV_T6_OLD:
    lcd.show_val_pos(4, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(6, 20);
    lcd.text(SHOW, 2, OP1, OP3);
    break;
  case DLMS_RDCH_ENERGY_III_IV_T7_OLD:
    lcd.show_val_pos(4, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(7, 20);
    lcd.text(SHOW, 2, OP1, OP3);
    break;
  case DLMS_RDCH_ENERGY_III_IV_T8_OLD:
    lcd.show_val_pos(4, 17);
    lcd.show_val_pos(1, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(8, 20);
    lcd.text(SHOW, 2, OP1, OP3);
    break;
  case DLMS_RDCH_ENERGY_III_IV_T1:
    lcd.show_val_pos(4, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(1, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 3, OP1, OP2, OP3);
    break;
  case DLMS_RDCH_ENERGY_III_IV_T2:
    lcd.show_val_pos(4, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(2, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 3, OP1, OP2, OP3);
    break;
  case DLMS_RDCH_ENERGY_III_IV_T3:
    lcd.show_val_pos(4, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(3, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 3, OP1, OP2, OP3);
    break;
  case DLMS_RDCH_ENERGY_III_IV_T4:
    lcd.show_val_pos(4, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(4, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 3, OP1, OP2, OP3);
    break;
  case DLMS_RDCH_ENERGY_III_IV_T5:
    lcd.show_val_pos(4, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(5, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 3, OP1, OP2, OP3);
    break;
  case DLMS_RDCH_ENERGY_III_IV_T6:
    lcd.show_val_pos(4, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(6, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 3, OP1, OP2, OP3);
    break;
  case DLMS_RDCH_ENERGY_III_IV_T7:
    lcd.show_val_pos(4, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 3, OP1, OP2, OP3);
    break;
  case DLMS_RDCH_ENERGY_III_IV_T8:
    lcd.show_val_pos(4, 17);
    lcd.show_val_pos(8, 18);
    lcd.show_val_pos(8, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(0, 21);
    lcd.show_val_pos(1, 22);
    lcd.text(SHOW, 3, OP1, OP2, OP3);
    break;
  case DLMS_RDCH_TIME:
    lcd.show_val_pos(0, 17);
    lcd.show_val_pos(0, 18);
    lcd.show_val_pos(9, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(1, 21);
    lcd.show_val_pos(0, 22);
    lcd.show_val_pos(1, 23);
    lcd.text(SHOW, 4, OP1, OP2, OP3, OP4);
    break;
  case DLMS_RDCH_DATE:
    lcd.show_val_pos(0, 17);
    lcd.show_val_pos(0, 18);
    lcd.show_val_pos(9, 19);
    lcd.show_val_pos(2, 20);
    lcd.show_val_pos(1, 21);
    lcd.show_val_pos(0, 22);
    lcd.show_val_pos(1, 23);
    lcd.text(SHOW, 4, OP1, OP2, OP3, OP4);
    break;
  case DLMS_L1_L2_VOLTAGE_OLD:
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(2, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(1, 20);
    lcd.text(SHOW, 2, OP2, OP3);
    break;
  case DLMS_L2_L3_VOLTAGE_OLD:
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(2, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(2, 20);
    lcd.text(SHOW, 2, OP2, OP3);
    break;
  case DLMS_L1_L3_VOLTAGE_OLD:
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(2, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(3, 20);
    lcd.text(SHOW, 2, OP2, OP3);
    break;
  case DLMS_L1_L2_VOLTAGE:
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(0, 18);
    lcd.show_val_pos(1, 19);
    lcd.show_val_pos(2, 20);
    lcd.show_val_pos(4, 21);
    lcd.show_val_pos(7, 22);
    lcd.show_val_pos(0, 23);
    lcd.text(SHOW, 4, OP1, OP2, OP5, OP6);
    break;
  case DLMS_L2_L3_VOLTAGE:
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(0, 18);
    lcd.show_val_pos(1, 19);
    lcd.show_val_pos(2, 20);
    lcd.show_val_pos(5, 21);
    lcd.show_val_pos(7, 22);
    lcd.show_val_pos(0, 23);
    lcd.text(SHOW, 4, OP1, OP2, OP5, OP6);
    break;
  case DLMS_L1_L3_VOLTAGE:
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(0, 18);
    lcd.show_val_pos(1, 19);
    lcd.show_val_pos(2, 20);
    lcd.show_val_pos(6, 21);
    lcd.show_val_pos(7, 22);
    lcd.show_val_pos(0, 23);
    lcd.text(SHOW, 4, OP1, OP2, OP5, OP6);
    break;
  case DLMS_COS:
    lcd.show_val_pos(1, 17);
    lcd.show_val_pos(3, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(0, 20);
    lcd.text(SHOW, 2, OP2, OP3);
    break;
  case DLMS_L1_COS:
    lcd.show_val_pos(3, 17);
    lcd.show_val_pos(3, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(0, 20);
    lcd.text(SHOW, 2, OP2, OP3);
    break;
  case DLMS_L2_COS:
    lcd.show_val_pos(5, 17);
    lcd.show_val_pos(3, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(0, 20);
    lcd.text(SHOW, 2, OP2, OP3);
    break;
  case DLMS_L3_COS:
    lcd.show_val_pos(7, 17);
    lcd.show_val_pos(3, 18);
    lcd.show_val_pos(7, 19);
    lcd.show_val_pos(0, 20);
    lcd.text(SHOW, 2, OP2, OP3);
    break;
  case DLMS_SERIAL_NUMBER_DISPLAY:
    lcd.show_val_pos(0, 17);
    lcd.show_val_pos(9, 18);
    lcd.show_val_pos(6, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(1, 21);
    lcd.show_val_pos(5, 22);
    lcd.show_val_pos(0, 23);
    lcd.text(SHOW, 3, OP1, OP3, OP4);
    break;
  case DLMS_TYPE_DISPLAY:
    lcd.show_val_pos(0, 17);
    lcd.show_val_pos(9, 18);
    lcd.show_val_pos(6, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(1, 21);
    lcd.show_val_pos(5, 22);
    lcd.show_val_pos(1, 23);
    lcd.text(SHOW, 3, OP1, OP3, OP4);
    break;
  case DLMS_FW_DISPLAY:
    lcd.show_val_pos(0, 17);
    lcd.show_val_pos(9, 18);
    lcd.show_val_pos(6, 19);
    lcd.show_val_pos(1, 20);
    lcd.show_val_pos(1, 21);
    lcd.show_val_pos(5, 22);
    lcd.show_val_pos(2, 23);
    lcd.text(SHOW, 3, OP1, OP3, OP4);
    break;
  default:
    break;
  }
}
//----------------------------------------------------------------------------//
void ShowData(obj_t element) {
  double tmpVal, tmp;
  lcd.text(HIDE, 1, M); // MW symbol (AKROS large power)

  if ((element.pConst->OBIS[2] == 22) &&
      (element.type == TAG_UINT16)) { // AKROS: HDLC device address
    lcd.show_long((s32)((u16)element.value), false, false, 0);
    return;
  }

  switch (element.type) {
  case TAG_ENUM: { // AKROS: IEC HDLC setup (class 23) - communication speed
    static const s32 hdlcBaud[10] = {300,  600,   1200,  2400,  4800,
                                     9600, 19200, 38400, 57600, 115200};
    if (element.pConst->OBIS[2] == 22) {
      if (element.value < 10)
        lcd.show_long(hdlcBaud[element.value], false, false, 0);
      else
        lcd.show_long((s32)element.value, false, false, 0);
    }
    return;
  }
  case TAG_FLOAT32: { // float32 bits are stored in the low 32 bits of value
                      // (AKROS)
    u32 raw = (u32)element.value;
    float f;
    memcpy(&f, &raw, sizeof(f));
    tmpVal = (double)f;
  } break;
  case TAG_INT32:
    tmpVal = (double)((s32)element.value);
    break;
  case TAG_UINT32: {
    if (0 == memcmp(element.pConst->OBIS,
                    dlms.obj[DLMS_OPERATING_TIME].pConst->OBIS,
                    6)) { // ˜˜˜˜ ˜˜˜˜˜
      lcd.show_long(element.value / 60, false, false, 0);
      return;
    } else
      tmpVal = (double)((u32)element.value);
  } break;
  case TAG_UINT16:
    tmpVal = (double)((u16)element.value);
    break;
  case TAG_INT16:
    tmpVal = (double)((s16)element.value);
    break;
  case TAG_UINT64:
    tmpVal = (double)((u64)element.value);
    break;
  case TAG_INT64:
    tmpVal = (double)((s64)element.value);
    break;
  case TAG_OCTET_STRING: {
    if ((0 ==
         memcmp(element.pConst->OBIS, dlms.obj[DLMS_TIME].pConst->OBIS, 6)) ||
        (0 == memcmp(element.pConst->OBIS,
                     dlms.obj[DLMS_RDCH_TIME].pConst->OBIS, 6))) { // ˜˜˜˜ ˜˜˜˜˜
      lcd.clear_data();
      RTC_TimeTypeDef RTC_Time;
      RTC_Time.Hours = BYTE_3(element.value);
      RTC_Time.Minutes = BYTE_2(element.value);
      RTC_Time.Seconds = BYTE_1(element.value);
      lcd.show_time(&RTC_Time);
      return;
    } else if ((0 == memcmp(element.pConst->OBIS,
                            dlms.obj[DLMS_DATE].pConst->OBIS, 6)) ||
               (0 == memcmp(element.pConst->OBIS,
                            dlms.obj[DLMS_RDCH_DATE].pConst->OBIS,
                            6))) { // ˜˜˜˜ ˜˜˜˜
      RTC_TimeTypeDef RTC_Time;
      RTC_Time.Date = BYTE_0(element.value);
      RTC_Time.Month = BYTE_1(element.value);
      RTC_Time.Year = WORD(BYTE_3(element.value), BYTE_2(element.value));
      lcd.show_date(&RTC_Time);
      lcd.text(SHOW, 2, PP2, PP4);
      return;
    } else if (0 == memcmp(element.pConst->OBIS,
                           dlms.obj[DLMS_SERIAL_NUMBER_METER].pConst->OBIS,
                           6)) {
      lcd.show_long(element.value, true, false, 0);
      return;
    } else if (0 == memcmp(element.pConst->OBIS,
                           dlms.obj[DLMS_TYPE_METER].pConst->OBIS, 6)) {
      lcd.show_val_pos(element.value % 10, 13);
      lcd.show_val_pos((element.value % 100) / 10, 12);
      lcd.show_val_pos((element.value % 1000) / 100, 11);
      lcd.show_val_pos((element.value % 10000) / 1000, 10);
      lcd.show_val_pos(element.value / 10000, 9);
      lcd.text(SHOW, 1, PP4);
      return;
    } else if (0 == memcmp(element.pConst->OBIS,
                           dlms.obj[DLMS_FW_METER].pConst->OBIS, 6)) {
      lcd.show_val_pos(element.value % 10, 13);
      lcd.show_val_pos((element.value % 100) / 10, 12);
      lcd.show_val_pos(element.value / 100, 11);
      lcd.text(SHOW, 1, PP4);
      return;
    } else
      return;
  }
  case TAG_NULLDATA: {
    if (0 == memcmp(element.pConst->OBIS,
                    dlms.obj[DLMS_SERIAL_NUMBER_DISPLAY].pConst->OBIS, 6)) {
      lcd.show_long(SystemInfo.serialNumber, true, false, 0);
      lcd.showText("\xCD"
                   "O"
                   "\xCC \xC4\xC4"); // NOM DD (cp1251 codes)
      lcd.text(SHOW, 1, CP3);
      return;
    } else if (0 == memcmp(element.pConst->OBIS,
                           dlms.obj[DLMS_TYPE_DISPLAY].pConst->OBIS, 6)) {
      lcd.show_val_pos(0, 9);
      lcd.show_val_pos(4, 10);
      lcd.show_val_pos(0, 11);
      lcd.show_val_pos(4, 12);
      lcd.show_val_pos(0, 13);
      lcd.showText("\xD2\xC8\xCF \xC4\xC4"); // TIP DD (cp1251 codes)
      lcd.text(SHOW, 1, PP4);
      return;
    } else if (0 == memcmp(element.pConst->OBIS,
                           dlms.obj[DLMS_FW_DISPLAY].pConst->OBIS, 6)) {
      u32 tmp = (FW_VERSION_MAJOR * 1000000) + (FW_VERSION_MINOR * 10000) +
                (FW_VERSION_PATCH * 100) + FW_VERSION_BUILD;
      lcd.show_long(tmp, false, false, 2); // ˜˜˜˜˜ ˜˜˜˜˜˜˜˜
      lcd.text(SHOW, 3, PP2, PP4, PP6);
      lcd.showText("\xCF\xCE \xC4\xC4"); // PO DD (cp1251 codes)
      return;
    } else
      return;
  }
  default:
    return;
  }
  if (element.scale > 0) {
    while (element.scale--)
      tmpVal = tmpVal * 10;
  } else if (element.scale < 0) {
    while (-(element.scale++))
      tmpVal = tmpVal / 10;
  }
  // Number of decimals by accuracy (ACC_2 without decimals from 1000000 - as
  // before)
  u8 dec = 2;
  if (element.pConst->acc == ACC_3)
    dec = 3;
  else if (element.pConst->acc == ACC_4)
    dec = 4;
  bool fMega = false;
  if (element.pConst->fKilo)
    tmpVal = tmpVal / 1000; // k
  if ((element.pConst->acc == ACC_2) && (tmpVal >= 1000000))
    dec = 0;
  // LCD has 8 digits (one of them is minus for negative values)
  {
    double lim = (tmpVal < 0) ? 10000000.0 : 100000000.0;
    double a = (tmpVal < 0) ? -tmpVal : tmpVal;
    double scale = 1;
    for (u8 i = 0; i < dec; i++)
      scale *= 10;
    if ((a * scale) >= lim) { // does not fit -> Mega (kW->MW, A->MA, V->MV ...)
      double div = element.pConst->fKilo ? 1000.0 : 1000000.0;
      tmpVal = tmpVal / div;
      a = a / div;
      fMega = true;
    }
    while ((dec > 0) &&
           ((a * scale) >= lim)) { // still does not fit -> fewer decimals
      dec--;
      scale /= 10;
    }
  }
  if (fMega)
    lcd.text(SHOW, 1, M);
  else if (element.pConst->fKilo)
    lcd.text(SHOW, 1, k);
  lcd.show_float(tmpVal, false, false,
                 dec); // decimal point is set inside show_long
  switch (element.unit) {
  case UNIT_ACTIVE_POWER: {
    lcd.text(SHOW, 2, V, t);
  } break;
  case UNIT_APPARENT_POWER: {
    lcd.text(SHOW, 2, V, A);
  } break;
  case UNIT_REACTIVE_POWER: {
    lcd.text(SHOW, 3, V, A, r);
  } break;
  case UNIT_ACTIVE_ENERGY: {
    lcd.text(SHOW, 3, V, t, h);
  } break;
  case UNIT_REACTIVE_ENERGY: {
    lcd.text(SHOW, 4, V, A, r, h);
  } break;
  case UNIT_CURRENT: {
    lcd.text(SHOW, 1, A);
  } break;
  case UNIT_VOLTAGE: {
    lcd.text(SHOW, 1, V);
  } break;
  case UNIT_FREQUENCY: {
    lcd.text(SHOW, 1, Hz);
  } break;
  case UNIT_TEMPERATURE: {
    lcd.show_val_pos(GRADUS, 14);
    lcd.show_val_pos(CELSIUS, 15);
  } break;
  default:
    break;
  }
}
//----------------------------------------------------------------------------//