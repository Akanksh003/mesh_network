#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include "mesh_events.h"

static const char *TAG = "MESH_EVENTS";

static EventGroupHandle_t event_group;          // event control for mesh

EventGroupHandle_t get_mesh_event_group(void)
{
    if (event_group == NULL)
    {
        event_group = xEventGroupCreate();
    }
    return event_group;
}