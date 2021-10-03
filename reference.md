inline void TFT_eSPI::begin_tft_write(void)
inline void TFT_eSPI::end_tft_write(void)
SET_BUS_WRITE_MODE
SET_BUS_READ_MODE

  #define tft_Write_8(C)  i2s_lcd_write(dev->i2s_lcd_handle, C, 1); WR_H
   #define CONSTRUCTOR_INIT_TFT_DATA_BUS







#elif defined (TFT_I2S_PARALLEL_8_BIT)