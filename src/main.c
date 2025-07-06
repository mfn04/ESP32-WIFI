#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "stdbool.h"
#include "string.h"

static bool ssid_found(const char* ssid) {

    esp_err_t err = esp_wifi_scan_start(NULL, true);
    switch(err){
        case ESP_ERR_WIFI_NOT_INIT:
            ESP_LOGW("WIFI ERROR", "WiFi was not initialized!");
            return false;
        case ESP_ERR_WIFI_NOT_STARTED:
            ESP_LOGW("WIFI ERROR", "WiFi was not started!");
            return false;
        case ESP_ERR_WIFI_STATE:
            ESP_LOGW("WIFI ERROR", "WiFi can't scan while connecting!");
            return false;
        case ESP_ERR_WIFI_TIMEOUT:
            ESP_LOGW("WIFI ERROR", "WiFi scan was blocked by timeout!");
            return false;
        default:
            ESP_LOGI("INFO", "WiFi scan was successful!");
    }

    uint16_t num = 0;

    esp_wifi_scan_get_ap_num(&num);
    ESP_LOGI("INFO", "WiFi APs Found: %d", num);


    wifi_ap_record_t* records = (wifi_ap_record_t*)malloc(sizeof(wifi_ap_record_t)*num);

    esp_wifi_scan_get_ap_records(&num, records);
    
    bool found = false;

    for(int i = 0; i < num; i++){
        ESP_LOGI("INFO", "Found SSID: %s", records[i].ssid);
        if(strcmp((const char*)records[i].ssid,ssid) == 0) {
            found = true;
        }
    }
    free(records);
    return found;
}


void app_main() {

    ESP_ERROR_CHECK(nvs_flash_init());

    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init_config_t conf = WIFI_INIT_CONFIG_DEFAULT();

    esp_wifi_init(&conf);

    esp_wifi_set_mode(WIFI_MODE_APSTA);

    esp_wifi_start();

    const char* ssid = "TP-Link 555 BH";

    bool found = ssid_found(ssid);

    if(found){
        ESP_LOGI("INFO","Successfully found the WiFi AP '%s'!", ssid);
    } else {
        ESP_LOGI("INFO","WiFi AP '%s' was not found!", ssid);
    }


}