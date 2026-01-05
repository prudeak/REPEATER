#include "recorder.h"

#include "esp_adc/adc_continuous.h"

#define TAG "RECORDER"
#define AUDIO_SAMPLE_RATE 26900 //27000 //22050*1.22 потому что частота дискретизации АЦП считается неправильно
#define ADC_BUF_READ_LEN 2048
/* ЗЕЛЕНЫЙ светодиод "ЗАПИСЬ"*/
static esp_err_t (*led_control_rec_led_callback_ptr)(bool state) = nullptr;
/* БЕЛЫЙ светодиод "МИНИМАЛЬНЫЙ УРОВЕНЬ СИГНАЛА"*/
static esp_err_t (*led_control_sig_low_led_callback_ptr)(bool state) = nullptr;
/* КРАСНЫЙ светодиод №2 "ОШИБКА ЗАПИСИ" - горит, когда уплыл ноль*/
static esp_err_t (*led_control_rec_err_led_callback_ptr)(bool state) = nullptr;
/* КРАСНЫЙ светодиод №1 "МАКСИМАЛЬНЫЙ УРОВЕНЬ СИГНАЛА - ПЕРЕГРУЗ" */
static esp_err_t (*led_control_sig_high_led_callback_ptr)(bool state) = nullptr;

static esp_err_t(*serial_output_callback_ptr)(const char * const str, const size_t size) = nullptr;

esp_err_t readout_logger(   const char *const mode,
                            const uint32_t samples_num, 
                            const int16_t max_sample_val,
                            const int16_t min_sample_val,
                            const uint32_t overtreshold_samples_count,
                            const int16_t adc_zero
                        );

esp_err_t recorder_serial_output_callback_register(esp_err_t(*callback_ptr)(const char * const data, const size_t size)){
    serial_output_callback_ptr = callback_ptr;
    return ESP_OK;
}

esp_err_t recorder_led_control_rec_led_callback_register(esp_err_t(*callback_ptr)(bool state)){
    led_control_rec_led_callback_ptr = callback_ptr;
    return ESP_OK;
};
esp_err_t recorder_led_control_sig_low_led_callback_register(esp_err_t(*callback_ptr)(bool state)){
    led_control_sig_low_led_callback_ptr = callback_ptr;
    return ESP_OK;
};
esp_err_t recorder_led_control_rec_err_led_callback_register(esp_err_t(*callback_ptr)(bool state)){
    led_control_rec_err_led_callback_ptr = callback_ptr;
    return ESP_OK;
};
esp_err_t recorder_led_control_sig_high_led_callback_register(esp_err_t(*callback_ptr)(bool state)){
    led_control_sig_high_led_callback_ptr = callback_ptr;
    return ESP_OK;
};

esp_err_t led_rec_set(bool state){
    if (led_control_rec_led_callback_ptr != nullptr){
        return (*led_control_rec_led_callback_ptr)(state);
    }else{
        return ESP_FAIL;
    }  
}

esp_err_t led_sig_low_set(bool state){
    if (led_control_sig_low_led_callback_ptr != nullptr){
        return (*led_control_sig_low_led_callback_ptr)(state);
    }else{
        return ESP_FAIL;
    }  
}

esp_err_t led_rec_err_set(bool state){
    if (led_control_rec_err_led_callback_ptr != nullptr){
        return (*led_control_rec_err_led_callback_ptr)(state);
    }else{
        return ESP_FAIL;
    }  
}

esp_err_t led_sig_high_set(bool state){
    if (led_control_sig_high_led_callback_ptr != nullptr){
        return (*led_control_sig_high_led_callback_ptr)(state);
    }else{
        return ESP_FAIL;
    }  
}

esp_err_t serial_out_write(const char * const data, const size_t size){
    if (serial_output_callback_ptr != nullptr){
        return (*serial_output_callback_ptr)(data, size);
    }else{
        return ESP_FAIL;
    }
}

esp_err_t serial_print(const char *const message){
    if(serial_output_callback_ptr != nullptr){
        return (*serial_output_callback_ptr)(message, strlen(message));
    }else{
        return ESP_FAIL;
    }
}

esp_err_t serial_println(const char *const message){
    if (serial_output_callback_ptr != nullptr){
        esp_err_t ret = (*serial_output_callback_ptr)(message, strlen(message));
        if (ret == ESP_OK) ret = (*serial_output_callback_ptr)("\n", 1);
        return ret;
    }else{
        return ESP_FAIL;
    }
}

