#include "esp_wifi.h"
#include "esp_mesh_lite.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "mesh_events.h"

static const char *TAG = "ROOT_READY";

static void root_ready_task(void *arg)
{
    wifi_config_t ap_conf;
    int mesh_level = -1;
    uint8_t channel = 0;

    ESP_LOGI(TAG, "root_ready_task started");

    EventGroupHandle_t event_group = get_mesh_event_group();

    while (1)
    {
        // Get mesh level
        mesh_level = esp_mesh_lite_get_level();

        // Get AP channel
        wifi_second_chan_t second;
        esp_wifi_get_channel(&channel, &second);

        // Get AP SSID info
        esp_wifi_get_config(WIFI_IF_AP, &ap_conf);

        // Check if AP and mesh are fully ready
        if (mesh_level >= 1 && channel != 0) {
            ESP_LOGI(TAG, "=== ROOT READY ===");
            ESP_LOGI(TAG, "Mesh Level: %d", mesh_level);
            ESP_LOGI(TAG, "AP SSID: %s", ap_conf.ap.ssid);
            ESP_LOGI(TAG, "SSID Hidden: %d", ap_conf.ap.ssid_hidden);
            ESP_LOGI(TAG, "Wi-Fi Channel: %d", channel);
            ESP_LOGI(TAG, "Max Children: %d", ap_conf.ap.max_connection);
            ESP_LOGI(TAG, "==================");

            // set the bit - tell waiting tasks we're done
            xEventGroupSetBits(event_group, ROOT_READY_BIT);

            break;  // exit loop, root is ready
        }

        ESP_LOGI(TAG, "Waiting for root AP to start... Level=%d Channel=%d", mesh_level, channel);
        vTaskDelay(pdMS_TO_TICKS(500));  // check every 0.5s
    }

    vTaskDelete(NULL);
}

void start_root_ready_task(void)
{
    xTaskCreate(root_ready_task, "root_ready", 4096, NULL, 5, NULL);
}