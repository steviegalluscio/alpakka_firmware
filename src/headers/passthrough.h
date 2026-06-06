// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2026, Stevie Galluscio

#pragma once

#define TRIGGER_THRESHOLD 30

#pragma pack(push, 1)
typedef struct {
    union {
        struct {
            // Standard XInput bits (wButtons)
            uint16_t dpad_up:1, dpad_down:1, dpad_left:1, dpad_right:1,
                start:1, back:1,
                l3:1, r3:1,
                l1:1, r1:1,
                guide:1, share:1,
                a:1, b:1, x:1, y:1;
            // Vendor specific bits (bVendorButtons)
            uint8_t mode:1, capture:1,
                paddle_l:1, paddle_r:1,
                reserved:4;
            // Calculated bits
            uint8_t l2:1, r2:1;
        } bits;
        struct {
            uint16_t standard_btns;
            uint8_t  vendor_btns;
        } raw;
    };
    int16_t thumbstick_lx;
    int16_t thumbstick_ly;
    int16_t thumbstick_rx;
    int16_t thumbstick_ry;
} passthrough_input_t;
#pragma pack(pop)

void passthrough_start();
void passthrough_report();
bool passthrough_get_input(passthrough_input_t *output);
void passthrough_pause_start();
void passthrough_pause_end();
