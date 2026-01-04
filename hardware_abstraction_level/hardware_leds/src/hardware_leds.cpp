#include "hardware_leds.h"

#include "driver/gpio.h"
#include "gpio_defs.h"

esp_err_t led_init_pins(){
    gpio_reset_pin(LED_REC_GPIO);
    gpio_set_level(LED_REC_GPIO, 0);
    gpio_set_direction(LED_REC_GPIO, GPIO_MODE_OUTPUT);

    gpio_reset_pin(LED_PTT_GPIO);
    gpio_set_level(LED_PTT_GPIO, 0);
    gpio_set_direction(LED_PTT_GPIO, GPIO_MODE_OUTPUT);
    
    gpio_reset_pin(LED_LOW_SIGNAL_GPIO);
    gpio_set_level(LED_LOW_SIGNAL_GPIO, 0);
    gpio_set_direction(LED_LOW_SIGNAL_GPIO, GPIO_MODE_OUTPUT);

    gpio_reset_pin(LED_MEDIUM_SIGNAL_GPIO);
    gpio_set_level(LED_MEDIUM_SIGNAL_GPIO, 0);
    gpio_set_direction(LED_MEDIUM_SIGNAL_GPIO, GPIO_MODE_OUTPUT);

    gpio_reset_pin(LED_HIGH_SIGNAL_GPIO);
    gpio_set_level(LED_HIGH_SIGNAL_GPIO, 0);
    gpio_set_direction(LED_HIGH_SIGNAL_GPIO, GPIO_MODE_OUTPUT);

    led_rec_ctrl(true);
    led_ptt_ctrl(true);
    led_sig_low_ctrl(true);
    led_sig_mid_ctrl(true);
    led_sig_high_ctrl(true);
    vTaskDelay(pdMS_TO_TICKS(500));
    led_rec_ctrl(false);
    led_ptt_ctrl(false);
    led_sig_low_ctrl(false);
    led_sig_mid_ctrl(false);
    led_sig_high_ctrl(false);
    return ESP_OK;
}

esp_err_t led_rec_ctrl(bool state){
    gpio_set_level(LED_REC_GPIO, state);
    return ESP_OK;
};

esp_err_t led_ptt_ctrl(bool state){
    gpio_set_level(LED_PTT_GPIO, state);
    return ESP_OK;
};
esp_err_t led_sig_low_ctrl(bool state){
    gpio_set_level(LED_LOW_SIGNAL_GPIO, state);
    return ESP_OK;
};
esp_err_t led_sig_mid_ctrl(bool state){
    gpio_set_level(LED_MEDIUM_SIGNAL_GPIO, state);
    return ESP_OK;
};
esp_err_t led_sig_high_ctrl(bool state){
    gpio_set_level(LED_HIGH_SIGNAL_GPIO, state);
    return ESP_OK;
};