esp_err_t record_raw(void * const output_data, const size_t buffer_size, size_t * const record_size){
    char text_output_buffer [255] = {0};
    ESP_LOGI(TAG, "INIT ADC");
    serial_println("INIT ADC");
    esp_log_level_set(TAG, ESP_LOG_VERBOSE);
    esp_log_level_set("REC", ESP_LOG_VERBOSE);
    esp_log_level_set("WAIT", ESP_LOG_VERBOSE);
    adc_continuous_handle_t adc_handle = NULL;
    adc_continuous_handle_cfg_t adc_config = {
        .max_store_buf_size = 2048,
        .conv_frame_size = 512,
        .flags = {
            .flush_pool = 1
        }

    };
    ESP_ERROR_CHECK(adc_continuous_new_handle(&adc_config, &adc_handle));
    adc_digi_pattern_config_t adc_pattern[SOC_ADC_PATT_LEN_MAX] = {0, 0, 0, 0};
        adc_pattern[0].atten = ADC_ATTEN_DB_12;
        adc_pattern[0].channel = ADC_CHANNEL_6;
        adc_pattern[0].unit = ADC_UNIT_1;
        adc_pattern[0].bit_width = SOC_ADC_DIGI_MAX_BITWIDTH;
    adc_continuous_config_t dig_cfg = {
        .pattern_num = 1,
        .adc_pattern = adc_pattern,
        .sample_freq_hz = AUDIO_SAMPLE_RATE,
        .conv_mode = ADC_CONV_SINGLE_UNIT_1,
        .format = ADC_DIGI_OUTPUT_FORMAT_TYPE1,
    };
    ESP_ERROR_CHECK(adc_continuous_config(adc_handle, &dig_cfg));

    ESP_ERROR_CHECK(adc_continuous_start(adc_handle));
    ESP_LOGI(TAG, "WAITING");
    serial_println("WAITING");
    uint32_t ret_num = 0;
    uint8_t adc_readout[ADC_BUF_READ_LEN] = {0};
    esp_err_t ret = ESP_OK;
    
    const uint8_t BIT_SHIFT = 3; //to convert 12bit unsigned int to 16bit signed int
    const uint16_t TRESHOLD = 600; // Measured value
    const uint8_t SAMPLE_SIZE = 2; // final sample size in audio buffer, bytes
    const uint32_t MIN_OVERTRESHOLD_SAMPLES_COUNT = 50; // Минимальное количество семплов выше TRESHOLD за выборку для начала записи
    size_t sample_counter = 0;
    uint16_t silent_readouts_count = 0;
    bool record_enabled = false;
    // Измеряем ноль перед циклом прослушивания / записи
    ESP_LOGI(TAG, "Measuring ADC zero point...");
    serial_println("Measuring ADC zero point...");
    uint16_t adc_zero = 2048;
    {
        uint32_t adc_accu = 0;
        ret = adc_continuous_read(adc_handle, adc_readout, ADC_BUF_READ_LEN, &ret_num, 100);
        for (uint16_t i=0; i < ret_num; i+= SOC_ADC_DIGI_RESULT_BYTES){
            adc_digi_output_data_t *p = (adc_digi_output_data_t*)&adc_readout[i];
            adc_accu += p->type1.data;   
        }
        adc_zero = adc_accu / (ret_num / SOC_ADC_DIGI_RESULT_BYTES);
    }
    ESP_LOGI(TAG, "ADC Zero is %d (%.2f V)", adc_zero, ((float)adc_zero * 3.3 / 4096));
    sprintf(text_output_buffer, "ADC Zero is %d (%.2f V)", adc_zero, ((float)adc_zero * 3.3 / 4096));
    serial_println(text_output_buffer);

    while(1){
        vTaskDelay(pdMS_TO_TICKS(30));
        ret = adc_continuous_read(adc_handle, adc_readout, ADC_BUF_READ_LEN, &ret_num, 100);
        if (ret == ESP_OK) {
            uint32_t adc_accu = 0;
            int16_t max_sample_val = 0;
            int16_t min_sample_val = 0;
            int32_t overtreshold_samples_count = 0;
            for (uint16_t i=0; i < ret_num; i+= SOC_ADC_DIGI_RESULT_BYTES){
                adc_digi_output_data_t *p = (adc_digi_output_data_t*)&adc_readout[i];
                adc_accu += p->type1.data;
                const int16_t sample_val = ((p->type1.data)<<3) - (adc_zero<<3);
                if (i==0){
                    max_sample_val = sample_val;
                    min_sample_val = sample_val;    
                };
                if (sample_val > max_sample_val) max_sample_val = sample_val;
                if (sample_val < min_sample_val) min_sample_val = sample_val;
                if (abs(sample_val)>TRESHOLD) overtreshold_samples_count ++;
                *((int16_t*)output_data + sample_counter) = sample_val;
                sample_counter++;
                if ((sample_counter * SAMPLE_SIZE)>=buffer_size) break;
            }
            adc_zero = adc_accu / (ret_num / SOC_ADC_DIGI_RESULT_BYTES);

            if ((sample_counter * SAMPLE_SIZE)>=buffer_size) {
                led_rec_err_set(false);
                led_rec_set(false);
                led_sig_low_set(false);
                led_sig_high_set(false);
                ret = ESP_ERR_NOT_FINISHED;
                break; // Буфер заполнен - выходим из цикла записи
            }
            if (record_enabled){ // Если запись ИДЕТ (RECORD_ENABLED == TRUE)
                readout_logger("REC", ret_num, max_sample_val, min_sample_val, overtreshold_samples_count, adc_zero);

                if (overtreshold_samples_count < MIN_OVERTRESHOLD_SAMPLES_COUNT) silent_readouts_count ++;
                else silent_readouts_count = 0;
                if (silent_readouts_count > 50) {
                    ret = ESP_OK;
                    led_rec_set(false);
                    led_rec_err_set(false);
                    led_sig_low_set(false);
                    led_sig_high_set(false);
                    break; // насчитали 30 пустых ридоутов - заканчиваем запись
                }
                led_rec_set(true);
            }else{ // Если запись НЕ ИДЕТ (RECORD_ENABLED == FALSE)
                readout_logger("WAIT", ret_num, max_sample_val, min_sample_val, overtreshold_samples_count, adc_zero);
                if (overtreshold_samples_count > MIN_OVERTRESHOLD_SAMPLES_COUNT) {
                    record_enabled = true; // Включаем запись
                    ESP_LOGI(TAG, "RECORD_STARTED");
                    serial_println("RECORD STARTED");    
                }
                else sample_counter = 0; // Перезаписываем буфер сначала
                led_rec_set(false); 
            }
            led_sig_low_set(overtreshold_samples_count > 3);
            led_sig_high_set((max_sample_val > 14000)&&(min_sample_val < -14000));
            led_rec_err_set(false);

        }else if (ret == ESP_ERR_TIMEOUT) {
            led_sig_low_set(false);
            led_sig_high_set(false);
            led_rec_err_set(true);
            ESP_LOGW(TAG, "NO_DATA_FROM ADC");
            serial_println("NO DATA FROM ADC");
        }
    }
    ESP_LOGI(TAG, "RECORD DONE, recorded %d samples", sample_counter);
    sprintf(text_output_buffer, "RECORD DONE, recorded %d samples", sample_counter);
    serial_println(text_output_buffer);
    ESP_ERROR_CHECK(adc_continuous_stop(adc_handle));
    ESP_ERROR_CHECK(adc_continuous_deinit(adc_handle));
    *record_size = sample_counter * SAMPLE_SIZE;
    return ret;
};

esp_err_t readout_logger(   const char *const mode,
                            const uint32_t samples_num, 
                            const int16_t max_sample_val,
                            const int16_t min_sample_val,
                            const uint32_t overtreshold_samples_count,
                            const int16_t adc_zero
                        ){
    char text_output_buffer [255] = {0};
    snprintf(   text_output_buffer,
                sizeof(text_output_buffer), 
                "%s %4ld: %+7d, %+7d, %5ld, BIAS:%5d(%.2fV)",
                mode,
                samples_num, 
                max_sample_val, 
                min_sample_val, 
                overtreshold_samples_count,
                adc_zero,
                ((float)adc_zero * 3.3 / 4096)
                );
    serial_println(text_output_buffer);
    ESP_LOGV(mode, "%4d: %+7d, %+7d, %5d, BIAS:%5d (%.2fV)",
                samples_num, 
                max_sample_val, 
                min_sample_val, 
                overtreshold_samples_count, 
                adc_zero,
                ((float)adc_zero * 3.3 / 4096));
    return ESP_OK;

}

