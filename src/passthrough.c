// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2026, Stevie Galluscio

#include <stdint.h>
#include <stdbool.h>
#include <pico/multicore.h>
#include <pio_usb.h>
#include <tusb.h>
#include "passthrough.h"
#include "lite_compat.h"
#include "pin.h"
#include "xinput_host.h"
#include "profile.h"
#include "logging.h"

auto_init_mutex(passthrough_mutex);
static passthrough_input_t gamepad;
static volatile bool host_initialized = false;
static volatile bool pause_requested = false;
static volatile bool pause_request_ack = false;
static uint32_t passthrough_core1_stack[2048 / sizeof(uint32_t)] __attribute__((aligned(8)));


void __no_inline_not_in_flash_func(passthrough_pause_start)() {
    if (!host_initialized) return;
    pause_requested = true;
    while (!pause_request_ack) {
        tight_loop_contents();
    }
}

void __no_inline_not_in_flash_func(passthrough_pause_end)() {
    if (!host_initialized) return;
    pause_requested = false;
    while (pause_request_ack) {
        tight_loop_contents();
    }
}

static void __no_inline_not_in_flash_func(passthrough_pause_loop)() {
    uint32_t interrupts = save_and_disable_interrupts();
    uint32_t next_frame = time_us_32();
    pause_request_ack = true;
    while (pause_requested) {
        uint32_t now = time_us_32();
        if ((int32_t)(now - next_frame) >= 0) {
            pio_usb_host_frame_keepalive();
            next_frame += 1000;
        }
        tight_loop_contents();
    }
    pause_request_ack = false;
    restore_interrupts(interrupts);
}

void __no_inline_not_in_flash_func(passthrough_core1)() {
    sleep_ms(50);
    info("INIT: USB host passthrough\n");
    gamepad = (passthrough_input_t){0};
    pio_usb_configuration_t pio_cfg = PIO_USB_DEFAULT_CONFIG;
    pio_cfg.pin_dp = PIN_USB_DP;
    pio_cfg.pinout = PIN_USB_PINOUT;
    pio_cfg.alarm_pool = alarm_pool_create_with_unused_hardware_alarm(1);
    tuh_configure(1, TUH_CFGID_RPI_PIO_USB_CONFIGURATION, &pio_cfg);
    info("  tuh_init start\n");
    if (!tuh_init(1)) {
        error("  tuh_init failed\n");
        while (true) tight_loop_contents();
    }
    host_initialized = true;
    info("  tuh_init done\n");
    while (true) {
        if (pause_requested) {
            passthrough_pause_loop();
            info("PASSTHROUGH: resume\n");
        }
        tuh_task();
    }
}

void passthrough_start() {
    multicore_launch_core1_with_stack(
        passthrough_core1,
        passthrough_core1_stack,
        sizeof(passthrough_core1_stack)
    );
}

bool passthrough_get_input(passthrough_input_t *output) {
    if(mutex_try_enter(&passthrough_mutex, NULL)){
        *output = gamepad;
        mutex_exit(&passthrough_mutex);
        return true;
    }
    return false;
}

