#ifndef _AI4NATIONWAR_H_
#define _AI4NATIONWAR_H_

namespace AI4 {

typedef float PROB;

enum GAME_STATE {
    GS_PREPARE = -1,
    GS_SOUTH = 0,
    GS_EAST = 1,
    GS_NORTH = 2,
    GS_WEST = 3,
    GS_END = 5,
};

enum PLAYER_ACTION {
    AC_SURRENDER = -1,
    AC_MOVE = 1,
    AC_SKIP = 2,
};

enum ACTION_RESULT {
    AR_ILLEGAL = -1,
    AR_MOVED = 0,
    AR_CAPTURE = 1,
    AR_LOST = 2,
    AR_FIRE = 3,    
    AR_LING_BOMP = 4,   
    AR_LING_LING = 5,   
    AR_LING_MINE = 6,
    AR_BOMP_LING = 7,
    AR_FLAG = 8,         // get the flag
};

enum PLAYER_STATUS {
    PS_OFFLINE = 0,
    PS_ONLINE = 1,
};

enum PLAYER_TYPE {
    PT_AI = 0,
    PT_MAN = 1,
};

enum PIECE_COLOR {
    PC_SOUTH = 0,
    PC_EAST = 1,
    PC_NORTH = 2,
    PC_WEST = 3
};

enum PIECE_ROLE {
    PR_UNKNOW = 13,

    PR_FLAG = 11,
    PR_MINE = 10,
    PR_BOMB = 9,
    PR_LING = 8,
    PR_JUN = 7,
    PR_SHI = 6,
    PR_LV = 5,
    PR_TUAN = 4,
    PR_YING = 3,
    PR_LIAN = 2,
    PR_PAI = 1,
    PR_MINER = 0,
};

enum POS_TYPE {
    POS_RAIL_MASK = 0x01,
    POS_CAMP_MASK = 0x02,
    POS_NORMAL_MASK = 0x04,
    POS_BASE_MASK = 0x08,
    POS_DOWN_MASK = 0x10,
    POS_CENTER_MASK = 0x20,
    POS_OTHERS = 0x40,
    
    POS_RAIL_LD = 0x0101,
    POS_RAIL_L3 = 0x0201,
    POS_RAIL_L2 = 0x0301,
    POS_RAIL_L1 = 0x0401,
    POS_RAIL_LU = 0x0501,
    POS_RAIL_U1 = 0x0601,
    POS_RAIL_U2 = 0x0701,
    POS_RAIL_U3 = 0x0801,
    POS_RAIL_RU = 0x0901,
    POS_RAIL_R1 = 0x0A01,
    POS_RAIL_R2 = 0x0B01,
    POS_RAIL_R3 = 0x0C01,
    POS_RAIL_RD = 0x0D01,
    POS_RAIL_D3 = 0x0E01,
    POS_RAIL_D2 = 0x0F01,
    POS_RAIL_D1 = 0x1001,

    POS_CAMP_LD = 0x0102,
    POS_CAMP_LU = 0x0202,
    POS_CAMP_RU = 0x0302,
    POS_CAMP_RD = 0x0402,
    POS_CAMP_CENTER = 0x0502,

    POS_NORMAL_LEFT = 0x0104,
    POS_NORMAL_UP = 0x0204,
    POS_NORMAL_RIGHT = 0x0304,
    POS_NORMAL_DOWN = 0x0404,

    POS_DOWN_LEFT = 0x0110,
    POS_DOWN_CENTER = 0x0210,
    POS_DOWN_RIGHT = 0x0310,
    POS_BASE_LEFT = 0x0108,
    POS_BASE_RIGHT = 0x0208,

