#ifndef _WIFI_FUNCS_H_
#define _WIFI_FUNCS_H_

#include "esp_log.h"
#include "esp_wifi.h"
#include "string.h"
#include "uart_funcs.h"

bool err_handle(esp_err_t err);

bool search_ssid(const char* ssid);

bool connect_to_network(void);

bool connect_to_network_creds(const char* ssid, const char* password);

const char* rrsi_to_str(int rssi);

void wifi_init(void);

#endif