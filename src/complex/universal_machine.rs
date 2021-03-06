use crate::Buffer;

const MACHINE_TRANS_KEYS: &[u8] = &[
    12, 48, 1, 15, 1, 1, 12, 48, 1, 1, 0, 48, 21, 21, 11, 48,
    11, 48, 1, 15, 1, 1, 11, 48, 22, 48, 23, 48, 24, 47, 25, 47,
    26, 47, 45, 46, 46, 46, 24, 48, 24, 48, 24, 48, 1, 1, 24, 48,
    23, 48, 23, 48, 23, 48, 22, 48, 22, 48, 22, 48, 22, 48, 11, 48,
    1, 48, 11, 48, 13, 21, 4, 4, 13, 13, 11, 48, 11, 48, 41, 42,
    42, 42, 11, 48, 11, 48, 22, 48, 23, 48, 24, 47, 25, 47, 26, 47,
    45, 46, 46, 46, 24, 48, 24, 48, 24, 48, 24, 48, 23, 48, 23, 48,
    23, 48, 22, 48, 22, 48, 22, 48, 22, 48, 11, 48, 1, 48, 1, 15,
    4, 4, 13, 21, 13, 13, 12, 48, 1, 48, 11, 48, 41, 42, 42, 42,
    21, 42, 1, 5, 0
];

const MACHINE_KEY_SPANS: &[u8] = &[
    37, 15, 1, 37, 1, 49, 1, 38,
    38, 15, 1, 38, 27, 26, 24, 23,
    22, 2, 1, 25, 25, 25, 1, 25,
    26, 26, 26, 27, 27, 27, 27, 38,
    48, 38, 9, 1, 1, 38, 38, 2,
    1, 38, 38, 27, 26, 24, 23, 22,
    2, 1, 25, 25, 25, 25, 26, 26,
    26, 27, 27, 27, 27, 38, 48, 15,
    1, 9, 1, 37, 48, 38, 2, 1,
    22, 5
];

const MACHINE_INDEX_OFFSETS: &[u16] = &[
    0, 38, 54, 56, 94, 96, 146, 148,
    187, 226, 242, 244, 283, 311, 338, 363,
    387, 410, 413, 415, 441, 467, 493, 495,
    521, 548, 575, 602, 630, 658, 686, 714,
    753, 802, 841, 851, 853, 855, 894, 933,
    936, 938, 977, 1016, 1044, 1071, 1096, 1120,
    1143, 1146, 1148, 1174, 1200, 1226, 1252, 1279,
    1306, 1333, 1361, 1389, 1417, 1445, 1484, 1533,
    1549, 1551, 1561, 1563, 1601, 1650, 1689, 1692,
    1694, 1717
];

