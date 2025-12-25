#ifndef REPESP32_PLAYER
#define REPESP32_PLAYER

#include "freertos/FreeRTOS.h"
#include "esp_log.h"

esp_err_t play_tone(uint16_t freq, uint16_t time_ms);
esp_err_t play_raw(const void * const data, const size_t size);
#endif // REPESP32_PLAYER