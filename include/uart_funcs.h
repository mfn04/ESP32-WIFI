#ifndef _UART_FUNCS_H_
#define _UART_FUNCS_H_

#include "driver/uart.h"

void get_input(char buff[], size_t buff_size);

void uart_init(void);

#endif