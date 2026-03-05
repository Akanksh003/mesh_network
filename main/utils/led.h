#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#define BLINK_GPIO 2

void blink_led(void);
void configure_led(void);
void set_led(uint8_t state);

#ifdef __cplusplus
}
#endif