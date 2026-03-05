#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void set_country(void);
void app_wifi_set_softap_info(void);
#ifdef SET_TX_WIFI_POWER
void set_tx_wifi_power(void);
void read_tx_wifi_power(void);
#endif

#ifdef __cplusplus
}
#endif