#include "player.h"

#include "driver/dac_cosine.h"
#include "driver/dac_continuous.h"

#define TAG "PLAYER"
#define AUDIO_SAMPLE_RATE 22050

esp_err_t play_tone(uint16_t freq, uint16_t time_ms){
    ESP_LOGI(TAG, "DAC init started");
    dac_cosine_handle_t chan0_handle;
    dac_cosine_config_t cos0_cfg = {
        .chan_id = DAC_CHAN_0,
        .freq_hz = freq, // It will be covered by 8000 in the latter configuration
        .clk_src = DAC_COSINE_CLK_SRC_DEFAULT,
        .atten = DAC_COSINE_ATTEN_DB_6,//DAC_COSINE_ATTEN_DEFAULT,
        .phase = DAC_COSINE_PHASE_0,
        .offset = 0,
        .flags = {
            .force_set_freq = false,
        },
    };
    ESP_ERROR_CHECK(dac_cosine_new_channel(&cos0_cfg, &chan0_handle));
    ESP_ERROR_CHECK(dac_cosine_start(chan0_handle));
    vTaskDelay(pdMS_TO_TICKS(time_ms));
    dac_cosine_stop(chan0_handle);
    dac_cosine_del_channel(chan0_handle);
    return ESP_OK;
};

esp_err_t play_raw(const void * const data, const size_t size){
    dac_continuous_handle_t dac_handle;
      dac_continuous_config_t cont_cfg = {
        .chan_mask = DAC_CHANNEL_MASK_CH0,//DAC_CHANNEL_MASK_ALL,
        .desc_num = 4,
        .buf_size = 2048,
        .freq_hz = AUDIO_SAMPLE_RATE,
        .offset = 0,
        .clk_src = DAC_DIGI_CLK_SRC_APLL,   // Using APLL as clock source to get a wider frequency range
        /* Assume the data in buffer is 'A B C D E F'
         * DAC_CHANNEL_MODE_SIMUL:
         *      - channel 0: A B C D E F
         *      - channel 1: A B C D E F
         * DAC_CHANNEL_MODE_ALTER:
         *      - channel 0: A C E
         *      - channel 1: B D F
         */
        .chan_mode = DAC_CHANNEL_MODE_SIMUL,
    };
    ESP_ERROR_CHECK(dac_continuous_new_channels(&cont_cfg, &dac_handle));
    ESP_ERROR_CHECK(dac_continuous_enable(dac_handle));
    ESP_LOGI(TAG, "DAC initialized success, DAC DMA is ready");
    //dac_write_data_synchronously(dac_handle, (uint8_t *)data, size);
    ESP_LOGI(TAG, "Audio size %d bytes, played at frequency %d Hz synchronously", size, AUDIO_SAMPLE_RATE);
    ESP_ERROR_CHECK(dac_continuous_write(dac_handle, (uint8_t *)data, size, NULL, -1));

    dac_continuous_disable(dac_handle);
    dac_continuous_del_channels(dac_handle);
    
    return ESP_OK;
}