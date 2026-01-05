#ifndef HAOS_CONSOLE_COMPONENT_HEADER
#define HAOS_CONSOLE_COMPONENT_HEADER

#include "freertos/FreeRTOS.h"
#include "esp_log.h"

typedef uint16_t haos_console_output_callback_handle_t;
typedef esp_err_t(*haos_session_output_callback_ptr)(const char * const data, const size_t size);

esp_err_t haos_session_output_callback_register(haos_session_output_callback_ptr, haos_console_output_callback_handle_t *const handle);
esp_err_t haos_session_write(const void *const data, const size_t size);

#endif // HAOS_CONSOLE_COMPONENT_HEADER