static bool passthrough_gyro_engage_pressed(uint8_t engage, passthrough_input_t *input) {
    switch (engage) {
        case ALPAKKA_V1_CTRL_PIN_SELECT_1:   return input->bits.back;
        case ALPAKKA_V1_CTRL_PIN_SELECT_2:   return input->bits.capture || input->bits.share;
        case ALPAKKA_V1_CTRL_PIN_START_1:    return input->bits.start;
        case ALPAKKA_V1_CTRL_PIN_START_2:    return input->bits.mode;
        case ALPAKKA_V1_CTRL_PIN_DPAD_DOWN:  return input->bits.dpad_down;
        case ALPAKKA_V1_CTRL_PIN_DPAD_RIGHT: return input->bits.dpad_right;
        case ALPAKKA_V1_CTRL_PIN_DPAD_LEFT:  return input->bits.dpad_left;
        case ALPAKKA_V1_CTRL_PIN_DPAD_UP:    return input->bits.dpad_up;
        case ALPAKKA_V1_CTRL_PIN_A:          return input->bits.a;
        case ALPAKKA_V1_CTRL_PIN_B:          return input->bits.b;
        case ALPAKKA_V1_CTRL_PIN_X:          return input->bits.x;
        case ALPAKKA_V1_CTRL_PIN_Y:          return input->bits.y;
        case ALPAKKA_V1_CTRL_PIN_L1:         return input->bits.l1;
        case ALPAKKA_V1_CTRL_PIN_L2:         return input->bits.l2;
        case ALPAKKA_V1_CTRL_PIN_L3:         return input->bits.l3;
        case ALPAKKA_V1_CTRL_PIN_L4:         return input->bits.paddle_l;
        case ALPAKKA_V1_CTRL_PIN_R1:         return input->bits.r1;
        case ALPAKKA_V1_CTRL_PIN_R2:         return input->bits.r2;
        case ALPAKKA_V1_CTRL_PIN_R4:         return input->bits.paddle_r;
        case ALPAKKA_V1_CTRL_PIN_R3:         return input->bits.r3;
        default:                             return false;
    }
}

// core0
void passthrough_report() {
    Profile *profile = profile_get_active(false);
    passthrough_input_t input;
    if (passthrough_get_input(&input)) {
        if(input.bits.guide) profile_set_home_virtual_press();
        profile->select_1.virtual_press = input.bits.back;
        profile->select_2.virtual_press = input.bits.capture || input.bits.share;
        profile->start_1.virtual_press = input.bits.start;
        profile->start_2.virtual_press = input.bits.mode; // TEGENARIA only
        profile->dpad_down.virtual_press = input.bits.dpad_down;
        profile->dpad_right.virtual_press = input.bits.dpad_right;
        profile->dpad_left.virtual_press = input.bits.dpad_left;
        profile->dpad_up.virtual_press = input.bits.dpad_up;
        profile->a.virtual_press = input.bits.a;
        profile->b.virtual_press = input.bits.b;
        profile->x.virtual_press = input.bits.x;
        profile->y.virtual_press = input.bits.y;
        profile->l1.virtual_press = input.bits.l1;
        profile->l2.virtual_press = input.bits.l2;
        profile->l3.virtual_press = input.bits.l3; // We use thumbstick.push for this
        profile->l4.virtual_press = input.bits.paddle_l;
        profile->r1.virtual_press = input.bits.r1;
        profile->r2.virtual_press = input.bits.r2;
        profile->r4.virtual_press = input.bits.paddle_r;

        profile->left_thumbstick.push.virtual_press = input.bits.l3;
        profile->left_thumbstick.virtual_x  = input.thumbstick_lx / 32767.0f;
        profile->left_thumbstick.virtual_y  = input.thumbstick_ly / 32767.0f;
        profile->left_thumbstick.saturation = 1.0f;
        profile->left_thumbstick.invert_x = false;
        profile->left_thumbstick.invert_y = true;

        profile->right_thumbstick.push.virtual_press = input.bits.r3;
        profile->right_thumbstick.virtual_x  = input.thumbstick_rx / 32767.0f;
        profile->right_thumbstick.virtual_y  = input.thumbstick_ry / 32767.0f;
        profile->right_thumbstick.saturation = 1.0f;
        profile->right_thumbstick.invert_x = false;
        profile->right_thumbstick.invert_y = true;

        if (profile->gyro.engage != PIN_NONE && profile->gyro.engage != PIN_TOUCH_IN) {
            profile->gyro.engage_button.virtual_press =
                passthrough_gyro_engage_pressed(profile->gyro.engage, &input);
        }
        /*
        if (input.thumbstick_lx) info("Thumbstick LX: %d\n", input.thumbstick_lx);
        if (input.thumbstick_ly) info("Thumbstick LY: %d\n", input.thumbstick_ly);
        if (input.thumbstick_rx) info("Thumbstick RX: %d\n", input.thumbstick_rx);
        if (input.thumbstick_ry) info("Thumbstick RY: %d\n", input.thumbstick_ry);
        if (input.bits.dpad_up) info("DPAD_UP\n");
        if (input.bits.dpad_down) info("DPAD_DOWN\n");
        if (input.bits.dpad_left) info("DPAD_LEFT\n");
        if (input.bits.dpad_right) info("DPAD_RIGHT\n");
        if (input.bits.start) info("START\n");
        if (input.bits.back) info("BACK\n");
        if (input.bits.l3) info("LEFT_THUMB\n");
        if (input.bits.r3) info("RIGHT_THUMB\n");
        if (input.bits.l1) info("LEFT_SHOULDER\n");
        if (input.bits.r1) info("RIGHT_SHOULDER\n");
        if (input.bits.l2) info("LEFT_TRIGGER\n");
        if (input.bits.r2) info("RIGHT_TRIGGER\n");
        //if (input.bits.guide) info("GUIDE\n"); // Home
        if (input.bits.share) info("SHARE\n");
        if (input.bits.a) info("A\n");
        if (input.bits.b) info("B\n");
        if (input.bits.x) info("X\n");
        if (input.bits.y) info("Y\n");
        if (input.bits.mode) info("MODE\n");
        if (input.bits.capture) info("CAPTURE\n");
        if (input.bits.paddle_l) info("PADDLE_L\n");
        if (input.bits.paddle_r) info("PADDLE_R\n");
        */
    }
}

