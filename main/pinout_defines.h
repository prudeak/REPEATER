
#define ADC_SIGNAL_IN_GPIO       GPIO_NUM_34

#define I2S_BCLK_GPIO            GPIO_NUM_4
#define I2S_WS_GPIO              GPIO_NUM_5
#define I2S_DOUT_GPIO            GPIO_NUM_18


#define LED_LOW_SIGNAL_GPIO      GPIO_NUM_14
#define LED_MEDIUM_SIGNAL_GPIO   GPIO_NUM_12
#define LED_HIGH_SIGNAL_GPIO     GPIO_NUM_13

#define LED_REC_GPIO             GPIO_NUM_27     // !!! Заменить на GPIO_NUM_15, перепаять!!! //!!!!! GREEN LED
//#define LED_PLAY_GPIO            GPIO_NUM_15   
#define LED_PTT_GPIO             GPIO_NUM_32    // RED LED

#define SD_MOSI_GPIO            GPIO_NUM_23     //  Как в ХВАРДе
#define SD_MISO_GPIO            GPIO_NUM_19     //  Как в ХВАРДе
#define SD_CS_GPIO              GPIO_NUM_27     //  Как в ХВАРДе
#define SD_SCK_GPIO             GPIO_NUM_18     //  Как в ХВАРДе

#define I2C_SCL_GPIO            GPIO_NUM_22     //  Как в ХВАРДе
#define I2C_SDA_GPIO            GPIO_NUM_21     //  Как в ХВАРДе

esp_err_t led_init_pins();

esp_err_t led_rec_ctrl(bool state);
esp_err_t led_ptt_ctrl(bool state);
esp_err_t led_sig_low_ctrl(bool state);
esp_err_t led_sig_mid_ctrl(bool state);
esp_err_t led_sig_high_ctrl(bool state);