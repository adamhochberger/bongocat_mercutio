#define KEYS_SIZE 50 // the number of keys stored in the array that tracks keypresses; how many keys are on the board?
#define IDLE_TIMEOUT 750 // the amount of time it takes to return to idle
#define ANIMATION_FRAME_DURATION 75 // how long each frame lasts in ms

#include "bongo_images.h"

enum animation_states {
    Idle,
    Prep,
    Tap
};

matrix_row_t pressed_keys[MATRIX_ROWS];
matrix_row_t pressed_keys_prev[MATRIX_ROWS];

bool new_key_press = false;

uint8_t animation_state = Idle;
uint8_t current_idle_frame = 0;
uint8_t current_tap_frame = 0;
uint32_t idle_timeout_timer = 0;
uint32_t animation_timer = 0;

// Function that determines if a new key was pressed, while updating the pressed keys cache
bool check_if_new_key_press(void) {
    
    // Set local variable that determines if at least one new key was pressed
    bool new_key_pressed = false;

    // Store the previous cycle's cache
    for (uint8_t cache_index = 0; cache_index < MATRIX_ROWS; ++cache_index) {
        pressed_keys_prev[cache_index] = pressed_keys[cache_index];
    }
    
    // Double for-loop to load cache with values that determine if a key was pressed or not
    for (uint8_t row_index = 0; row_index < MATRIX_ROWS; ++row_index) {

        // Obtains the integer representation of which keys are pressed on the row
        // Since this value is unique depending on the keys pressed, we can store that and check against it later on
        matrix_row_t pressed_keys_on_row_as_bits = matrix_get_row(row_index);
        pressed_keys[row_index] = pressed_keys_on_row_as_bits;

        // Performs check against cached value for the pressed_key and sets a boolean to return once the processing is complete (still need to obtain all pressed keys for the next cache)
        if (pressed_keys[row_index] > 0 && pressed_keys[row_index] != pressed_keys_prev[row_index]) {
            new_key_pressed = true;
        }
    }

    return new_key_pressed;
}

void set_animation_state(void) {
    new_key_press = check_if_new_key_press();

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
