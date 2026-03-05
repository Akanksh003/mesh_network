#include "esp_log.h"

#include "esp_mesh_lite_core.h"
#include "esp_mesh_lite.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_mac.h"
#include "esp_bridge.h"

#include "mesh_config.h"
#include "mesh_nodes.h"
#include "mesh_utils.h"
#include "mesh_events.h"

#define NODE_STA_HOSTNAME "MESH_NODE_01"

static const char *TAG = "MESH_NODE";

void wifi_init(void)
{
    // TCP/IP stack
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Create default STA + AP netifs
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    ESP_ERROR_CHECK(esp_netif_set_hostname(sta_netif, NODE_STA_HOSTNAME));

    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // APSTA mode for mesh
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));

    set_country();

    // start the wifi
    ESP_ERROR_CHECK(esp_wifi_start());
    vTaskDelay(pdMS_TO_TICKS(WIFI_WAIT_MS));

    // Disable power save for stability
    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));

    // Wait until connected (event handler recommended)
    vTaskDelay(pdMS_TO_TICKS(10000));

    ESP_LOGI(TAG, "Wifi started");

    wifi_mode_t mode;
    esp_wifi_get_mode(&mode);
    ESP_LOGI(TAG, "Wifi mode: %d", mode);
}

void mesh_init(void)
{
    esp_mesh_lite_config_t mesh_config = ESP_MESH_LITE_DEFAULT_INIT();

    ESP_LOGI(TAG, "mesh_config: mesh_id=%d max_level=%d max_node_number=%d",
             mesh_config.mesh_id, mesh_config.max_level, mesh_config.max_node_number);

    esp_mesh_lite_init(&mesh_config);

    app_wifi_set_softap_info();

    esp_mesh_lite_start();

    ESP_LOGI(TAG, "Mesh NODE initialised with Mesh ID %d", MESH_ID);
}

void node_connection_scan(void)
{
    wifi_ap_record_t ap_info[20];
    uint16_t ap_count = 20;
    ESP_ERROR_CHECK(esp_wifi_scan_start(NULL, true));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&ap_count, ap_info));

    for (int i=0; i < ap_count; i++)
    {
        ESP_LOGI(TAG, "Found AP: SSID=%s, CH=%d, RSSI=%d",
                 ap_info[i].ssid, ap_info[i].primary, ap_info[i].rssi);
    }
}