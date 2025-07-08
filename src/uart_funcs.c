#include "uart_funcs.h"

void get_input(char buff[], size_t buff_size){
    char c;
    int err;
    int index = 0;
    while(1){
        if ((err = uart_read_bytes(UART_NUM_0, &c, sizeof(char), pdMS_TO_TICKS(100))) > 0) {
            if( (c != '\n') && index < (buff_size-1) ) {
                buff[index++] = c;
            } else {
                buff[index] = '\0';
                break;
            }
        } else if ( err < 0 ) {
            break;
        }
    }
}

void uart_init(void){

    QueueHandle_t queue_handler;

    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_0, 1024, 1024, 10, &queue_handler, 0));

    uart_config_t conf = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT
    };

    uart_param_config(UART_NUM_0, &conf);

}