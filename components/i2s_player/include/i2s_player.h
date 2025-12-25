#ifndef REPESP32_I2S_PLAYER
#define REPESP32_I2S_PLAYER

#include "freertos/FreeRTOS.h"
#include "esp_log.h"

esp_err_t i2s_play_tone(uint16_t freq, uint16_t time_ms);
esp_err_t i2s_play_raw(const void * const data, const size_t size);

#endif // REPESP32_I2S_PLAYER