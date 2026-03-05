#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ROOT_READY_BIT          BIT0
#define NODE_CONNECTED_BIT      BIT1

EventGroupHandle_t get_mesh_event_group(void);

#ifdef __cplusplus
}
#endif