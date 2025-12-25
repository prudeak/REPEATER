#ifndef REPESP32_I2S_PLAYER
#define REPESP32_I2S_PLAYER

#include "freertos/FreeRTOS.h"
#include "esp_log.h"

esp_err_t i2s_play_tone(uint16_t freq, uint16_t time_ms);
esp_err_t i2s_play_raw(const void * const data, const size_t size);

esp_err_t i2s_player_serial_output_callback_register(esp_err_t(*callback_ptr)(const char * const data, const size_t size));
esp_err_t i2s_player_led_control_play_led_callback_register(esp_err_t(*callback_ptr)(bool state));

#endif // REPESP32_I2S_PLAYER