    POS_CENTER_00 = 0x0120,
    POS_CENTER_10 = 0x0220,
    POS_CENTER_11 = 0x0320,
    POS_CENTER_12 = 0x0420,
    POS_CENTER_13 = 0x0520,
    POS_CENTER_14 = 0x0620,
    POS_CENTER_15 = 0x0720,
    POS_CENTER_16 = 0x0820,
    POS_CENTER_17 = 0x0920,
};

const POS_TYPE POSTABLE[6][5] = {
    { POS_RAIL_LU,      POS_RAIL_U1,        POS_RAIL_U2,        POS_RAIL_U3,        POS_RAIL_RU    },
    { POS_RAIL_L1,      POS_CAMP_LU,        POS_NORMAL_UP,      POS_CAMP_RU,        POS_RAIL_R1    },
    { POS_RAIL_L2,      POS_NORMAL_LEFT,    POS_CAMP_CENTER,    POS_NORMAL_RIGHT,   POS_RAIL_R2    },
    { POS_RAIL_L3,      POS_CAMP_LD,        POS_NORMAL_DOWN,    POS_CAMP_RD,        POS_RAIL_R3    },
    { POS_RAIL_LD,      POS_RAIL_D1,        POS_RAIL_D2,        POS_RAIL_D3,        POS_RAIL_RD    },
    { POS_DOWN_LEFT,    POS_BASE_LEFT,      POS_DOWN_CENTER,    POS_BASE_RIGHT,     POS_DOWN_RIGHT },
};


/**************************
    6     5      4
    7            3 
    0     1      2
**************************/
const int L1_TABLE[] = {
    0, -1, 1, -1, 2,
    2, -1, 3, -1, 4,
    4, -1, 5, -1, 6,
    6, -1, 7, -1, 0,
};

const float VALUE_TABLE[] = {
    15,         // PR_MINER
    1,          // PR_PAI
    2,          // PR_LIAN
    4,          // PR_YING
    6,          // PR_TUAN
    20,         // PR_LV
    30,         // PR_SHI
    40,         // PR_JUN
    100,        // PR_LING
    30,         // PR_BOMP
    10,         // PR_MINE
    200,        // PR_FLAG
};

enum ESTIMATE_PIECE_ROLE {
    EPR_MINER1 = 0x00000001,
    EPR_MINER2 = 0x00000002,
    EPR_MINER3 = 0x00000004,
    EPR_MINERS = 0x00000007,
    EPR_PAI1   = 0x00000008,
    EPR_PAI2   = 0x00000010,
    EPR_PAI3   = 0x00000020,
    EPR_PAIS   = 0x00000038,
    EPR_LIAN1  = 0x00000040,
    EPR_LIAN2  = 0x00000080,
    EPR_LIAN3  = 0x00000100,
    EPR_LIANS  = 0x000001C0,
    EPR_YING1  = 0x00000200,
    EPR_YING2  = 0x00000400,
    EPR_YINGS  = 0x00000600,
    EPR_TUAN1  = 0x00000800,
    EPR_TUAN2  = 0x00001000,
    EPR_TUANS  = 0x00001800,
    EPR_LV1    = 0x00002000,
    EPR_LV2    = 0x00004000,
    EPR_LVS    = 0x00006000,
    EPR_SHI1   = 0x00008000,
    EPR_SHI2   = 0x00010000,
    EPR_SHIS   = 0x00018000,
    EPR_JUN    = 0x00020000,
    EPR_LING   = 0x00040000,
    EPR_BOMB1  = 0x00080000,
    EPR_BOMB2  = 0x00100000,
    EPR_BOMBS  = 0x00180000,
    EPR_MINE1  = 0x00200000,
    EPR_MINE2  = 0x00400000,
    EPR_MINE3  = 0x00800000,
    EPR_MINES  = 0x00E00000,
    EPR_FLAG   = 0x01000000,

    EPR_NORMAL          = 0x0007FFFF,
    EPR_FIRST_ROW       = 0x0007FFFF,
    EPR_MOVEABLE        = 0x001FFFFF,
    EPR_LAST_TWO_ROWS   = 0x00FFFFFF,  
    EPR_ALL             = 0x01FFFFFF,
};


const unsigned int SEQ_TABLE[] = {
    EPR_MINERS,
    EPR_PAIS,
    EPR_LIANS,
    EPR_YINGS,
    EPR_TUANS,
    EPR_LVS,
    EPR_SHIS,
    EPR_JUN,
    EPR_LING,
    EPR_BOMBS,
    EPR_MINES,
    EPR_FLAG 
};

const PIECE_ROLE SEQ_TABLE2[] = {
    PR_MINER,
    PR_MINER,
    PR_MINER,
    PR_PAI,
    PR_PAI,
    PR_PAI,
    PR_LIAN,
    PR_LIAN,
    PR_LIAN,
    PR_YING,
    PR_YING,
    PR_TUAN,
    PR_TUAN,
    PR_LV,
    PR_LV,
    PR_SHI,
    PR_SHI,
    PR_JUN,
    PR_LING,
    PR_BOMB,
    PR_BOMB,
    PR_MINE,
    PR_MINE,
    PR_MINE,
    PR_FLAG,
};

}
#endif
