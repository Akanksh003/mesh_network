#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void start_mesh_status_monitor(void);
void read_mesh_level(void);
void read_ap_channel(void);
void read_ap_connected_status(void);

void read_ap_MAC_address(void);
#if CONFIG_MESH_ROLE_ROOT
void read_sta_MAC_address(void);
#endif

#ifdef __cplusplus
}
#endif