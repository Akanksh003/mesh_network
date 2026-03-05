#include "esp_wifi.h"
#include "esp_mesh_lite.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "mesh_events.h"
#include "mesh_config.h"

static const char *TAG = "NODE_READY";

// Wait for node to show attached
static void node_ready_task(void *arg)
{
    wifi_ap_record_t ap_info;
    esp_err_t ret;

    ESP_LOGI(TAG, "node_ready_task started");
    ESP_LOGI(TAG, "Scanning for root AP SSID: %s", MESH_AP_SSID);

    EventGroupHandle_t event_group = get_mesh_event_group();

    while (1)
    {
        ret = esp_wifi_sta_get_ap_info(&ap_info);

        if (ret == ESP_OK && strcmp((char *)ap_info.ssid, MESH_AP_SSID) == 0)
        {
            ESP_LOGI(TAG, "Root AP found! RSSI: %d dBm", ap_info.rssi);

            // set the bit - tell waiting tasks we're connected to root
            xEventGroupSetBits(event_group, NODE_CONNECTED_BIT);

            break;
        }

        ESP_LOGI(TAG, "Root AP not found yet, retrying...");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    vTaskDelete(NULL);
}

void start_node_ready_task(void)
{
    xTaskCreate(node_ready_task, "node_ready", 4096, NULL, 5, NULL);
}