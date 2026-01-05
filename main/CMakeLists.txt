
set(PRIV_REQUIRED_COMPONENTS)

list(APPEND PRIV_REQUIRED_COMPONENTS esp_psram)
list(APPEND PRIV_REQUIRED_COMPONENTS recorder)
list(APPEND PRIV_REQUIRED_COMPONENTS i2s_player)
list(APPEND PRIV_REQUIRED_COMPONENTS bluetooth_serial)
list(APPEND PRIV_REQUIRED_COMPONENTS esp_driver_gpio)
list(APPEND PRIV_REQUIRED_COMPONENTS hardware_leds)

idf_component_register(SRCS "app_main.cpp"
                       PRIV_REQUIRES   ${PRIV_REQUIRED_COMPONENTS}
                       INCLUDE_DIRS ".")


