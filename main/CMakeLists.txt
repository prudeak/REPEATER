
set(REQUIRED_COMPONENTS)

list(APPEND REQUIRED_COMPONENTS esp_psram)
list(APPEND REQUIRED_COMPONENTS recorder)
list(APPEND REQUIRED_COMPONENTS player)
list(APPEND REQUIRED_COMPONENTS i2s_player)
list(APPEND REQUIRED_COMPONENTS bluetooth_serial)

idf_component_register(SRCS "app_main.cpp"
                       PRIV_REQUIRES   ${REQUIRED_COMPONENTS}
                       INCLUDE_DIRS ".")


