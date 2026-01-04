#ifndef HAOS_HAL_HARDWARE_LEDS_HEADER
#define HAOS_HAL_HARDWARE_LEDS_HEADER

#include "freertos/FreeRTOS.h"
#include "esp_log.h"


esp_err_t led_init_pins();

esp_err_t led_rec_ctrl(bool state);
esp_err_t led_ptt_ctrl(bool state);
esp_err_t led_sig_low_ctrl(bool state);
esp_err_t led_sig_mid_ctrl(bool state);
esp_err_t led_sig_high_ctrl(bool state);

#endif  // HAOS_HAL_HARDWARE_LEDS_HEADER