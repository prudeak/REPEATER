#include "bluetooth_serial.h"

#include "nvs_flash.h"

#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_spp_api.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"

static uint32_t spp_handle = 0; // Дескриптор соединения bluetooth SPP
static void spp_callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);

esp_err_t bluetooth_serial_init(){
    const char * TAG = "BT_INIT";
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_LOGI(TAG, "Bluetooth SPP Initialization started");
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_BLE));
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_bt_controller_init(&bt_cfg));
    ESP_ERROR_CHECK(esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT));
    esp_bluedroid_config_t bluedroid_cfg = BT_BLUEDROID_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_bluedroid_init_with_cfg(&bluedroid_cfg));
    ESP_ERROR_CHECK(esp_bluedroid_enable());

    esp_spp_register_callback(spp_callback);
    esp_spp_cfg_t bt_spp_cfg = {
        .mode = ESP_SPP_MODE_CB,
        .enable_l2cap_ertm = true,
        .tx_buffer_size = 0, /* Only used for ESP_SPP_MODE_VFS mode */
    };
    ESP_ERROR_CHECK(esp_spp_enhanced_init(&bt_spp_cfg));
    ESP_LOGI(TAG, "Bluetooth SPP Initialized");
    return ESP_OK;
};

static void spp_callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
   const char * TAG = "BT_SP_Callback";
    switch (event) {
        case ESP_SPP_INIT_EVT:
            ESP_LOGI(TAG, "SPP initialized");
            //esp_bt_dev_set_device_name("ESP_REPEATER");
            esp_bt_gap_set_device_name("ESP_REP_V3");
            esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);
            esp_spp_start_srv(ESP_SPP_SEC_NONE, ESP_SPP_ROLE_SLAVE, 0, "ESP_REP_V3");
            break;
        case ESP_SPP_SRV_OPEN_EVT:
            ESP_LOGI(TAG, "SPP server connection open");
            spp_handle = param->srv_open.handle; // Сохраняем дескриптор соединения
            break;
        case ESP_SPP_CLOSE_EVT:
            ESP_LOGI(TAG, "SPP server connection closed");
            spp_handle = 0;
            break;
        default:
            break;
    } 
};
esp_err_t bluetooth_serial_write(const char * const data, const size_t size){
    if (spp_handle){
        return esp_spp_write(spp_handle, size, (uint8_t*)data);
    }else{
        return ESP_ERR_NOT_ALLOWED;
    }
};