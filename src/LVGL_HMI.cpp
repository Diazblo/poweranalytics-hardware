#include "LVGL_HMI.h"
#include <powermonitor.h>
extern powermonitor_data pwanl;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 0);
TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */


static lv_obj_t *chart;
static lv_chart_series_t *ser;
static lv_chart_cursor_t *cursor;
static lv_obj_t *chart2;
static lv_chart_series_t *ser2;
static lv_chart_cursor_t *cursor2;


bool pause_plot = 0;
unsigned long pause_plot_wait = 0;

void lvgl_pause_plot(lv_event_t * event)
{
    pause_plot = 1;
    pause_plot_wait = millis();
    lv_chart_set_zoom_x(chart, 500);
    lv_chart_set_zoom_x(chart2, 500);

}

void lvgl_plot()
{
    // for(uint8_t i=0; i<powermonitorno; i++){
    //         httpRequestData += "\"" + String(powermonitor_sync_data_name[i][10]) + "\":\"" + String(*powermonitor_sync_ptr[i]) + "\",";
    //     }

    if (!pause_plot)
    {
        lv_chart_refresh(chart);
        lv_chart_refresh(chart2);
        lv_chart_set_next_value(chart, ser, pwanl.instpower);
        lv_chart_set_next_value(chart, ser2, pwanl.totalpower);
    }
    else if ((millis() > pause_plot_wait + 5000))
    {
        pause_plot = 0;
        lv_chart_set_zoom_x(chart, 250);
        lv_chart_set_zoom_x(chart2, 250);
        lv_obj_scroll_to_x(chart, 0, LV_ANIM_OFF);
        lv_obj_scroll_to_x(chart2, 0, LV_ANIM_OFF);
    }
}


void load_screen()
{
    /* Instantaneous Power chart */
    chart = lv_chart_create(lv_scr_act());
    lv_obj_set_size(chart, 200, 130);
    lv_obj_align(chart, LV_ALIGN_TOP_LEFT, 40, 25);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 260);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_X, 0, 20);
    lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_SHIFT);
    // lv_chart_set_x_start_point(chart, ser, 20);
    lv_chart_set_point_count(chart, 20);
    lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_Y, 5, 3, 6, 5, true, 40);
    // lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_X, 5, 1, 10, 5, true, 20);
    cursor = lv_chart_add_cursor(chart, lv_palette_main(LV_PALETTE_BLUE), LV_DIR_LEFT | LV_DIR_BOTTOM);

    lv_obj_add_event_cb(chart, lvgl_pause_plot, LV_EVENT_CLICKED, NULL);
    lv_obj_refresh_ext_draw_size(chart);

    ser = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);

    lv_chart_set_zoom_x(chart, 250);

    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "Instantaneous Power (Watts)");
    lv_obj_align_to(label, chart, LV_ALIGN_OUT_TOP_MID, 0, -5);


    /* Average Power chart*/
    chart2 = lv_chart_create(lv_scr_act());
    lv_obj_set_size(chart2, 200, 130);
    lv_obj_align(chart2, LV_ALIGN_BOTTOM_LEFT, 40, -10);
    lv_chart_set_range(chart2, LV_CHART_AXIS_PRIMARY_Y, 0, 200);
    lv_chart_set_range(chart2, LV_CHART_AXIS_PRIMARY_X, 0, 20);
    lv_chart_set_update_mode(chart2, LV_CHART_UPDATE_MODE_SHIFT);
    // lv_chart_set_x_start_point(chart, ser2, 20);
    lv_chart_set_point_count(chart2, 20);
    lv_chart_set_axis_tick(chart2, LV_CHART_AXIS_PRIMARY_Y, 5, 3, 6, 5, true, 40);
    // lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_X, 5, 1, 10, 5, true, 20);
    cursor2 = lv_chart_add_cursor(chart2, lv_palette_main(LV_PALETTE_BLUE), LV_DIR_LEFT | LV_DIR_BOTTOM);

    lv_obj_add_event_cb(chart2, lvgl_pause_plot, LV_EVENT_CLICKED, NULL);
    lv_obj_refresh_ext_draw_size(chart2);

    ser2 = lv_chart_add_series(chart2, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_zoom_x(chart2, 250);
    lv_obj_t *label2 = lv_label_create(lv_scr_act());
    lv_label_set_text(label2, "Average Power (Watts)");
    lv_obj_align_to(label2, chart2, LV_ALIGN_OUT_TOP_MID, 0, -5);


    
    
}


lv_color_t *color_p;
lv_area_t area;
lv_disp_drv_t *disp;

TaskHandle_t Task1;
void codeForTask1(void *parameter)
{
    for (;;)
    {
        lv_timer_handler();
        vTaskDelay(1);
    }
}
TaskHandle_t Task2;
volatile bool newdata = 0;
void codeForTask2(void *parameter)
{
    for (;;)
    {
        if (newdata)
        {
            uint32_t w = (area.x2 - area.x1 + 1);
            uint32_t h = (area.y2 - area.y1 + 1);
            tft.startWrite();
            tft.setAddrWindow(area.x1, area.y1, w, h);
            tft.pushColors((uint16_t *)&color_p->full, w * h, true);
            tft.endWrite();
            newdata = 0;
            lv_disp_flush_ready(disp);
        }
        vTaskDelay(1);
    }
}

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *dispp, const lv_area_t *areaa, lv_color_t *color_pp)
{
    if (newdata == 0)
    {
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
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
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
    if (touchpad.z > 100)
        touched = 1;

    if (!touched)
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        data->state = LV_INDEV_STATE_PR;

        /*Set the coordinates*/
        data->point.x = touchX;
        data->point.y = touchY;

        Serial.print("Data x ");
        Serial.print(touchX);
        Serial.print("\t Data y ");
        Serial.println(touchY);
    }
#endif
}

void lvgl_hmi_init()
{
    //touch
    analogReadResolution(10);
    analogSetWidth(10);

    tft.begin();        /* TFT init */
    tft.setRotation(1); /* Landscape orientation, flipped */

    lv_init();
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, BUF_MULT);

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    /*Initialize the (dummy) input device driver*/
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);

    // lv_demo_music();
    // lvgl_test();
    // lv_demo_widgets();
    // lv_demo_stress();
    // lv_demo_benchmark();
    load_screen();

    xTaskCreatePinnedToCore(codeForTask1, "LVGL", 10000, NULL, 1, &Task1, 0);
    xTaskCreatePinnedToCore(codeForTask2, "LVGL flush", 10000, NULL, 1, &Task2, 1);
}