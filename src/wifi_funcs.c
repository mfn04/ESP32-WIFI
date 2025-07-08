#include "wifi_funcs.h"

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

bool ssid_found(const char* ssid) {

    esp_err_t err = esp_wifi_scan_start(NULL, true);
    
    if(!err_handle(err)) return false;

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

bool connect_to_network(const char* ssid, const char* password) {
    wifi_interface_t interface = WIFI_IF_STA;
    wifi_config_t conf = {};
    strncpy((char*)conf.sta.ssid,ssid,sizeof(conf.sta.ssid));
    strncpy((char*)conf.sta.password,password,sizeof(conf.sta.password));

    ESP_ERROR_CHECK(esp_wifi_set_config(interface, &conf));

    esp_err_t err = esp_wifi_connect();

    if(!err_handle(err)) return false;

    ESP_LOGI("INFO", "Successfully connected to the WiFi network '%s'!", ssid);
    return true;

}

void wifi_init(void*) {
    wifi_init_config_t conf = WIFI_INIT_CONFIG_DEFAULT();

    esp_netif_t* netift = esp_netif_create_default_wifi_sta();

    esp_wifi_init(&conf);

    esp_wifi_set_mode(WIFI_MODE_APSTA);

    esp_wifi_start();

    uint8_t mac[6] = {0};
    ESP_ERROR_CHECK(esp_netif_get_mac(netift, mac));

    ESP_LOGI("INFO", "esp's MAC ADDRESS: %s",mac);

    char ssid[32];
    printf("Write the SSID you would like to connect to: \n");
    fgets(ssid,sizeof(ssid), stdin);

    char password[64];
    printf("Write the password: \n");
    fgets(password,sizeof(password), stdin);

    bool found = ssid_found(ssid);

    if(found){
        ESP_LOGI("INFO","Successfully found the WiFi AP '%s'!", ssid);
        connect_to_network(ssid,password);
    } else {
        ESP_LOGI("INFO","WiFi AP '%s' was not found!", ssid);
    }

    while(true){
        ESP_LOGI("WIFI_TASK","I AM PINGING!!!");
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}