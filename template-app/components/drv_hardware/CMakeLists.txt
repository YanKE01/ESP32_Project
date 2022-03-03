idf_component_register(SRCS "drv_adc.c" "drv_multiButton.c" "drv_adxl345.c" "drv_sht20.c" "drv_hardware.c" "drv_bh1750.c" "drv_gpio.c"
                    INCLUDE_DIRS "include" 
                    REQUIRES driver efuse
                    PRIV_REQUIRES drv_comm)
