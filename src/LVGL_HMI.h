#ifndef LVGL_HMI_H
#define LVGL_HMI_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <lvgl.h>
#include <lv_demo.h>
// #include <User_Setup.h>

/* LCD */
static const uint16_t screenWidth  = 480;
static const uint16_t screenHeight = 320;


/* LVGL */
#define BUF_MULT 320 * 40
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ BUF_MULT ];

/* TOUCH */ 
#define TFT_D0   12  // Must use pins in the range 0-31 for the data bus
#define TFT_D1   13  // so a single register write sets/clears all bits

#include <TouchScreen.h>
#define XP 32  // must be an analog pin, use "An" notation!
#define YM 33  // must be an analog pin, use "An" notation!
#define XM TFT_D1   // can be a digital pin
#define YP TFT_D0  // can be a digital pin
#define MINPRESSURE 0
#define MAXPRESSURE 5000


void lvgl_hmi_init();
void lvgl_plot();

#endif /* LVGL_HMI_H */