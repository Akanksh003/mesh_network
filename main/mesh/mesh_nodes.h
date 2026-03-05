#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void wifi_init(void);
void mesh_init(void);

#if CONFIG_MESH_ROLE_NODE
void node_connection_scan(void);
void wait_for_root_ap(void);
#endif

#ifdef __cplusplus
}
#endif