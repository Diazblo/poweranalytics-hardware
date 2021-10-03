#include "LVGL_HMI.h"

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 0);
TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */



static lv_obj_t * chart;
static lv_chart_series_t * ser;
static lv_chart_cursor_t * cursor;

static void event_cb(lv_event_t * e)
{
    static int32_t last_id = -1;
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    if(code == LV_EVENT_VALUE_CHANGED) {
        last_id = lv_chart_get_pressed_point(obj);
        if(last_id != LV_CHART_POINT_NONE) {
            lv_chart_set_cursor_point(obj, cursor, NULL, last_id);
        }
    }
    else if(code == LV_EVENT_DRAW_PART_END) {
        lv_obj_draw_part_dsc_t * dsc = lv_event_get_draw_part_dsc(e);
        if(dsc->part == LV_PART_CURSOR && dsc->p1 && dsc->p2 && dsc->p1->y == dsc->p2->y && last_id >= 0) {
            lv_coord_t * data_array = lv_chart_get_y_array(chart, ser);
            lv_coord_t v = data_array[last_id];
            char buf[16];
            lv_snprintf(buf, sizeof(buf), "%d", v);

            lv_point_t size;
            lv_txt_get_size(&size, buf, LV_FONT_DEFAULT, 0, 0, LV_COORD_MAX, LV_TEXT_FLAG_NONE);

            lv_area_t a;
            a.y2 = dsc->p1->y - 5;
            a.y1 = a.y2 - size.y - 10;
            a.x1 = dsc->p1->x + 10;
            a.x2 = a.x1 + size.x + 10;

            lv_draw_rect_dsc_t draw_rect_dsc;
            lv_draw_rect_dsc_init(&draw_rect_dsc);
            draw_rect_dsc.bg_color = lv_palette_main(LV_PALETTE_BLUE);
            draw_rect_dsc.radius = 3;

            lv_draw_rect(&a, dsc->clip_area, &draw_rect_dsc);

            lv_draw_label_dsc_t draw_label_dsc;
            lv_draw_label_dsc_init(&draw_label_dsc);
            draw_label_dsc.color = lv_color_white();
            a.x1 += 5;
            a.x2 -= 5;
            a.y1 += 5;
            a.y2 -= 5;
            lv_draw_label(&a, dsc->clip_area, &draw_label_dsc, buf, NULL);
        }
    }
}

/**
 * Show cursor on the clicked point
 */
void load_screen()
{
    chart = lv_chart_create(lv_scr_act());
    lv_obj_set_size(chart, 200, 150);
    lv_obj_align(chart, LV_ALIGN_TOP_LEFT, 50, 50);

    lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 6, 5, true, 40);
    lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_X, 10, 5, 10, 1, true, 30);

    lv_obj_add_event_cb(chart, event_cb, LV_EVENT_ALL, NULL);
    lv_obj_refresh_ext_draw_size(chart);

    cursor = lv_chart_add_cursor(chart, lv_palette_main(LV_PALETTE_BLUE), LV_DIR_LEFT | LV_DIR_BOTTOM);

    ser = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    uint32_t i;
    for(i = 0; i < 10; i++) {
        lv_chart_set_next_value(chart, ser, lv_rand(10,90));
    }

    lv_chart_set_zoom_x(chart, 100);

    lv_obj_t * label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "Power");
    lv_obj_align_to(label, chart, LV_ALIGN_OUT_TOP_MID, 0, -5);
}










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
#if 1
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
    // lv_demo_stress();
    // lv_demo_benchmark();
    load_screen();

    xTaskCreatePinnedToCore(codeForTask1, "LVGL", 10000, NULL, 1, &Task1, 0);
    xTaskCreatePinnedToCore(codeForTask2, "LVGL flush", 10000, NULL, 1, &Task2, 1);
}