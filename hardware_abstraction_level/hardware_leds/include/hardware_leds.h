#ifndef HAOS_HAL_HARDWARE_LEDS_HEADER
#define HAOS_HAL_HARDWARE_LEDS_HEADER

#include "freertos/FreeRTOS.h"
#include "esp_log.h"

/* 
 * @brief Инициализация пинов GPIO, которые используются для LED
 * @return
 *     - ESP_OK Success
*/
esp_err_t haos_hal_led_init_pins();

/* 
 * @brief Управляет индикацией записи, когда запись активна - горит ЗЕЛЕНЫЙ светодиод
 * @return
 *     - ESP_OK Если GPIO светодиода определен
 *     - ESP_FAIL Если GPIO светодиода не определен
*/
esp_err_t haos_hal_led_rec_ctrl(bool state);

/* 
 * @brief Управляет индикацией ошибки записи, когда запись невозможна,
 * слишком длинная или слишком короткая - горит КРАСНЫЙ светодиод №2
 * @return
 *     - ESP_OK Если GPIO светодиода определен
 *     - ESP_FAIL Если GPIO светодиода не определен
*/
esp_err_t haos_hal_led_rec_err_ctrl(bool state);
/* 
 * @brief Управляет индикацией передачи, когда передача активна - горит СИНИЙ светодиод.
 * Этот же GPIO управляет собственно кнопкой PTT через оптопары
 * @return
 *     - ESP_OK Если GPIO светодиода определен
 *     - ESP_FAIL Если GPIO светодиода не определен
*/
esp_err_t haos_hal_led_ptt_ctrl(bool state);
/* 
 * @brief Управляет индикацией минимального уровня сигнала, когда в наборе сэмплов найден хоть один семпл выше
 * treshold - горит БЕЛЫЙ светодиод
 * @return
 *     - ESP_OK Если GPIO светодиода определен
 *     - ESP_FAIL Если GPIO светодиода не определен
*/
esp_err_t haos_hal_led_sig_low_ctrl(bool state);
/* 
 * @brief Управляет индикацией максимального уровня сигнала, когда в наборе сэмплов найдены сэмплы,
 * близкие к пределам измерения АЦП - горит КРАСНЫЙ светодиод №1
 * @return
 *     - ESP_OK Если GPIO светодиода определен
 *     - ESP_FAIL Если GPIO светодиода не определен
*/
esp_err_t haos_hal_led_sig_high_ctrl(bool state);

#endif  // HAOS_HAL_HARDWARE_LEDS_HEADER