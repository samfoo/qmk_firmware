RGB_MATRIX_EFFECT(reactive_complement_multiwide)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

const uint8_t COLOUR_WHEEL_OPP = 128;

static uint8_t reactive_complement_math(uint8_t base, uint8_t comp, uint8_t intensity, uint8_t dist, uint16_t tick) {
    uint16_t effect = tick + dist * 3;
    if (effect > COLOUR_WHEEL_OPP) {
        effect = COLOUR_WHEEL_OPP;
    }

    uint8_t rs = qadd8(intensity, COLOUR_WHEEL_OPP - effect);

    if (rs > COLOUR_WHEEL_OPP) {
        return COLOUR_WHEEL_OPP;
    } else {
        return rs;
    }
}

bool reactive_complement_effect(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    HSV ghsv = rgb_matrix_config.hsv;
    uint8_t base = ghsv.h;
    uint8_t comp = base + COLOUR_WHEEL_OPP;

    // Loop through every led on the keyboard...
    for (uint8_t i = led_min; i < led_max; i++) {
        // (???) cargo culted from the original code.
        RGB_MATRIX_TEST_LED_FLAGS();

        // get the global colour settings
        HSV hsv = rgb_matrix_config.hsv;

        // Figure out what intensity (between the base and complement colour)
        // this led is going to be.
        uint8_t intensity = 0;

        // Loop through all of our recently pressed keys...
        for (uint8_t j = 0; j < g_last_hit_tracker.count; j++) {
            int16_t dx = g_led_config.point[i].x - g_last_hit_tracker.x[j];
            int16_t dy = g_led_config.point[i].y - g_last_hit_tracker.y[j];

            // compute the distance from the current pressed key to the current LED
            uint8_t dist = sqrt16(dx * dx + dy * dy);

            // scale the time since the current key was pressed by the currently
            // configured effect speed.
            uint16_t tick = scale16by8(g_last_hit_tracker.tick[j], rgb_matrix_config.speed);

            // Increment the aggregate intensity of this LED (based on all the
            // input from all the keys that have been pressed.
            intensity = reactive_complement_math(base, comp, intensity, dist, tick);
        }

        // Given the intensity, add it to the base colour. Intentionally this
        // is able to overflow, because the overflow is what properly sets the
        // colour wheel complement.
        hsv.h += intensity;

        // update the LED matrix for this LED
        RGB rgb = hsv_to_rgb(hsv);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }

    return led_max < DRIVER_LED_TOTAL;
}


// This function gets called periodically (on any keypress???). I'm not really
// sure on the lifecycle of the custom LED functions.
static bool reactive_complement_multiwide(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    for (uint8_t i = led_min; i < led_max; i++) {
        rgb_matrix_set_color(i, 0xff, 0xff, 0xff);
    }

    return reactive_complement_effect(params);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS

