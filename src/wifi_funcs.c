#include "wifi_funcs.h"

const char* rssi_to_str(int8_t rssi) {
    if(rssi >= -50) {
        return "Excellent";
    } else if(rssi <= -51 && rssi >= -60) {
        return "Good";
    } else if(rssi <= -60 && rssi >= -70) {
        return "Fair";
    } else if(rssi <= -71 && rssi >= -85) {
        return "Poor";
    } else if(rssi <= -86) {
        return "Unreliable";
    }
    return "";
}

bool err_handle(esp_err_t err){
    switch(err){
        case ESP_ERR_WIFI_NOT_INIT:
            ESP_LOGW("WIFI ERROR", "WiFi was not initialized!");
            return false;
        case ESP_ERR_WIFI_NOT_STARTED:
            ESP_LOGW("WIFI ERROR", "WiFi was not started!");
            return false;
        case ESP_ERR_WIFI_STATE:
            ESP_LOGW("WIFI ERROR", "WiFi is busy connecting!");
            return false;
        case ESP_ERR_WIFI_TIMEOUT:
            ESP_LOGW("WIFI ERROR", "WiFi operation blocked by timeout!");
            return false;
        case ESP_ERR_WIFI_SSID:
            ESP_LOGW("WIFI ERROR", "WiFi operation blocked due to invalid SSID!");
            return false;
        case ESP_ERR_WIFI_PASSWORD:
            ESP_LOGW("WIFI ERROR", "WiFi operation blocked due to invalid password!");
            return false;
        case ESP_ERR_WIFI_IF:
            ESP_LOGW("WIFI ERROR", "WiFi operation failed due to interface error!");
            return false;
        case ESP_ERR_WIFI_CONN:
            ESP_LOGW("WIFI ERROR", "WiFi operation failed due to control block of station or soft-AP error!");
            return false;
        case ESP_OK:
            ESP_LOGW("WIFI ERROR", "WiFi operation was successful!");
            return true;
        default:
            ESP_LOGW("WARN", "WiFi operation failed due to unhandled error '%s'!", esp_err_to_name(err));
            return false;
    }
}

bool search_ssid(const char* ssid) {

    esp_err_t err = esp_wifi_scan_start(NULL, true);
    
    if(!err_handle(err)) return false;

    uint16_t num = 0;

    esp_wifi_scan_get_ap_num(&num);
    ESP_LOGI("INFO", "Number of WiFi APs Found: %d", num);


    wifi_ap_record_t* records = (wifi_ap_record_t*)calloc(num,sizeof(wifi_ap_record_t));

    esp_wifi_scan_get_ap_records(&num, records);
    
    bool found = false;

    if(num > 0){
        ESP_LOGI("INFO", "------------[WIFI SCAN]------------");
        for(int i = 0; i < num; i++){
            ESP_LOGI("INFO", "SSID: %s -> Signal Strength: %s (%d dBm)", records[i].ssid, rssi_to_str(records[i].rssi), records[i].rssi);
            if(strcmp((const char*)records[i].ssid,ssid) == 0) {
                found = true;
            }
        }
        ESP_LOGI("INFO", "-----------------------------------");
    }
    free(records);
    return found;
}

bool connect_to_network(){
    const int SSID_SIZE = 32;
    char ssid[SSID_SIZE];
    printf("Write the SSID you would like to connect to: \n");
    get_input(ssid,SSID_SIZE);

    const int PASS_SIZE = 64;
    char password[PASS_SIZE];
    printf("Write the password: \n");
    get_input(password,PASS_SIZE);

    bool found = search_ssid(ssid);

    if(found){
        ESP_LOGI("INFO","Successfully found the WiFi AP '%s'!", ssid);
        return connect_to_network_creds(ssid,password);
    } else {
        ESP_LOGW("WARN","WiFi AP '%s' was not found!", ssid);
        return false;
    }
}

bool connect_to_network_creds(const char* ssid, const char* password) {
    ESP_LOGI("INFO", "Attempting to connect to the WiFi network...");
    wifi_interface_t interface = WIFI_IF_STA;
    wifi_config_t conf = {};
    strncpy((char*)conf.sta.ssid, ssid, sizeof(conf.sta.ssid));
    strncpy((char*)conf.sta.password, password, sizeof(conf.sta.password));

    ESP_ERROR_CHECK(esp_wifi_set_config(interface, &conf));

    esp_err_t err = esp_wifi_connect();

    if(!err_handle(err)) {
        ESP_LOGW("WARN", "Failed to connect to the WiFi network '%s'!", ssid);
        return false;
    }

    ESP_LOGI("INFO", "Successfully connected to the WiFi network '%s'!", ssid);
    return true;

}

void wifi_init() {
    ESP_ERROR_CHECK(esp_netif_init());

    wifi_init_config_t conf = WIFI_INIT_CONFIG_DEFAULT();

    esp_netif_t* netift = esp_netif_create_default_wifi_sta();

    ESP_ERROR_CHECK(esp_wifi_init(&conf));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));

    ESP_ERROR_CHECK(esp_wifi_start());

    uint8_t mac[6];
    ESP_ERROR_CHECK(esp_netif_get_mac(netift, mac));

    ESP_LOGI("INFO", "ESP32 Interface MAC ADDRESS: %02X:%02X:%02X:%02X:%02X:%02X",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
}