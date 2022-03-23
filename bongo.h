#define ANIMATION_FRAME_DURATION 75 // how long each frame lasts in ms
#define IDLE_TIMEOUT 750 // the amount of time it takes to return to idle
#define KEYS_SIZE 50 // the number of keys stored in the array that tracks keypresses; how many keys are on the board?

#include "bongo_images.h"

enum animation_states {
    Idle,
    Prep,
    Tap
};

struct key_coordinate {
    uint8_t row;
    uint8_t col;
};

struct key_coordinate pressed_keys[KEYS_SIZE];
struct key_coordinate pressed_keys_prev[KEYS_SIZE];

char wpm_str[10];

uint8_t animation_state = Idle;

uint8_t anim_state = Idle;
uint8_t current_idle_frame = 0;
uint8_t current_tap_frame = 0;
uint8_t key_array_index = 0;
uint32_t idle_timeout_timer = 0;
uint32_t animation_timer = 0;

bool detect_key_down(void) {
    // Set local variable that determines if new keys were pressed
    bool new_keys_pressed = false;

    // store the previous cycle's cache
    for (uint8_t i = 0; i < KEYS_SIZE; ++i) {
        pressed_keys_prev[i].row = pressed_keys[i].row;
        pressed_keys_prev[i].col = pressed_keys[i].col;
    }

    // fill cache with currently pressed keys
    key_array_index = 0;

    for (uint8_t row_index = 0; row_index < MATRIX_ROWS; ++row_index) {
        for (uint8_t col_index = 0; col_index < MATRIX_COLS; ++col_index) {
            bool key_is_down = (matrix_get_row(row_index) & (1 << col_index)) > 0;
            if (key_is_down) {
                pressed_keys[key_array_index].row = row_index+1; // adding 1 to the row/col so that we can use 0 as a null-check
                pressed_keys[key_array_index].col = col_index+1;

                if (pressed_keys[key_array_index].row && pressed_keys[key_array_index].col && !pressed_keys_prev[key_array_index].row && !pressed_keys_prev[key_array_index].col) {
                    new_keys_pressed = true;
                }

            }
            else {
                pressed_keys[key_array_index].row = 0;
                pressed_keys[key_array_index].col = 0;
            }
            ++key_array_index;
        }
    }

    return new_keys_pressed;
}

void set_animation_state(void) {
    key_down = detect_key_down();
    
    // It is being set to Prep right after being set to Tap
    // Logic flow: I press when it is idle, it is set to Tap and then Prep immediately (seems like two presses get registered on one)

    switch (animation_state) {
        case Idle:
            if (new_key_press) {
                animation_state = Tap;
            }
            break;

        case Prep:
            if (new_key_press) {
                animation_state = Tap;
            }
            else if (timer_elapsed32(idle_timeout_timer) >= IDLE_TIMEOUT) {
                animation_state = Idle;
                current_idle_frame = 0;
            }
            break;

        case Tap:
            if (!new_key_press) {
                animation_state = Prep;
                idle_timeout_timer = timer_read32();
            }
            break;

        default:
            break;
    }
}

static void draw_bongo(bool minimal) {
    set_animation_state();
    oled_set_cursor(0, 0);

    switch (animation_state) {
    // }
    

    switch (anim_state) {
        case Idle:
            oled_write_raw_P(idle[abs((IDLE_FRAMES - 1) - current_idle_frame)], ANIMATION_SIZE);

            if (timer_elapsed32(animation_timer) > ANIMATION_FRAME_DURATION) {
                current_idle_frame = (current_idle_frame + 1) % IDLE_FRAMES;
                animation_timer = timer_read32();
            }
            break;

        case Prep:
            oled_write_raw_P(prep[0], ANIMATION_SIZE);
            break;

        case Tap:
            oled_write_raw_P(tap[abs((TAP_FRAMES - 1) - current_tap_frame)], ANIMATION_SIZE);
            current_tap_frame = (current_tap_frame + 1) % TAP_FRAMES;
            break;

        default:
            break;
    }

    // if (!minimal) {
    //     // // calculate && print clock
    //     // oled_set_cursor(0, 2);
    //     // uint8_t  hour = last_minute / 60;
    //     // uint16_t minute = last_minute % 60;
    //     // bool is_pm = (hour / 12) > 0;
    //     // hour = hour % 12;
    //     // if (hour == 0) {
    //     //     hour = 12;
    //     // }
    //     // static char time_str[8] = "";
    //     // sprintf(time_str, "%02d:%02d%s", hour, minute, is_pm ? "pm" : "am");
    //     // oled_write(time_str, false);
    // }
}
