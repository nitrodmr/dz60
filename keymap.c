#include QMK_KEYBOARD_H

LEADER_EXTERNS();
// Tap Dance Declarations
//   CAKEWARP - moves between layers on tap

enum { CAKEWARP = 0, TD_ESC_CAPS = 1, TD_ESC_CLOSE = 2, TD_SPECIAL_KEYS = 3, TD_MO };

typedef struct {
    bool    is_press_action;
    uint8_t state;
} tap;

enum {
    SINGLE_TAP = 1,
    SINGLE_HOLD,
    DOUBLE_TAP,
    DOUBLE_HOLD,
    DOUBLE_SINGLE_TAP,  // Send two single taps
    TRIPLE_TAP,
    TRIPLE_HOLD
};

enum {
    BASE=0,
    LEDS = 1,
    QUICK=2,
    PANZER=3,
    NAV = 4
};

uint8_t cur_dance(qk_tap_dance_state_t *state);

// Functions associated with individual tap dances
void ql_finished(qk_tap_dance_state_t *state, void *user_data);
void ql_reset(qk_tap_dance_state_t *state, void *user_data);

void cake_count(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        //// Check to see if the layer is already set
        if (layer_state_is(PANZER)) {
            // If already set, then switch it off
            layer_off(PANZER);
            reset_tap_dance(state);
        } else if (layer_state_is(NAV)) {
            layer_off(PANZER);  // define single tap or hold here
            layer_off(NAV);
            rgblight_enable();
            rgblight_mode(RGBLIGHT_MODE_TWINKLE + 4);
            reset_tap_dance(state);
        } else {
            // If not already set, then switch the layer on
            layer_on(PANZER);
        }

    } else if (state->count == 2) {
        // Navigation
        layer_on(NAV);  // define double tap here
        layer_off(PANZER);
        rgblight_enable();
        rgblight_mode(RGBLIGHT_MODE_ALTERNATING);
    } else if (state->count == 3) {
        // Number Pad
        layer_on(PANZER);  // define triple tap here
        layer_off(NAV);
        rgblight_mode(RGBLIGHT_MODE_KNIGHT);

    } else {
        // Base Layer
        layer_off(PANZER);  // define single tap or hold here
        layer_off(NAV);
        rgblight_enable();
        rgblight_mode(RGBLIGHT_MODE_TWINKLE + 4);

        reset_tap_dance(state);
    }
}


void special_keys(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        // Navigation
        layer_on(4);  // define double tap here
        layer_off(3);
        layer_off(5);
        rgblight_enable();
        rgblight_mode(RGBLIGHT_MODE_ALTERNATING);
    } else {
        // Base Layer
        layer_off(3);  // define single tap or hold here
        layer_off(4);
        layer_off(5);
        rgblight_enable();
        rgblight_mode(RGBLIGHT_MODE_TWINKLE + 4);
        reset_tap_dance(state);
    }
}

