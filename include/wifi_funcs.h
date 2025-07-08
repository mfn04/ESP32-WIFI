#ifndef _WIFI_FUNCS_H_
#define _WIFI_FUNCS_H_

#include "esp_log.h"
#include "esp_wifi.h"
#include "string.h"

bool err_handle(esp_err_t err);

bool ssid_found(const char* ssid);

bool connect_to_network(const char* ssid, const char* password);

void wifi_init(void*);

#endif