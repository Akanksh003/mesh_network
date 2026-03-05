#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_log.h"

#include "mesh_events.h"
#include "mesh_tasks.h"
#include "led.h"

static const char *TAG = "MESH_TASKS";

static void node_ready_indicator_task(void *arg)
{
    ESP_LOGI(TAG, "node_ready_indicator_task started");

    configure_led();

    EventGroupHandle_t event_group = get_mesh_event_group();

    ESP_LOGI(TAG, "Wait node event bits");

    while (1)
    {
        EventBits_t bits = xEventGroupWaitBits(
            event_group,
            ROOT_READY_BIT | NODE_CONNECTED_BIT,
            pdFALSE,
            pdFALSE,
            portMAX_DELAY
        );

        blink_led();

        vTaskDelay(pdMS_TO_TICKS(250));
    }
}

void start_node_ready_indicator_task(void)
{
    xTaskCreate(node_ready_indicator_task, "node_ready_indicator", 4096, NULL, 5, NULL);
}