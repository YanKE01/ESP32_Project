idf_component_register(SRCS "drv_multiButton.c" "drv_adxl345.c" "drv_sht20.c" "drv_hardware.c" "drv_bh1750.c" "drv_gpio.c"
                    INCLUDE_DIRS "include" 
                    PRIV_REQUIRES drv_comm)
