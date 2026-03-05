#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#if CONFIG_MESH_ROLE_ROOT
#define ROUTER_SSID      "ConnectSecure"
#define ROUTER_PASSWORD  "Con@Vjn#560040"
#define ROUTER_CHANNEL   0    // Changed to 0 to allow auto-scanning for your router
#endif

// mesh AP
#define MESH_AP_PASSWORD    CONFIG_BRIDGE_SOFTAP_PASSWORD
#define MESH_AP_SSID        CONFIG_BRIDGE_SOFTAP_SSID
#define MESH_ID             CONFIG_MESH_LITE_ID
#define MESH_AP_MAX_CONN    CONFIG_MESH_LITE_MAX_ROUTER_NUMBER
#define MESH_AP_WIFI_CHANNEL 0    

#define HOSTNAME            CONFIG_BRIDGE_SOFTAP_SSID

// Wi-Fi country code - "EU" or "IN" both support 13 channels
#define WIFI_COUNTRY "IN" 
#define WIFI_START_CHANNEL 1
#define WIFI_NUM_CHANNELS 13

// Optional Wi-Fi wait time for TCP/IP stabilization
#define WIFI_WAIT_MS 3000

// Heap safety
#define MIN_FREE_HEAP 60000    

// Optional: Max number of nodes for safety
#define MAX_MESH_NODES 10

// fix compile issues in mesh_lite
#define MESH_LITE_VENDOR_ID_0 1
#define MESH_LITE_VENDOR_ID_1 2
#define MESH_LITE_MAX_ROUTER_NUMBER 10    
#define MESH_LITE_MAXIMUM_LEVEL_ALLOWED 4
#define MESH_LITE_DEVICE_CATEGORY "1"
#define MESH_MAX_ROUTER_NUMBER 10    

// various config options
#define MESH_NODE_START_MS 5000

// Wifi TX power
#define SET_TX_WIFI_POWER    

#ifdef __cplusplus
}
#endif