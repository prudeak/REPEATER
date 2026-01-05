#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "esp_chip_info.h"

#include "esp_log.h"

#include "hardware_leds.h"
#include "recorder.h"
#include "i2s_player.h"
#include "bluetooth_serial.h"
#include "haos_session.h"

#define AUDIO_SAMPLE_RATE 22050
#define AUDIO_MESSAGE_LENGTH_SEC 15
#define AUDIO_BYTES_PER_SAMPLE 2 

extern "C"{void app_main(void);}
void app_main(void)
{
    const char * TAG = "ESP-REP";
    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), %s%s%s%s \n",
           CONFIG_IDF_TARGET,
           chip_info.cores,
           (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
           (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "",
           (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");

    const size_t audio_buffer_size = AUDIO_SAMPLE_RATE * AUDIO_MESSAGE_LENGTH_SEC * AUDIO_BYTES_PER_SAMPLE;

    void * const audio_buffer_ptr = heap_caps_malloc(audio_buffer_size, MALLOC_CAP_SPIRAM);

    if (audio_buffer_ptr != nullptr) ESP_LOGI(TAG, "audio buffer of %u bytes successfully alocated", audio_buffer_size);
    else ESP_LOGE(TAG, "audio buffer allocation failed");

    bluetooth_serial_init();
    haos_hal_led_init_pins();

    haos_session_output_callback_register(&bluetooth_serial_write, nullptr);
    const char * session_test = "\nSESSOIN_TEST\n\n";
    haos_session_write(session_test, strlen(session_test));

    recorder_serial_output_callback_register(&bluetooth_serial_write);
    
    recorder_led_control_rec_led_callback_register(&haos_hal_led_rec_ctrl);
    recorder_led_control_sig_low_led_callback_register(&haos_hal_led_sig_low_ctrl);
    recorder_led_control_rec_err_led_callback_register(&haos_hal_led_rec_err_ctrl);
    recorder_led_control_sig_high_led_callback_register(&haos_hal_led_sig_high_ctrl);

    i2s_player_serial_output_callback_register(&bluetooth_serial_write);
    i2s_player_led_control_play_led_callback_register(&haos_hal_led_ptt_ctrl);

    i2s_play_tone(1000, 180);
    i2s_play_tone(1200, 180);
    i2s_play_tone(1400, 180);
    i2s_play_tone(1000, 220);

    
    while(1){
        size_t recorded_audio_size = 0;
        esp_err_t ret = record_raw(audio_buffer_ptr, audio_buffer_size, &recorded_audio_size);
        if (recorded_audio_size / (AUDIO_BYTES_PER_SAMPLE * AUDIO_SAMPLE_RATE) < 3) {
            ESP_LOGI(TAG, "RECORD is too short, DROPPED");
            haos_hal_led_rec_err_ctrl(true);
            vTaskDelay(pdMS_TO_TICKS(500));
            haos_hal_led_rec_err_ctrl(false);
            continue;
        }
        if (ret == ESP_ERR_NOT_FINISHED){
            ESP_LOGI(TAG, "RECORD is too long, NOT_FINISHED. DROPPED");
            haos_hal_led_rec_err_ctrl(true);
            vTaskDelay(pdMS_TO_TICKS(500));
            haos_hal_led_rec_err_ctrl(false);
            continue;
        }
        
        i2s_play_raw(audio_buffer_ptr, recorded_audio_size);
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
        
}

