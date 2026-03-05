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

static const char *TAG = "MESH_ROOT";

static char hostname[20];
static char* get_hostname(void)
{
    snprintf(hostname, sizeof(hostname), "%s_%02d", HOSTNAME, MESH_ID);
    return hostname;
}

static void set_sta_hostname(void)
{
    // Get default STA netif
    esp_netif_t *sta_netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    if (!sta_netif) {
        ESP_LOGE(TAG, "STA netif not found");
        return;
    }

    // Set the hostname
    char* hostname = get_hostname();
    esp_err_t err = esp_netif_set_hostname(sta_netif, hostname);
    if (err == ESP_OK)
    {
        ESP_LOGI(TAG, "STA hostname set to %s", hostname);
    }
    else
    {
        ESP_LOGE(TAG, "Failed to set hostname: %d", err);
    }
}

static void wifi_sta_init(void)
{
    // STA configuration for router connection
    
    wifi_config_t sta_config = { 0 };
    strcpy((char *)sta_config.sta.ssid, ROUTER_SSID);
    strcpy((char *)sta_config.sta.password, ROUTER_PASSWORD);
    sta_config.sta.scan_method = WIFI_ALL_CHANNEL_SCAN;
    sta_config.sta.sort_method = WIFI_CONNECT_AP_BY_SECURITY;
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &sta_config));

    // Set custom hostname
    set_sta_hostname();

    ESP_LOGI(TAG, "Root Wi-Fi STA initialized");
}

static void wifi_ap_init(void)
{
    // AP configuration (for Nodes)

    wifi_config_t ap_config = { 0 };
    strcpy((char *)ap_config.ap.ssid, MESH_AP_SSID);
    strcpy((char *)ap_config.ap.password, MESH_AP_PASSWORD);
    ap_config.ap.ssid_len = strlen(MESH_AP_SSID);
    ap_config.ap.channel = MESH_AP_WIFI_CHANNEL;
    ap_config.ap.authmode = WIFI_AUTH_WPA2_PSK;
    ap_config.ap.max_connection = MESH_AP_MAX_CONN;
    ap_config.ap.ssid_hidden = 0;    // visible
    ap_config.ap.beacon_interval = 200;    // if nodes not attaching change this (multiples of 100)
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_config));
}

void wifi_init(void)
{
    // TCP/IP stack
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Create default STA interface (router connection)
    esp_netif_create_default_wifi_sta();

    // Create default AP interface for mesh children
    esp_netif_create_default_wifi_ap();

    // Initialise WiFi
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // Set WiFi APSTA mode - AP+STA needed for root
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));

    // STA config (connect to router)
    wifi_sta_init();
    ESP_LOGI(TAG, "Wi-Fi STA initialized");

    // AP config (for nodes)
    wifi_ap_init();
    ESP_LOGI(TAG, "Wi-Fi AP initialized");

    // Disable power save for stability
    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));

    set_country();

    // start the wifi
    ESP_ERROR_CHECK(esp_wifi_start());
    vTaskDelay(pdMS_TO_TICKS(WIFI_WAIT_MS));

    ESP_LOGI(TAG, "Connecting to router on channel %d...", ROUTER_CHANNEL);

    ESP_ERROR_CHECK(esp_wifi_connect());

    // Wait until connected (event handler recommended)
    vTaskDelay(pdMS_TO_TICKS(3000));

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

    vTaskDelay(pdMS_TO_TICKS(WIFI_WAIT_MS));

    ESP_LOGI(TAG, "Mesh ROOT initialized with Mesh ID %d", MESH_ID);
}