// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2026, Stevie Galluscio

#pragma once

// Alpakka v1 CTRL app pins used for compatibility with Alpakka Lite.
typedef enum AlpakkaV1CtrlPin_enum {
    ALPAKKA_V1_CTRL_PIN_NONE       = 0,
    ALPAKKA_V1_CTRL_PIN_HEXAGON    = 12,
    ALPAKKA_V1_CTRL_PIN_DPAD_DOWN  = 100,
    ALPAKKA_V1_CTRL_PIN_DPAD_RIGHT = 101,
    ALPAKKA_V1_CTRL_PIN_L1         = 102,
    ALPAKKA_V1_CTRL_PIN_DPAD_UP    = 103,
    ALPAKKA_V1_CTRL_PIN_DPAD_LEFT  = 104,
    ALPAKKA_V1_CTRL_PIN_L4         = 108,
    ALPAKKA_V1_CTRL_PIN_L3         = 109,
    ALPAKKA_V1_CTRL_PIN_SELECT_2   = 110,
    ALPAKKA_V1_CTRL_PIN_SELECT_1   = 114,
    ALPAKKA_V1_CTRL_PIN_L2         = 115,
    ALPAKKA_V1_CTRL_PIN_START_1    = 200,
    ALPAKKA_V1_CTRL_PIN_START_2    = 201,
    ALPAKKA_V1_CTRL_PIN_R3         = 202,
    ALPAKKA_V1_CTRL_PIN_R4         = 207,
    ALPAKKA_V1_CTRL_PIN_B          = 210,
    ALPAKKA_V1_CTRL_PIN_Y          = 211,
    ALPAKKA_V1_CTRL_PIN_R1         = 212,
    ALPAKKA_V1_CTRL_PIN_X          = 213,
    ALPAKKA_V1_CTRL_PIN_R2         = 214,
    ALPAKKA_V1_CTRL_PIN_A          = 215,
} AlpakkaV1CtrlPin;