const MACHINE_INDICIES: &[u8] = &[
    1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 1, 0, 3, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 4, 2, 3, 2,
    6, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 6, 5, 7, 5,
    8, 9, 10, 8, 11, 12, 10, 10,
    10, 10, 10, 3, 13, 14, 10, 15,
    8, 8, 16, 17, 10, 10, 18, 19,
    20, 21, 22, 23, 24, 18, 25, 26,
    27, 28, 29, 30, 10, 31, 32, 33,
    10, 34, 35, 36, 37, 38, 39, 40,
    13, 10, 42, 41, 44, 1, 43, 43,
    45, 43, 43, 43, 43, 43, 46, 47,
    48, 49, 50, 51, 52, 53, 47, 54,
    46, 55, 56, 57, 58, 43, 59, 60,
    61, 43, 43, 43, 43, 62, 63, 64,
    65, 1, 43, 44, 1, 43, 43, 45,
    43, 43, 43, 43, 43, 66, 47, 48,
    49, 50, 51, 52, 53, 47, 54, 55,
    55, 56, 57, 58, 43, 59, 60, 61,
    43, 43, 43, 43, 62, 63, 64, 65,
    1, 43, 44, 67, 67, 67, 67, 67,
    67, 67, 67, 67, 67, 67, 67, 67,
    68, 67, 44, 67, 44, 1, 43, 43,
    45, 43, 43, 43, 43, 43, 43, 47,
    48, 49, 50, 51, 52, 53, 47, 54,
    55, 55, 56, 57, 58, 43, 59, 60,
    61, 43, 43, 43, 43, 62, 63, 64,
    65, 1, 43, 47, 48, 49, 50, 51,
    43, 43, 43, 43, 43, 43, 56, 57,
    58, 43, 59, 60, 61, 43, 43, 43,
    43, 48, 63, 64, 65, 69, 43, 48,
    49, 50, 51, 43, 43, 43, 43, 43,
    43, 43, 43, 43, 43, 59, 60, 61,
    43, 43, 43, 43, 43, 63, 64, 65,
    69, 43, 49, 50, 51, 43, 43, 43,
    43, 43, 43, 43, 43, 43, 43, 43,
    43, 43, 43, 43, 43, 43, 43, 63,
    64, 65, 43, 50, 51, 43, 43, 43,
    43, 43, 43, 43, 43, 43, 43, 43,
    43, 43, 43, 43, 43, 43, 43, 63,
    64, 65, 43, 51, 43, 43, 43, 43,
    43, 43, 43, 43, 43, 43, 43, 43,
    43, 43, 43, 43, 43, 43, 63, 64,
    65, 43, 63, 64, 43, 64, 43, 49,
    50, 51, 43, 43, 43, 43, 43, 43,
    43, 43, 43, 43, 59, 60, 61, 43,
    43, 43, 43, 43, 63, 64, 65, 69,
    43, 49, 50, 51, 43, 43, 43, 43,
    43, 43, 43, 43, 43, 43, 43, 60,
    61, 43, 43, 43, 43, 43, 63, 64,
    65, 69, 43, 49, 50, 51, 43, 43,
    43, 43, 43, 43, 43, 43, 43, 43,
    43, 43, 61, 43, 43, 43, 43, 43,
    63, 64, 65, 69, 43, 71, 70, 49,
    50, 51, 43, 43, 43, 43, 43, 43,
    43, 43, 43, 43, 43, 43, 43, 43,
    43, 43, 43, 43, 63, 64, 65, 69,
    43, 48, 49, 50, 51, 43, 43, 43,
    43, 43, 43, 56, 57, 58, 43, 59,
    60, 61, 43, 43, 43, 43, 48, 63,
    64, 65, 69, 43, 48, 49, 50, 51,
    43, 43, 43, 43, 43, 43, 43, 57,
    58, 43, 59, 60, 61, 43, 43, 43,
    43, 48, 63, 64, 65, 69, 43, 48,
    49, 50, 51, 43, 43, 43, 43, 43,
    43, 43, 43, 58, 43, 59, 60, 61,
    43, 43, 43, 43, 48, 63, 64, 65,
    69, 43, 47, 48, 49, 50, 51, 43,
    53, 47, 43, 43, 43, 56, 57, 58,
    43, 59, 60, 61, 43, 43, 43, 43,
    48, 63, 64, 65, 69, 43, 47, 48,
    49, 50, 51, 43, 72, 47, 43, 43,
    43, 56, 57, 58, 43, 59, 60, 61,
    43, 43, 43, 43, 48, 63, 64, 65,
    69, 43, 47, 48, 49, 50, 51, 43,
    43, 47, 43, 43, 43, 56, 57, 58,
    43, 59, 60, 61, 43, 43, 43, 43,
    48, 63, 64, 65, 69, 43, 47, 48,
    49, 50, 51, 52, 53, 47, 43, 43,
    43, 56, 57, 58, 43, 59, 60, 61,
    43, 43, 43, 43, 48, 63, 64, 65,
    69, 43, 44, 1, 43, 43, 45, 43,
    43, 43, 43, 43, 43, 47, 48, 49,
    50, 51, 52, 53, 47, 54, 43, 55,
    56, 57, 58, 43, 59, 60, 61, 43,
    43, 43, 43, 62, 63, 64, 65, 1,
    43, 44, 67, 67, 67, 67, 67, 67,
    67, 67, 67, 67, 67, 67, 67, 68,
    67, 67, 67, 67, 67, 67, 67, 48,
    49, 50, 51, 67, 67, 67, 67, 67,
    67, 67, 67, 67, 67, 59, 60, 61,
    67, 67, 67, 67, 67, 63, 64, 65,
    69, 67, 44, 1, 43, 43, 45, 43,
    43, 43, 43, 43, 43, 47, 48, 49,
    50, 51, 52, 53, 47, 54, 46, 55,
    56, 57, 58, 43, 59, 60, 61, 43,
    43, 43, 43, 62, 63, 64, 65, 1,
    43, 74, 73, 73, 73, 73, 73, 73,
    73, 75, 73, 11, 76, 74, 73, 44,
    1, 43, 43, 45, 43, 43, 43, 43,
    43, 77, 47, 48, 49, 50, 51, 52,
    53, 47, 54, 46, 55, 56, 57, 58,
    43, 59, 60, 61, 43, 78, 79, 43,
    62, 63, 64, 65, 1, 43, 44, 1,
    43, 43, 45, 43, 43, 43, 43, 43,
    43, 47, 48, 49, 50, 51, 52, 53,
    47, 54, 46, 55, 56, 57, 58, 43,
    59, 60, 61, 43, 78, 79, 43, 62,
    63, 64, 65, 1, 43, 78, 79, 80,
    79, 80, 3, 6, 81, 81, 82, 81,
    81, 81, 81, 81, 83, 18, 19, 20,
    21, 22, 23, 24, 18, 25, 27, 27,
    28, 29, 30, 81, 31, 32, 33, 81,
    81, 81, 81, 37, 38, 39, 40, 6,
    81, 3, 6, 81, 81, 82, 81, 81,
    81, 81, 81, 81, 18, 19, 20, 21,
    22, 23, 24, 18, 25, 27, 27, 28,
    29, 30, 81, 31, 32, 33, 81, 81,
    81, 81, 37, 38, 39, 40, 6, 81,
    18, 19, 20, 21, 22, 81, 81, 81,
    81, 81, 81, 28, 29, 30, 81, 31,
    32, 33, 81, 81, 81, 81, 19, 38,
    39, 40, 84, 81, 19, 20, 21, 22,
    81, 81, 81, 81, 81, 81, 81, 81,
    81, 81, 31, 32, 33, 81, 81, 81,
    81, 81, 38, 39, 40, 84, 81, 20,
    21, 22, 81, 81, 81, 81, 81, 81,
    81, 81, 81, 81, 81, 81, 81, 81,
    81, 81, 81, 81, 38, 39, 40, 81,
    21, 22, 81, 81, 81, 81, 81, 81,
    81, 81, 81, 81, 81, 81, 81, 81,
    81, 81, 81, 81, 38, 39, 40, 81,
    22, 81, 81, 81, 81, 81, 81, 81,
    81, 81, 81, 81, 81, 81, 81, 81,
    81, 81, 81, 38, 39, 40, 81, 38,
    39, 81, 39, 81, 20, 21, 22, 81,
    81, 81, 81, 81, 81, 81, 81, 81,
    81, 31, 32, 33, 81, 81, 81, 81,
    81, 38, 39, 40, 84, 81, 20, 21,
    22, 81, 81, 81, 81, 81, 81, 81,
    81, 81, 81, 81, 32, 33, 81, 81,
    81, 81, 81, 38, 39, 40, 84, 81,
    20, 21, 22, 81, 81, 81, 81, 81,
    81, 81, 81, 81, 81, 81, 81, 33,
    81, 81, 81, 81, 81, 38, 39, 40,
    84, 81, 20, 21, 22, 81, 81, 81,
    81, 81, 81, 81, 81, 81, 81, 81,
    81, 81, 81, 81, 81, 81, 81, 38,
    39, 40, 84, 81, 19, 20, 21, 22,
    81, 81, 81, 81, 81, 81, 28, 29,
    30, 81, 31, 32, 33, 81, 81, 81,
    81, 19, 38, 39, 40, 84, 81, 19,
    20, 21, 22, 81, 81, 81, 81, 81,
    81, 81, 29, 30, 81, 31, 32, 33,
    81, 81, 81, 81, 19, 38, 39, 40,
    84, 81, 19, 20, 21, 22, 81, 81,
    81, 81, 81, 81, 81, 81, 30, 81,
    31, 32, 33, 81, 81, 81, 81, 19,
    38, 39, 40, 84, 81, 18, 19, 20,
    21, 22, 81, 24, 18, 81, 81, 81,
    28, 29, 30, 81, 31, 32, 33, 81,
    81, 81, 81, 19, 38, 39, 40, 84,
    81, 18, 19, 20, 21, 22, 81, 85,
    18, 81, 81, 81, 28, 29, 30, 81,
    31, 32, 33, 81, 81, 81, 81, 19,
    38, 39, 40, 84, 81, 18, 19, 20,
    21, 22, 81, 81, 18, 81, 81, 81,
    28, 29, 30, 81, 31, 32, 33, 81,
    81, 81, 81, 19, 38, 39, 40, 84,
    81, 18, 19, 20, 21, 22, 23, 24,
    18, 81, 81, 81, 28, 29, 30, 81,
    31, 32, 33, 81, 81, 81, 81, 19,
    38, 39, 40, 84, 81, 3, 6, 81,
    81, 82, 81, 81, 81, 81, 81, 81,
    18, 19, 20, 21, 22, 23, 24, 18,
    25, 81, 27, 28, 29, 30, 81, 31,
    32, 33, 81, 81, 81, 81, 37, 38,
    39, 40, 6, 81, 3, 81, 81, 81,
    81, 81, 81, 81, 81, 81, 81, 81,
    81, 81, 4, 81, 81, 81, 81, 81,
    81, 81, 19, 20, 21, 22, 81, 81,
    81, 81, 81, 81, 81, 81, 81, 81,
    31, 32, 33, 81, 81, 81, 81, 81,
    38, 39, 40, 84, 81, 3, 86, 86,
    86, 86, 86, 86, 86, 86, 86, 86,
    86, 86, 86, 4, 86, 87, 81, 14,
    81, 81, 81, 81, 81, 81, 81, 88,
    81, 14, 81, 6, 86, 86, 86, 86,
    86, 86, 86, 86, 86, 86, 86, 86,
    86, 86, 86, 86, 86, 86, 86, 86,
    86, 86, 86, 86, 86, 86, 86, 86,
    86, 86, 86, 6, 86, 86, 86, 6,
    86, 9, 81, 81, 81, 9, 81, 81,
    81, 81, 81, 3, 6, 14, 81, 82,
    81, 81, 81, 81, 81, 81, 18, 19,
    20, 21, 22, 23, 24, 18, 25, 26,
    27, 28, 29, 30, 81, 31, 32, 33,
    81, 34, 35, 81, 37, 38, 39, 40,
    6, 81, 3, 6, 81, 81, 82, 81,
    81, 81, 81, 81, 81, 18, 19, 20,
    21, 22, 23, 24, 18, 25, 26, 27,
    28, 29, 30, 81, 31, 32, 33, 81,
    81, 81, 81, 37, 38, 39, 40, 6,
    81, 34, 35, 81, 35, 81, 78, 80,
    80, 80, 80, 80, 80, 80, 80, 80,
    80, 80, 80, 80, 80, 80, 80, 80,
    80, 80, 78, 79, 80, 9, 86, 86,
    86, 9, 86, 0
];

