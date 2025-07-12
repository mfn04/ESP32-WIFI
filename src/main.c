#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "stdbool.h"
#include "string.h"
#include "wifi_funcs.h"
#include "driver/uart.h"

void app_main() {

    ESP_ERROR_CHECK(nvs_flash_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());

    uart_init();

    wifi_init();

    bool connected = false;
    do{
        connected = connect_to_network();
    } while(!connected);

    //TaskHandle_t wifi_handler = NULL;

    //xTaskCreate(wifi_init,"wifi_task",4096,NULL,tskIDLE_PRIORITY,&wifi_handler);

    while(true){
        ESP_LOGI("APP_MAIN","I AM MAIN FUNCTION!");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    return;
}