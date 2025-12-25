#include "i2s_player.h"
#include "driver/i2s_std.h"
#include "driver/gpio.h"
#include <cmath>
#define TAG "I2C-PLAYER"
#define AUDIO_SAMPLE_RATE 22050

static esp_err_t (*led_control_play_led_callback_ptr)(bool state) = nullptr;
static esp_err_t(*serial_output_callback_ptr)(const char * const str, const size_t size) = nullptr;

esp_err_t i2s_player_serial_output_callback_register(esp_err_t(*callback_ptr)(const char * const data, const size_t size)){
    serial_output_callback_ptr = callback_ptr;
    return ESP_OK;
};
esp_err_t i2s_player_led_control_play_led_callback_register(esp_err_t(*callback_ptr)(bool state)){
    led_control_play_led_callback_ptr = callback_ptr;
    return ESP_OK;
};

esp_err_t led_play_set(bool state){
    if (led_control_play_led_callback_ptr != nullptr){
        return (*led_control_play_led_callback_ptr)(state);
    }else{
        return ESP_FAIL;
    }  
}

esp_err_t i2s_palyer_serial_out_write(const char * const data, const size_t size){
    if (serial_output_callback_ptr != nullptr){
        return (*serial_output_callback_ptr)(data, size);
    }else{
        return ESP_FAIL;
    }
}

i2s_chan_handle_t i2s_dac_init();
esp_err_t i2s_dac_deinit(i2s_chan_handle_t handle);

esp_err_t i2s_play_tone(uint16_t freq, uint16_t time_ms){
    ESP_LOGI(TAG, "PLAY_TONE");
    char text_output_buf[255];
    size_t len = sprintf(text_output_buf, "PLAYING TONE %u Hz, %ums\n", freq, time_ms);
    i2s_palyer_serial_out_write(text_output_buf, len);
    // Заполняем буфер синусоидой
    const uint16_t samples_per_period = AUDIO_SAMPLE_RATE / freq;
    int16_t audio_buf [samples_per_period];
    const double PI = 3.141592653589793;
    for(uint16_t i = 0; i < samples_per_period; i++){
        double phase = i * 2 * PI / samples_per_period;
        int16_t val = 0x3fff * std::cos(phase);
        audio_buf[i] = val;
    }
    i2s_chan_handle_t tx_chan_handle = i2s_dac_init();
    for (uint32_t i=0; i < (freq * time_ms / 1000); i++){
        size_t bytes_written = 0;
        i2s_channel_write(tx_chan_handle,  audio_buf, sizeof(audio_buf), &bytes_written, 300); 
    }
    i2s_dac_deinit(tx_chan_handle);
    ESP_LOGI(TAG, "PLAY_TONE_FINISHED");
    return ESP_OK;
};
esp_err_t i2s_play_raw(const void * const data, const size_t size){
    ESP_LOGI(TAG, "PLAY_RAW");
    //char text_output_buf[255];
    //size_t len = sprintf(text_output_buf, "PLAYING RAW %u bytes (%u samples)\n", size, size / 2);
    //i2s_palyer_serial_out_write(text_output_buf, len);
    led_play_set(true);
    vTaskDelay(pdMS_TO_TICKS(500));
    i2s_play_tone(1200, 500);
    vTaskDelay(pdMS_TO_TICKS(300));
    i2s_chan_handle_t tx_chan_handle = i2s_dac_init();
    size_t sent_bytes = 0;
    i2s_channel_write(tx_chan_handle, data, size, &sent_bytes, portMAX_DELAY);
    ESP_LOGI(TAG, "SENT %d bytes", sent_bytes);
    vTaskDelay(pdMS_TO_TICKS(300));
    i2s_dac_deinit(tx_chan_handle);
    
    vTaskDelay(pdMS_TO_TICKS(500));
    i2s_play_tone(1400, 500);
    led_play_set(false);
    ESP_LOGI(TAG, "PLAY_RAW_FINISHED");
    return ESP_OK;
};

i2s_chan_handle_t i2s_dac_init(){
    i2s_chan_handle_t tx_chan_handle;
    i2s_chan_config_t tx_chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);
    ESP_ERROR_CHECK(i2s_new_channel(&tx_chan_cfg, &tx_chan_handle, NULL));
    i2s_std_config_t tx_std_cfg = {
        .clk_cfg  = I2S_STD_CLK_DEFAULT_CONFIG(AUDIO_SAMPLE_RATE),
        .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO),
        //.slot_cfg = I2S_STD_PHILIP_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO),
        //.slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO),
        //.slot_cfg = I2S_STD_PCM_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO),
        .gpio_cfg = {
            .mclk = I2S_GPIO_UNUSED,    // some codecs may require mclk signal, this example doesn't need it
            .bclk = GPIO_NUM_4,
            .ws   = GPIO_NUM_5,
            .dout = GPIO_NUM_18,
            .din  = I2S_GPIO_UNUSED,
            .invert_flags = {
                .mclk_inv = false,
                .bclk_inv = false,
                .ws_inv   = false,
            },
        },
    };

    ESP_ERROR_CHECK(i2s_channel_init_std_mode(tx_chan_handle, &tx_std_cfg));
    i2s_channel_enable(tx_chan_handle);
    return tx_chan_handle;;
};
esp_err_t i2s_dac_deinit(i2s_chan_handle_t handle){
    i2s_channel_disable(handle);
    i2s_del_channel(handle);
    return ESP_OK;
};