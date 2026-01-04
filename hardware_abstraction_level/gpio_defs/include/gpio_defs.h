#ifndef HAOS_HAL_GPIO_DEFINES_HEADER
#define HAOS_HAL_GPIO_DEFINES_HEADER


#define HAOS_HAL_ADC_SIGNAL_IN_GPIO       GPIO_NUM_34

#define HAOS_HAL_I2S_BCLK_GPIO            GPIO_NUM_4
#define HAOS_HAL_I2S_WS_GPIO              GPIO_NUM_5
#define HAOS_HAL_I2S_DOUT_GPIO            GPIO_NUM_18

/* WHITE LED Индикатор минимального уровня сигнала, зажигается, когда abs(signal) > treshold */
#define HAOS_HAL_LED_LOW_SIGNAL_GPIO        GPIO_NUM_14         
/* GREEN LED Индикатор записи - зажигается  когда активна запись */
#define HAOS_HAL_LED_REC_GPIO               GPIO_NUM_15
/* RED LED #1 Индикатор перегруза по входу зажигается когда амплитуда сигнала близка к границе диапазона ADC*/          
#define HAOS_HAL_LED_HIGH_SIGNAL_GPIO       GPIO_NUM_13         
/* BLUE LED Индикатор передачи - зажигается вместе с PTT*/
#define HAOS_HAL_LED_PTT_GPIO               GPIO_NUM_32
/* RED LED #2 Индикатор ошибки записи - загорается когда запись невозможна, или когда запись слишком длинная или слишком короткая*/ 
#define HAOS_HAL_LED_REC_ERROR_GPIO         GPIO_NUM_12         

#define HAOS_HAL_SD_MOSI_GPIO            GPIO_NUM_23     //  Как в ХВАРДе
#define HAOS_HAL_SD_MISO_GPIO            GPIO_NUM_19     //  Как в ХВАРДе
#define HAOS_HAL_SD_CS_GPIO              GPIO_NUM_27     //  Как в ХВАРДе
#define HAOS_HAL_SD_SCK_GPIO             GPIO_NUM_18     //  Как в ХВАРДе

#define HAOS_HAL_I2C_SCL_GPIO            GPIO_NUM_22     //  Как в ХВАРДе
#define HAOS_HAL_I2C_SDA_GPIO            GPIO_NUM_21     //  Как в ХВАРДе

#endif  // HAOS_HAL_GPIO_DEFINES_HEADER