const MACHINE_TRANS_TARGS: &[u8] = &[
    5, 9, 5, 41, 2, 5, 1, 53,
    6, 7, 5, 34, 37, 63, 64, 67,
    68, 72, 43, 44, 45, 46, 47, 57,
    58, 60, 69, 61, 54, 55, 56, 50,
    51, 52, 70, 71, 73, 62, 48, 49,
    5, 5, 5, 5, 8, 0, 33, 12,
    13, 14, 15, 16, 27, 28, 30, 31,
    24, 25, 26, 19, 20, 21, 32, 17,
    18, 5, 11, 5, 10, 22, 5, 23,
    29, 5, 35, 36, 5, 38, 39, 40,
    5, 5, 3, 42, 4, 59, 5, 65,
    66
];

const MACHINE_TRANS_ACTIONS: &[u8] = &[
    1, 0, 2, 3, 0, 4, 0, 5,
    0, 5, 8, 0, 5, 9, 0, 9,
    3, 0, 5, 5, 0, 0, 0, 5,
    5, 5, 3, 3, 5, 5, 5, 5,
    5, 5, 0, 0, 0, 3, 0, 0,
    10, 11, 12, 13, 5, 0, 5, 0,
    0, 0, 0, 0, 0, 0, 0, 5,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 14, 5, 15, 0, 0, 16, 0,
    0, 17, 0, 0, 18, 5, 0, 0,
    19, 20, 0, 3, 0, 5, 21, 0,
    0
];