// Tap Dance Definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    // tap for Layer 0, tap twice to switch to symbol layer, and tap three times to mouse layer
    [CAKEWARP]        = ACTION_TAP_DANCE_FN(cake_count),
    [TD_ESC_CAPS]     = ACTION_TAP_DANCE_DOUBLE(KC_ESC, KC_GRV),
    [TD_SPECIAL_KEYS] = ACTION_TAP_DANCE_FN(special_keys),
    [TD_MO]           = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, ql_finished, ql_reset, 275)

};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [BASE] = LAYOUT_all(
		TD(TD_ESC_CAPS), KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_NO, KC_BSPC, 
		KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS, 
		TD(CAKEWARP), KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENT, 
		KC_LSFT, KC_NO, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, MO(1), KC_UP , KC_RSFT, 
		KC_LCTL, KC_LGUI, KC_LALT, KC_SPC, TD(TD_MO), KC_LEAD, KC_DEL, KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT),

	[LEDS] = LAYOUT_all(
		KC_GRV, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_TRNS, KC_DEL, 
		KC_TRNS, RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, RESET, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, BL_DEC, BL_TOGG, BL_INC, BL_STEP, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),

	[QUICK] = LAYOUT_all(
		KC_TILD, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_BSPC, KC_BSPC, 
		KC_TAB, KC_LALT, KC_TAB, KC_END, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_UP, KC_TRNS, KC_PSCR, KC_MEDIA_PREV_TRACK, KC_MEDIA_NEXT_TRACK, KC_AUDIO_MUTE, 
		KC_CAPS, KC_PGUP, KC_LCTL, KC_PGDOWN, KC_TRNS, KC_TRNS, KC_HOME, KC_LEFT, KC_DOWN, KC_RGHT, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_RSFT, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_VOLD, KC_VOLU, KC_MPLY, KC_INSERT, KC_PGUP, KC_TRNS,
		KC_LCTL, KC_LGUI, KC_LALT, KC_BSPC, KC_TRNS, KC_BSPC, KC_DEL, KC_TRNS, KC_TRNS, KC_PGDOWN, KC_TRNS),

	[PANZER] = LAYOUT_all(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_7, KC_8, KC_9, KC_SLSH, KC_MINS, KC_EQL, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_EXLM, KC_AT, KC_LCBR, KC_RCBR, KC_PIPE, KC_TRNS, KC_4, KC_5, KC_6, KC_ASTR, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_HASH, KC_DLR, KC_LPRN, KC_RPRN, KC_GRAVE, KC_TRNS, KC_1, KC_2, KC_3, KC_MINS, KC_TRNS, KC_ENT, 
		KC_TRNS, KC_TRNS, KC_PERC, KC_CIRC, KC_LBRACKET, KC_RBRACKET, KC_TILD, KC_TRNS, KC_0, KC_0, KC_DOT, KC_PLUS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),

    [NAV] = LAYOUT_all(
		KC_TILD, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_BSPC, KC_BSPC, 
		KC_TAB, KC_TRNS, KC_PGUP, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_UP, KC_TRNS, KC_TRNS, KC_MEDIA_PREV_TRACK, KC_MEDIA_NEXT_TRACK, KC_AUDIO_MUTE, 
		KC_TRNS, KC_HOME, KC_PGDOWN, KC_END, KC_TRNS, KC_TRNS, KC_TRNS, KC_LEFT, KC_DOWN, KC_RGHT, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_LSFT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_VOLD, KC_VOLU, KC_MPLY, KC_INSERT, KC_PGUP, KC_TRNS,
		KC_LCTL, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_BSPC, KC_TRNS, KC_TRNS, KC_TRNS, KC_PGDOWN, KC_TRNS),
};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt) {
    switch (id) {
        case 0:
            if (record->event.pressed) {
                return MACRO(D(UP), D(LEFT), U(UP), U(LEFT), END);
            }
            break;
        case 1:
            if (record->event.pressed) {
                return MACRO(D(LCTL), T(LEFT), U(LCTL), END);
            }
            break;
        case 2:
            if (record->event.pressed) {
                return MACRO(D(LCTL), T(RGHT), U(LCTL), END);
            }
            break;
        case 3:
            if (record->event.pressed) {
                return MACRO(D(LCTL), D(GRV), U(LCTL), U(GRV), END);
            }
            break;
    }
    return MACRO_NONE;
}

void matrix_init_user(void) {}

bool process_record_user(uint16_t keycode, keyrecord_t *record) { return true; }

void led_set_user(uint8_t usb_led) {
    if (usb_led & (1 << USB_LED_NUM_LOCK)) {
    } else {
    }

    if (usb_led & (1 << USB_LED_CAPS_LOCK)) {
        DDRB |= (1 << 2);
        PORTB &= ~(1 << 2);
    } else {
        DDRB &= ~(1 << 2);
        PORTB &= ~(1 << 2);
    }

    if (usb_led & (1 << USB_LED_SCROLL_LOCK)) {
    } else {
    }

    if (usb_led & (1 << USB_LED_COMPOSE)) {
    } else {
    }

    if (usb_led & (1 << USB_LED_KANA)) {
    } else {
    }
}

bool did_leader_succeed;

void leader_start(void) {
    // Add your code to run when pressing the leader key here
}
void leader_end(void) {
    if (did_leader_succeed) {
        // If any sequence was matched, did_leader_succeed will have
        // been set to true up in the matrix_scan_user function.
        // Put your code for a matched leader key sequence here.
    } else {
        // If no sequence was matched, did_leader_succeed will not
        // have been set to true anywhere, so we'll end up here.
        // Put your code for an unmatched leader key sequence here.
    }
}

void matrix_scan_user(void) {
    LEADER_DICTIONARY() {
        leading            = false;
        did_leader_succeed = leading = false;

        // Replace the sequence below with your own sequences.
        SEQ_TWO_KEYS(KC_C, KC_H) {
            send_string(SS_LCTRL(SS_LSFT("t")));
            // Chrome reopen closed tab
            did_leader_succeed = true;
        }
        SEQ_TWO_KEYS(KC_C, KC_N) {
            send_string(SS_LCTRL("n"));
            // Chrome new window
            did_leader_succeed = true;
        }
        SEQ_TWO_KEYS(KC_C, KC_I) {
            send_string(SS_LCTRL(SS_LSFT("n")));
            // Chrome new incog tab
            did_leader_succeed = true;
        }
        SEQ_TWO_KEYS(KC_N, KC_T) {
            // Chrome new tab
            send_string(SS_LCTRL("t"));
            did_leader_succeed = true;
        }
        SEQ_TWO_KEYS(KC_O, KC_D) {
            // Open directory in vscode
            send_string(SS_LCTRL("ko"));
            did_leader_succeed = true;
        }
        SEQ_TWO_KEYS(KC_O, KC_F) {
            // Open file in vscode
            send_string(SS_LCTRL("o"));
            did_leader_succeed = true;
        }
        SEQ_ONE_KEY(KC_P) {
            // Print Screen
            register_code(KC_LALT);
            register_code(KC_PSCR);
            unregister_code(KC_LALT);
            unregister_code(KC_PSCR);
            did_leader_succeed = true;
        }
        SEQ_ONE_KEY(KC_C) {
            // Center Cursor
            send_string(SS_TAP(X_LEFT));
            send_string(SS_TAP(X_LEFT));
            did_leader_succeed = true;
        }
        SEQ_ONE_KEY(KC_T) {
            // Toggle terminal in vscode
            send_string(SS_LCTRL("`"));
            did_leader_succeed = true;
        }
        SEQ_THREE_KEYS(KC_C, KC_A, KC_D) {
            // ctrl+alt+del
            register_code(KC_LCTL);
            register_code(KC_LALT);
            register_code(KC_DEL);
            unregister_code(KC_LCTL);
            unregister_code(KC_LALT);
            unregister_code(KC_DEL);
        }
        SEQ_ONE_KEY(KC_4) {
            // alt + f4
            // closes current program
            send_string(SS_LALT(SS_TAP(X_F4)));
        }
        SEQ_ONE_KEY(KC_L) {
            // windows + l
            // logout
            register_code(KC_LGUI);
            register_code(KC_L);
            unregister_code(KC_L);
            unregister_code(KC_LGUI);
        }
        SEQ_ONE_KEY(KC_LSFT) {
            layer_on(4);
            layer_off(3);  // define single tap or hold here
            layer_off(5);
        }
        SEQ_ONE_KEY(KC_RSFT) {
            layer_off(4);
            layer_off(3);  // define single tap or hold here
            layer_off(5);
        }
        leader_end();
    }
}

// Determine the current tap dance state
uint8_t cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (!state->pressed) {
            return SINGLE_TAP;
        } else {
            return SINGLE_HOLD;
        }
    } else if (state->count == 2) {
        if (!state->pressed) {
            return DOUBLE_TAP;
        }
        else {
            return DOUBLE_HOLD;
        }

    } else
        return 8;
}

// Initialize tap structure associated with example tap dance key
static tap ql_tap_state = {.is_press_action = true, .state = 0};

// Functions that control what our tap dance key does
void ql_finished(qk_tap_dance_state_t *state, void *user_data) {
    ql_tap_state.state = cur_dance(state);
    switch (ql_tap_state.state) {
        case SINGLE_TAP:
            set_oneshot_layer(QUICK, ONESHOT_START);
            clear_oneshot_layer_state(ONESHOT_PRESSED);
            break;
        case SINGLE_HOLD:
            layer_on(QUICK);
            break;
        case DOUBLE_HOLD:
            //// Check to see if the layer is already set
            // if (layer_state_is(_MY_LAYER)) {
            //    // If already set, then switch it off
            //    layer_off(_MY_LAYER);
            //} else {
            //    // If not already set, then switch the layer on
            //    layer_on(_MY_LAYER);
            //}
            layer_on(PANZER);
            break;
        case DOUBLE_TAP:
            set_oneshot_layer(PANZER, ONESHOT_START);
            clear_oneshot_layer_state(ONESHOT_PRESSED);
            break;
    }
}

void ql_reset(qk_tap_dance_state_t *state, void *user_data) {
    // If the key was held down and now is released then switch off the layer
    switch (ql_tap_state.state) {
        case SINGLE_TAP:
            break;
        case SINGLE_HOLD:
            layer_off(QUICK);
            break;
        case DOUBLE_TAP:
            break;
        case DOUBLE_HOLD:
            layer_off(PANZER);
            break;
    }
    ql_tap_state.state = 0;
}

