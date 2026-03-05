# 1. Start with the files every build needs
set(srcs 
    "main.c"
    "utils/led.c"
    "mesh/mesh_status.c"
    "mesh/mesh_utils.c"
    "mesh/mesh_events.c"
    "mesh/mesh_tasks.c"
)

# 2. Add files based on the Role (Root or Node)
if(CONFIG_MESH_ROLE_ROOT)
    list(APPEND srcs "mesh/mesh_root.c" "mesh/root_ready.c")
    message(STATUS ">>> BUILDING: ROOT VARIANT <<<")
elseif(CONFIG_MESH_ROLE_NODE)
    list(APPEND srcs "mesh/mesh_node.c" "mesh/node_ready.c")
    message(STATUS ">>> BUILDING: NODE VARIANT <<<")
endif()

# 3. Register everything at once
idf_component_register(
    SRCS ${srcs}
    INCLUDE_DIRS "." "./mesh" "./utils"
    REQUIRES 
        driver 
        esp_driver_gpio
        esp_wifi 
        esp_event 
        esp_netif 
        esp_system 
        nvs_flash 
        mesh_lite 
        iot_bridge
)