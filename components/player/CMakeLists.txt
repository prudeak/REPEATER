cmake_minimum_required(VERSION 3.16)
set(SOURCE_DIR "./src")

file(GLOB_RECURSE COMPONENT_SOURCE_FILES 
                    "${SOURCE_DIR}/*.cpp"
                    "${SOURCE_DIR}/*.c")

set(PRIV_REQUIRED_COMPONENTS)
list(APPEND PRIV_REQUIRED_COMPONENTS esp_driver_dac)

set(INCLUDE_DIR "./include")
idf_component_register( SRCS ${COMPONENT_SOURCE_FILES}
                        PRIV_REQUIRES   ${PRIV_REQUIRED_COMPONENTS}
                        INCLUDE_DIRS ${INCLUDE_DIR})
