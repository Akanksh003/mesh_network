#include "esp_log.h"

#include "esp_mesh_lite_core.h"
#include "esp_mesh_lite.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"

#include "mesh_config.h"
#include "mesh_utils.h"

#define WIFI_COUNTRY "IN"       // India
#define WIFI_START_CHANNEL 1    
#define WIFI_NUM_CHANNELS 13    // Use all 13 channels

static const char *TAG = "MESH_UTILS";


#ifdef SET_TX_WIFI_POWER
void set_tx_wifi_power(void)
{
    esp_wifi_set_max_tx_power(1);

    read_tx_wifi_power();
}

void read_tx_wifi_power(void)
{
    wifi_ap_record_t ap;
    esp_wifi_sta_get_ap_info(&ap);
    ESP_LOGI(TAG, "RSSI: %d\n", ap.rssi);
}
#endif

void set_country(void)
{
    // wifi country code
    wifi_country_t country = {
        .cc = WIFI_COUNTRY,             // change to your country code
        .schan = WIFI_START_CHANNEL,    // starting channel
        .nchan = WIFI_NUM_CHANNELS,     // number of channels
        .policy = WIFI_COUNTRY_POLICY_MANUAL   // manual - do not use auto
    };
    ESP_ERROR_CHECK(esp_wifi_set_country(&country));

    ESP_LOGI(TAG, "Country info set");
}

void app_wifi_set_softap_info(void)
{
    char softap_ssid[33];
    char softap_psw[64];
    uint8_t softap_mac[6];
    size_t ssid_size = sizeof(softap_ssid);
    size_t psw_size = sizeof(softap_psw);
    esp_wifi_get_mac(WIFI_IF_AP, softap_mac);
    memset(softap_ssid, 0x0, sizeof(softap_ssid));
    memset(softap_psw, 0x0, sizeof(softap_psw));

    if (esp_mesh_lite_get_softap_ssid_from_nvs(softap_ssid, &ssid_size) == ESP_OK) {
        ESP_LOGI(TAG, "Get ssid from nvs: %s", softap_ssid);
    } else {
#ifdef CONFIG_BRIDGE_SOFTAP_SSID_END_WITH_THE_MAC
        snprintf(softap_ssid, sizeof(softap_ssid), "%s", CONFIG_BRIDGE_SOFTAP_SSID);
#else
        snprintf(softap_ssid, sizeof(softap_ssid), "%.32s", CONFIG_BRIDGE_SOFTAP_SSID);
#endif
        ESP_LOGI(TAG, "Get ssid from nvs failed, set ssid: %s", softap_ssid);
    }

    if (esp_mesh_lite_get_softap_psw_from_nvs(softap_psw, &psw_size) == ESP_OK) {
        ESP_LOGI(TAG, "Get psw from nvs: [HIDDEN]");
    } else {
        strlcpy(softap_psw, CONFIG_BRIDGE_SOFTAP_PASSWORD, sizeof(softap_psw));
        ESP_LOGI(TAG, "Get psw from nvs failed, set psw: [HIDDEN]");
    }

    esp_mesh_lite_set_softap_info(softap_ssid, softap_psw);
}