const MACHINE_TO_STATE_ACTIONS: &[u8] = &[
    0, 0, 0, 0, 0, 6, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0
];

const MACHINE_FROM_STATE_ACTIONS: &[u8] = &[
    0, 0, 0, 0, 0, 7, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0
];

const MACHINE_EOF_TRANS: &[u8] = &[
    1, 3, 3, 6, 6, 0, 42, 44,
    44, 68, 68, 44, 44, 44, 44, 44,
    44, 44, 44, 44, 44, 44, 71, 44,
    44, 44, 44, 44, 44, 44, 44, 44,
    68, 44, 74, 77, 74, 44, 44, 81,
    81, 82, 82, 82, 82, 82, 82, 82,
    82, 82, 82, 82, 82, 82, 82, 82,
    82, 82, 82, 82, 82, 82, 82, 87,
    82, 82, 82, 87, 82, 82, 82, 82,
    81, 87
];

#[derive(Clone, Copy)]
pub enum SyllableType {
    IndependentCluster,
    ViramaTerminatedCluster,
    SakotTerminatedCluster,
    StandardCluster,
    NumberJoinerTerminatedCluster,
    NumeralCluster,
    SymbolCluster,
    BrokenCluster,
    NonCluster,
}

pub fn find_syllables(buffer: &mut Buffer) {
    let mut cs = 5usize;
    let mut ts = 0;
    let mut te = 0;
    let mut act = 0;
    let mut p = 0;
    let pe = buffer.len();
    let eof = buffer.len();
    let mut syllable_serial = 1u8;
    let mut reset = true;
    let mut slen;
    let mut trans = 0;
    if p == pe {
        if MACHINE_EOF_TRANS[cs] > 0 {
            trans = (MACHINE_EOF_TRANS[cs] - 1) as usize;
        }
    }

    loop {
        if reset {
            if MACHINE_FROM_STATE_ACTIONS[cs] == 7 {
                ts = p;
            }

            slen = MACHINE_KEY_SPANS[cs] as usize;
            let cs_idx = ((cs as i32) << 1) as usize;
            let i = if slen > 0 &&
                MACHINE_TRANS_KEYS[cs_idx] <= buffer.info[p].indic_category() as u8 &&
                buffer.info[p].indic_category() as u8 <= MACHINE_TRANS_KEYS[cs_idx + 1]
            {
                (buffer.info[p].indic_category() as u8 - MACHINE_TRANS_KEYS[cs_idx]) as usize
            } else {
                slen
            };
            trans = MACHINE_INDICIES[MACHINE_INDEX_OFFSETS[cs] as usize + i] as usize;
        }
        reset = true;

        cs = MACHINE_TRANS_TARGS[trans] as usize;

        if MACHINE_TRANS_ACTIONS[trans] != 0 {
            match MACHINE_TRANS_ACTIONS[trans] {
                5 => {
                    te = p + 1;
                }
                12 => {
                    te = p + 1;
                    found_syllable(ts, te, &mut syllable_serial, SyllableType::IndependentCluster, buffer);
                }
                14 => {
                    te = p + 1;
                    found_syllable(ts, te, &mut syllable_serial, SyllableType::StandardCluster, buffer);
                }
                10 => {
                    te = p + 1;
                    found_syllable(ts, te, &mut syllable_serial, SyllableType::BrokenCluster, buffer);
                }
                8 => {
                    te = p + 1;
                    found_syllable(ts, te, &mut syllable_serial, SyllableType::NonCluster, buffer);
                }
                11 => {
                    te = p;
                    p -= 1;
                    found_syllable(ts, te, &mut syllable_serial, SyllableType::IndependentCluster, buffer);
                }
                15 => {
                    te = p;
                    p -= 1;
                    found_syllable(ts, te, &mut syllable_serial, SyllableType::ViramaTerminatedCluster, buffer);
                }
                16 => {
                    te = p;
                    p -= 1;
                    found_syllable(ts, te, &mut syllable_serial, SyllableType::SakotTerminatedCluster, buffer);
                }
                13 => {
                    te = p;
                    p -= 1;
                    found_syllable(ts, te, &mut syllable_serial, SyllableType::StandardCluster, buffer);
                }
                18 => {
                    te = p;
                    p -= 1;
                    found_syllable(ts, te, &mut syllable_serial, SyllableType::NumberJoinerTerminatedCluster, buffer);
                }
                17 => {
                    te = p;
                    p -= 1;
                    found_syllable(ts, te, &mut syllable_serial, SyllableType::NumeralCluster, buffer);
                }
                19 => {
                    te = p;
                    p -= 1;
                    found_syllable(ts, te, &mut syllable_serial, SyllableType::SymbolCluster, buffer);
                }
                20 => {
                    te = p;
                    p -= 1;
                    found_syllable(ts, te, &mut syllable_serial, SyllableType::BrokenCluster, buffer);
                }
                1 => {
                    p = te - 1;
                    found_syllable(ts, te, &mut syllable_serial, SyllableType::StandardCluster, buffer);
                }
                2 => {
                    match act {
                        8 => {
                            p = te - 1;
                            found_syllable(ts, te, &mut syllable_serial, SyllableType::BrokenCluster, buffer);
                        }
                        9 => {
                            p = te - 1;
                            found_syllable(ts, te, &mut syllable_serial, SyllableType::NonCluster, buffer);
                        }
                        _ => {}
                    }
                }
                3 => {
                    te = p + 1;
                    act = 8;
                }
                9 => {
                    te = p + 1;
                    act = 9;
                }
                _ => {}
            }
        }

        if MACHINE_TO_STATE_ACTIONS[cs] == 6 {
            ts = 0;
        }

        p += 1;
        if p != pe {
            continue;
        }

        if p == eof {
            if MACHINE_EOF_TRANS[cs] > 0 {
                trans = (MACHINE_EOF_TRANS[cs] - 1) as usize;
                reset = false;
                continue;
            }
        }

        break;
    }
}

#[inline]
fn found_syllable(
    start: usize,
    end: usize,
    syllable_serial: &mut u8,
    kind: SyllableType,
    buffer: &mut Buffer,
) {
    for i in start..end {
        buffer.info[i].set_syllable((*syllable_serial << 4) | kind as u8);
    }

    *syllable_serial += 1;

    if *syllable_serial == 16 {
        *syllable_serial = 1;
    }
}
