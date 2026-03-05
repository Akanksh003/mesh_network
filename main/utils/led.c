#include <stdio.h>
#include "driver/gpio.h"
#include "esp_log.h"

#include "led.h"

static const char *TAG = "LED";

static uint8_t s_led_state = 0;

void set_led(uint8_t state)
{
    s_led_state = state;

    /* Set the GPIO level according to the state (LOW or HIGH)*/
    gpio_set_level(BLINK_GPIO, s_led_state);
}

void blink_led(void)
{
    /* Set the GPIO level according to the state (LOW or HIGH)*/
    set_led(s_led_state);

    s_led_state = !s_led_state;
}

void configure_led(void)
{
    ESP_LOGI(TAG, "Example configured to blink GPIO LED!");
    gpio_reset_pin(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    set_led(0);
}