usbh_class_driver_t const* usbh_app_driver_get_cb(uint8_t* driver_count){
    *driver_count = 1;
    return &usbh_xinput_driver;
}

void tuh_xinput_report_received_cb(uint8_t dev_addr, uint8_t instance, xinputh_interface_t const* xid_itf, uint16_t len)
{
    const xinput_gamepad_t *p = &xid_itf->pad;

    if (xid_itf->last_xfer_result == XFER_RESULT_SUCCESS)
    {
        if (xid_itf->connected && xid_itf->new_pad_data)
        {
            debug("[%02x, %02x], Type %s, Buttons %04x, LT: %02x RT: %02x, LX: %d, LY: %d, RX: %d, RY: %d\n",
                dev_addr, instance, xinput_type_to_str(xid_itf->type), p->wButtons, p->bLeftTrigger, p->bRightTrigger, p->sThumbLX, p->sThumbLY, p->sThumbRX, p->sThumbRY);
            mutex_enter_blocking(&passthrough_mutex);
            if (xid_itf->type == TEGENARIA) {
                gamepad.raw.vendor_btns = p->bVendorButtons;
            } else {
                gamepad.raw.standard_btns = p->wButtons;
                gamepad.bits.l2 = p->bLeftTrigger > TRIGGER_THRESHOLD;
                gamepad.bits.r2 = p->bRightTrigger > TRIGGER_THRESHOLD;
                gamepad.thumbstick_lx = p->sThumbLX;
                gamepad.thumbstick_ly = p->sThumbLY;
                gamepad.thumbstick_rx = p->sThumbRX;
                gamepad.thumbstick_ry = p->sThumbRY;
            }
            mutex_exit(&passthrough_mutex);
        }
    }
    tuh_xinput_receive_report(dev_addr, instance);
}

void tuh_xinput_mount_cb(uint8_t dev_addr, uint8_t instance, const xinputh_interface_t *xinput_itf)
{
    const char *gamepad_type = xinput_type_to_str(xinput_itf->type);
    info("PASSTHROUGH: %s mounted addr=%02x instance=%d\n", gamepad_type, dev_addr, instance);
    bool received = tuh_xinput_receive_report(dev_addr, instance);
    if(received) info("PASSTHROUGH: %s controller report received\n", gamepad_type);
}

void tuh_xinput_umount_cb(uint8_t dev_addr, uint8_t instance)
{
    info("PASSTHROUGH: unmounted addr=%02x instance=%d\n", dev_addr, instance);
}
