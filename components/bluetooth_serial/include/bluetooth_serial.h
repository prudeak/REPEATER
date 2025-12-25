#ifndef REPESP32_BLIETOOTH_SERIAL
#define REPESP32_BLIETOOTH_SERIAL
#include "freertos/FreeRTOS.h"
#include "esp_log.h"

esp_err_t bluetooth_serial_init();

esp_err_t bluetooth_serial_write(const char * const str, const size_t size);

#endif // REPESP32_BLIETOOTH_SERIAL