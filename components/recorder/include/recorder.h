#ifndef REPESP32_RECORDER
#define REPESP32_RECORDER

#include "freertos/FreeRTOS.h"
#include "esp_log.h"
esp_err_t recorder_serial_output_callback_register(esp_err_t(*callback_ptr)(const char * const data, const size_t size));

esp_err_t recorder_led_control_rec_led_callback_register(esp_err_t(*callback_ptr)(bool state));
esp_err_t recorder_led_control_rec_err_led_callback_register(esp_err_t(*callback_ptr)(bool state));

esp_err_t recorder_led_control_sig_low_led_callback_register(esp_err_t(*callback_ptr)(bool state));
esp_err_t recorder_led_control_sig_high_led_callback_register(esp_err_t(*callback_ptr)(bool state));

esp_err_t record_raw(void * const data, const size_t buffer_size, size_t * const record_size);
#endif //REPESP32_RECORDER