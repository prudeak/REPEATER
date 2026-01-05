#include "haos_session.h"

class haos_session_t{
    public:
        esp_err_t write(const void *const data, const size_t size);
        esp_err_t output_callback_register(haos_session_output_callback_ptr, haos_console_output_callback_handle_t *const handle);
    private:
        const char * LOG_TAG = "TEST";
        
};
esp_err_t haos_session_t::write(const void *const data, const size_t size){
    
    char strbuf [size + 4];
    memset(strbuf, 0, sizeof(strbuf));
    memcpy(strbuf, data, size);
    ESP_LOGI("HAOS SESSION", "TEST RUN, received: %s", strbuf);
    return ESP_OK;
}

esp_err_t haos_session_t::output_callback_register(haos_session_output_callback_ptr callback_ptr, haos_console_output_callback_handle_t *const handle){
    ESP_LOGI("HAOS SESSION", "CALLBACK REGISTER FUNC CALLED");
    // проверить handle на nullptr перед записью
    return ESP_OK;
}
haos_session_t * haos_default_session_ptr = new haos_session_t();

// C-Warpers
esp_err_t haos_session_output_callback_register(haos_session_output_callback_ptr callback_ptr, haos_console_output_callback_handle_t *const handle){
    haos_default_session_ptr->output_callback_register(callback_ptr, handle);
    return ESP_OK;
};
esp_err_t haos_session_write(const void *const data, const size_t size){
    haos_default_session_ptr->write(data, size);
    return ESP_OK;
};