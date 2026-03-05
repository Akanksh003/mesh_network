#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_mesh_lite.h"
#include "esp_mesh_lite_core.h"
#include "esp_wifi.h"
#include "esp_mac.h"

#include "mesh_status.h"
#include "mesh_config.h"

#include "sdkconfig.h"

static const char *TAG = "MESH_STATUS";

#if CONFIG_MESH_ROLE_ROOT
static void print_nodes_list(void)
{
    // show the number of attached nodes?
    uint32_t num_nodes = esp_mesh_lite_get_mesh_node_number();
    ESP_LOGI(TAG, "ROOT: # nodes connected: %d", num_nodes);

    uint32_t size = 0;
    const node_info_list_t *node = esp_mesh_lite_get_nodes_list(&size);
    ESP_LOGI(TAG, "ROOT: size: %d", size);
}

void read_sta_MAC_address(void)
{
    uint8_t baseMac[6];
    esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
    if (ret == ESP_OK)
    {
        ESP_LOGI(TAG, "STA MAC address: " MACSTR "", MAC2STR(baseMac));
    }
    else
    {
        ESP_LOGI(TAG, "Unable to get STA MAC address");
    }
}
#endif

void read_ap_connected_status(void)
{
    wifi_ap_record_t ap_info;
    esp_err_t err = esp_wifi_sta_get_ap_info(&ap_info);
    if (err == ESP_OK)
    {
        ESP_LOGI(TAG, "Connected to %s, Channel: %d, RSSI: %d",
                 ap_info.ssid, ap_info.primary, ap_info.rssi);

        ESP_LOGI(TAG, "Parent MAC (BSSID): " MACSTR "", MAC2STR(ap_info.bssid));
    }
    else
    {
        ESP_LOGW(TAG, "Not connected - error=%d", err);
    }
}

void read_mesh_level(void)
{
    int level = esp_mesh_lite_get_level();
    ESP_LOGI(TAG, "Mesh level: %d", level);
}

void read_ap_channel(void)
{
    // Get Wi-Fi channel
    uint8_t channel = 0;
    wifi_second_chan_t second;
    esp_wifi_get_channel(&channel, &second);
    ESP_LOGI(TAG, "Wifi Channel: %d", channel);
}

void read_ap_MAC_address(void)
{
    uint8_t baseMac[6];
    esp_err_t ret = esp_wifi_get_mac(WIFI_IF_AP, baseMac);
    if (ret == ESP_OK)
    {
        ESP_LOGI(TAG, "AP MAC address: " MACSTR "", MAC2STR(baseMac));
    }
    else
    {
        ESP_LOGI(TAG, "Unable to get AP MAC address");
    }
}

void mesh_status_task(void *arg)
{
    ESP_LOGI(TAG, "mesh_status_task started");

    while (1) {

#if CONFIG_MESH_ROLE_ROOT
        print_nodes_list();

        read_sta_MAC_address();
#endif

        read_ap_MAC_address();

        read_ap_channel();

        read_mesh_level();

        read_ap_connected_status();

        vTaskDelay(pdMS_TO_TICKS(5000)); // every 5 seconds
    }
}

void start_mesh_status_monitor(void)
{
    xTaskCreate(mesh_status_task, "mesh_status_task", 4096, NULL, 3, NULL);
}