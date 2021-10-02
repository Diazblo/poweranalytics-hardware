#include "LVGL_HMI.h"

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 0);
TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */

lv_color_t *color_p;
lv_area_t area;
lv_disp_drv_t *disp;

TaskHandle_t Task1;
void codeForTask1(void * parameter){
  for(;;){
      lv_timer_handler();
      vTaskDelay(1);
    }
}
TaskHandle_t Task2;
volatile bool newdata = 0;
void codeForTask2(void * parameter){
    for(;;){
        if(newdata){
            uint32_t w = ( area.x2 - area.x1 + 1 );
            uint32_t h = ( area.y2 - area.y1 + 1 );
            tft.startWrite();
            tft.setAddrWindow( area.x1, area.y1, w, h );
            tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
            tft.endWrite();
            newdata = 0;
            lv_disp_flush_ready( disp );
        }
        vTaskDelay(1);
    }
}

/* Display flushing */
void my_disp_flush( lv_disp_drv_t *dispp, const lv_area_t *areaa, lv_color_t *color_pp )
{   
    if(newdata == 0){
        area = *areaa;
        color_p = color_pp;
        disp = dispp;
        newdata = 1;
        // codeForTask2();
        // tft.startWrite();
        // tft.setAddrWindow( area->x1, area->y1, w, h );
        // tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
        // tft.endWrite();

        // lv_disp_flush_ready( disp );
    }
}

/*Read the touchpad*/
void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
#if 0
    uint16_t touchX, touchY;

    // bool touched = tft.getTouch( &touchX, &touchY, 600 );

    TSPoint touchpad = ts.getPoint();
    pinMode(XP, OUTPUT);
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    pinMode(YM, OUTPUT);
    touchX = map(touchpad.x, 990, 160, 0, 480);
    touchY = map(touchpad.y, 940, 150, 0, 320);
    
    bool touched = 0;
    if (touchpad.z > 100)touched = 1;

    if( !touched )
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        data->state = LV_INDEV_STATE_PR;

        /*Set the coordinates*/
        data->point.x = touchX;
        data->point.y = touchY;

        Serial.print( "Data x " );
        Serial.print( touchX );
        Serial.print( "\t Data y " );
        Serial.println( touchY );
    }
#endif
}


void lvgl_hmi_init()
{
    //touch
    analogReadResolution(10);
    analogSetWidth(10);

    tft.begin();          /* TFT init */
    tft.setRotation( 1 ); /* Landscape orientation, flipped */

    lv_init();
    lv_disp_draw_buf_init( &draw_buf, buf, NULL, BUF_MULT );
    
    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init( &disp_drv );
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register( &disp_drv );

    /*Initialize the (dummy) input device driver*/
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init( &indev_drv );
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register( &indev_drv );


    // lv_demo_music();
    // lvgl_test();
    // lv_demo_widgets(); 
    lv_demo_stress();
    // lv_demo_benchmark();

    xTaskCreatePinnedToCore(codeForTask1, "LVGL", 10000, NULL, 1, &Task1, 0);
    xTaskCreatePinnedToCore(codeForTask2, "LVGL flush", 10000, NULL, 1, &Task2, 1);
}