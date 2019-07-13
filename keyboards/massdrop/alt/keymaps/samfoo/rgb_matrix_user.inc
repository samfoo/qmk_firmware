RGB_MATRIX_EFFECT(reactive_gradient)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static bool reactive_gradient(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    for (uint8_t i = led_min; i < led_max; i++) {
        rgb_matrix_set_color(i, 0xff, 0xff, 0xff);
    }

    return led_max < DRIVER_LED_TOTAL;
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS

