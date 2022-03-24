/* Copyright 2021 Kyle McCreery
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include <stdio.h>

#define BONGO_ENABLE
#define _QWERTY 0
#define _NUM 1
#define _F 2
#define _EMPTY 3

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QWERTY] = LAYOUT_all(
                                                                                                                  KC_MUTE,
      KC_TAB,           KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
      MO(_NUM),            KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_QUOT, KC_ENT,
      KC_LSFT, KC_SLSH, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,           KC_RSFT,
      KC_LCTL, KC_LGUI, KC_LALT,          KC_SPC,  KC_SPC,           KC_SPC,           KC_RALT, MO(_F),            KC_RCTL ),

  [_NUM] = LAYOUT_all(
                                                                                                                  KC_TRNS,
  	  KC_ESC,           KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_DEL,
  	  KC_TRNS,          KC_TRNS, KC_GRV,  KC_BSLS, KC_TRNS, KC_TRNS, KC_TRNS, KC_LBRC, KC_RBRC, KC_SCLN, KC_TRNS, KC_QUOT,
  	  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MINS, KC_EQL,  KC_SLSH,          KC_UP,
  	  KC_TRNS, KC_TRNS, KC_TRNS,          KC_HOME, KC_TRNS,          KC_END,           KC_LEFT, KC_DOWN,          KC_RIGHT ),

  [_F] = LAYOUT_all(
                                                                                                                  KC_TRNS,
  	  KC_TRNS,          KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_TRNS,
  	  KC_CAPS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  	  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,
  	  KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS,          KC_TRNS,          KC_TRNS, KC_TRNS,          KC_TRNS ),

  [_EMPTY] = LAYOUT_all(
                                                                                                                  KC_TRNS,
  	  KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  	  KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  	  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,
  	  KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS,          KC_TRNS,          KC_TRNS, KC_TRNS,          KC_TRNS )
};

#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
    switch (index) {
        case 0:
            if (clockwise) {
                tap_code(KC_VOLU);
            } else {
                tap_code(KC_VOLD);
            }
        break;
    }
    return true;
}
#endif

#ifdef OLED_ENABLE
#ifdef BONGO_ENABLE
#include "bongo.h"

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_180;  // flips the display 180 degrees
}

bool oled_task_kb(void) {
    if (!oled_task_user()) { return false; }

    // // Host Keyboard Layer Status
    // oled_write_P(PSTR("Layer: "), false);

    // switch (get_highest_layer(layer_state)) {
    //     case _QWERTY:
    //         oled_write_P(PSTR("Default\n"), false);
    //         break;
    //     case _NUM:
    //         oled_write_P(PSTR("Nums\n"), false);
    //         break;
    //     case _F:
    //         oled_write_P(PSTR("Func\n"), false);
    //         break;
    //     case _EMPTY:
    //         oled_write_P(PSTR("None\n"), false);
    //         break;
    //     default:
    //         // Or use the write_ln shortcut over adding '\n' to the end of your string
    //         oled_write_ln_P(PSTR("Undefined"), false);
    // }
    oled_clear();
    draw_bongo(false);

    #ifdef WPM_ENABLE
    oled_set_cursor(0,4);
    sprintf(wpm_str, "WPM: %03d", get_current_wpm());
    oled_write(wpm_str, false);
    #endif
    
    return false;
}

#endif
#endif