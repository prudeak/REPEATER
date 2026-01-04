#include "hardware_leds.h"

#include "driver/gpio.h"
#include "gpio_defs.h"

esp_err_t haos_hal_led_init_pins(){
    #ifdef HAOS_HAL_LED_REC_GPIO
        gpio_reset_pin(HAOS_HAL_LED_REC_GPIO);
        gpio_set_level(HAOS_HAL_LED_REC_GPIO, 0);
        gpio_set_direction(HAOS_HAL_LED_REC_GPIO, GPIO_MODE_OUTPUT);
    #endif //HAOS_HAL_LED_REC_GPIO

    #ifdef HAOS_HAL_LED_PTT_GPIO
        gpio_reset_pin(HAOS_HAL_LED_PTT_GPIO);
        gpio_set_level(HAOS_HAL_LED_PTT_GPIO, 0);
        gpio_set_direction(HAOS_HAL_LED_PTT_GPIO, GPIO_MODE_OUTPUT);
    #endif // HAOS_HAL_LED_PTT_GPIO
    
    #ifdef HAOS_HAL_LED_LOW_SIGNAL_GPIO
        gpio_reset_pin(HAOS_HAL_LED_LOW_SIGNAL_GPIO);
        gpio_set_level(HAOS_HAL_LED_LOW_SIGNAL_GPIO, 0);
        gpio_set_direction(HAOS_HAL_LED_LOW_SIGNAL_GPIO, GPIO_MODE_OUTPUT);
    #endif // HAOS_HAL_LED_LOW_SIGNAL_GPIO

    #ifdef HAOS_HAL_LED_REC_ERROR_GPIO
        gpio_reset_pin(HAOS_HAL_LED_REC_ERROR_GPIO);
        gpio_set_level(HAOS_HAL_LED_REC_ERROR_GPIO, 0);
        gpio_set_direction(HAOS_HAL_LED_REC_ERROR_GPIO, GPIO_MODE_OUTPUT);
    #endif // HAOS_HAL_LED_REC_ERROR_GPIO

    #ifdef HAOS_HAL_LED_HIGH_SIGNAL_GPIO
        gpio_reset_pin(HAOS_HAL_LED_HIGH_SIGNAL_GPIO);
        gpio_set_level(HAOS_HAL_LED_HIGH_SIGNAL_GPIO, 0);
        gpio_set_direction(HAOS_HAL_LED_HIGH_SIGNAL_GPIO, GPIO_MODE_OUTPUT);
    #endif // HAOS_HAL_LED_HIGH_SIGNAL_GPIO

    haos_hal_led_rec_ctrl(true);
    haos_hal_led_ptt_ctrl(true);
    haos_hal_led_sig_low_ctrl(true);
    haos_hal_led_rec_err_ctrl(true);
    haos_hal_led_sig_high_ctrl(true);

    vTaskDelay(pdMS_TO_TICKS(500));
    
    haos_hal_led_rec_ctrl(false);
    haos_hal_led_ptt_ctrl(false);
    haos_hal_led_sig_low_ctrl(false);
    haos_hal_led_rec_err_ctrl(false);
    haos_hal_led_sig_high_ctrl(false);
    return ESP_OK;
}

esp_err_t haos_hal_led_rec_ctrl(bool state){
    #ifdef HAOS_HAL_LED_REC_GPIO
        gpio_set_level(HAOS_HAL_LED_REC_GPIO, state);
        return ESP_OK;
    #else // HAOS_HAL_LED_REC_GPIO
        return ESP_FAIL;
    #endif // HAOS_HAL_LED_REC_GPIO    
};

esp_err_t haos_hal_led_ptt_ctrl(bool state){
    #ifdef HAOS_HAL_LED_PTT_GPIO
        gpio_set_level(HAOS_HAL_LED_PTT_GPIO, state);
        return ESP_OK;
    #else // HAOS_HAL_LED_PTT_GPIO
        return ESP_FAIL;
    #endif // HAOS_HAL_LED_PTT_GPIO
};
esp_err_t haos_hal_led_sig_low_ctrl(bool state){
    #ifdef HAOS_HAL_LED_LOW_SIGNAL_GPIO
        gpio_set_level(HAOS_HAL_LED_LOW_SIGNAL_GPIO, state);
        return ESP_OK;
    #else // HAOS_HAL_LED_LOW_SIGNAL_GPIO
        return ESP_FAIL;
    #endif // HAOS_HAL_LED_LOW_SIGNAL_GPIO
};
esp_err_t haos_hal_led_rec_err_ctrl(bool state){
    #ifdef HAOS_HAL_LED_REC_ERROR_GPIO
        gpio_set_level(HAOS_HAL_LED_REC_ERROR_GPIO, state);
        return ESP_OK;
    #else // HAOS_HAL_LED_REC_ERROR_GPIO
        return ESP_FAIL;
    #endif // HAOS_HAL_LED_REC_ERROR_GPIO
};
esp_err_t haos_hal_led_sig_high_ctrl(bool state){
    #ifdef HAOS_HAL_LED_HIGH_SIGNAL_GPIO
        gpio_set_level(HAOS_HAL_LED_HIGH_SIGNAL_GPIO, state);
        return ESP_OK;
    #else // HAOS_HAL_LED_HIGH_SIGNAL_GPIO
        return ESP_FAIL;
    #endif // HAOS_HAL_LED_HIGH_SIGNAL_GPIO
};
