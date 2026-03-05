#include <inttypes.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "esp_mac.h"
#include "esp_mesh_lite.h"
#include "esp_system.h"
#include "esp_netif.h"

#include "led.h"
#include "mesh_nodes.h"
#include "mesh_status.h"
#include "mesh_config.h"
#if CONFIG_MESH_ROLE_ROOT
#include "root_ready.h"
#else
#include "node_ready.h"
#endif
#include "mesh_tasks.h"

#include "sdkconfig.h"

static const char *TAG = "MAIN";

static esp_err_t esp_storage_init(void)
{
    esp_err_t ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    return ret;
}

// main method
void app_main(void)
{
    ESP_LOGI(TAG, "Starting Mesh Lite v1.0.2");

#if CONFIG_MESH_ROLE_ROOT
    ESP_LOGI(TAG, "Running as ROOT");
#else
    ESP_LOGI(TAG, "Running as NODE");
#endif

    // set log level
    //esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("mesh_lite", ESP_LOG_WARN);

    configure_led();
    set_led(0); // turn off LED

    start_node_ready_indicator_task();

    // Initialize storage
    ESP_ERROR_CHECK(esp_storage_init());

    // Optional: log free heap
    ESP_LOGI(TAG, "Free heap before Wi-Fi: %u bytes", esp_get_free_heap_size());

    // Initialize Wi-Fi and mesh
    wifi_init();

    vTaskDelay(pdMS_TO_TICKS(3000));

    ESP_LOGI(TAG, "Free heap after Wi-Fi start: %u bytes", esp_get_free_heap_size());

    // start the mesh
    mesh_init();

    vTaskDelay(pdMS_TO_TICKS(6000));

#if CONFIG_MESH_ROLE_ROOT
    start_root_ready_task();
#else
    start_node_ready_task();
#endif

    // start status monitoring task
    start_mesh_status_monitor();

#if CONFIG_MESH_ROLE_NODE
    node_connection_scan();
#endif

#if CONFIG_MESH_ROLE_ROOT
    read_sta_MAC_address();
#endif
    read_ap_MAC_address();

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}