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



#define PARAMS_PANEL_X 240
#define PARAMS_PANEL_Y 15
#define PARAMS_PANEL_Y_DIST 36
#define PARAMS_LABEL_DIST 120
lv_obj_t * params_panel[3];
lv_obj_t * params_label[3];
static lv_style_t style_common;
static lv_style_t style_params;


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
        lv_chart_set_next_value(chart, ser2, pwanl.avgpower);
        lv_label_set_text_fmt(params_panel[0], ":%d", (int)pwanl.instpower);
        lv_label_set_text_fmt(params_panel[1], ":%d", (int)pwanl.avgpower);
        lv_label_set_text_fmt(params_panel[2], ":%d", (int)pwanl.totalpower);
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




static void scroll_begin_event(lv_event_t * e)
{
    /*Disable the scroll animations. Triggered when a tab button is clicked */
    if(lv_event_get_code(e) == LV_EVENT_SCROLL_BEGIN) {
        lv_anim_t * a = lv_event_get_scroll_anim(e);
        if(a)  a->time = 0;
    }
}

void load_screen()
{   
    lv_obj_t *tabview;
    tabview = lv_tabview_create(lv_scr_act(), LV_DIR_RIGHT, 10);
    lv_obj_add_event_cb(lv_tabview_get_content(tabview), scroll_begin_event, LV_EVENT_SCROLL_BEGIN, NULL);

    lv_obj_t * tab_btns = lv_tabview_get_tab_btns(tabview);
    lv_obj_t *tab1 = lv_tabview_add_tab(tabview, "1");
    lv_obj_t *tab2 = lv_tabview_add_tab(tabview, "2");
    lv_obj_t *tab3 = lv_tabview_add_tab(tabview, "3");


    /* Instantaneous Power chart */
    chart = lv_chart_create(tab1); //lv_scr_act()
    lv_obj_set_size(chart, 200, 125);
    lv_obj_align(chart, LV_ALIGN_TOP_LEFT, 25, 10);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 260);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_X, 0, 20);
    lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_SHIFT);
    // lv_chart_set_x_start_point(chart, ser, 20);
    lv_chart_set_point_count(chart, 20);
    lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_Y, 5, 3, 6, 5, true, 40);
    // lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_X, 5, 1, 10, 5, true, 20);
    cursor = lv_chart_add_cursor(chart, lv_palette_main(LV_PALETTE_BLUE), LV_DIR_LEFT | LV_DIR_BOTTOM);

    /*Do not display points on the data*/
    lv_obj_set_style_size(chart, 0, LV_PART_INDICATOR);
    lv_obj_add_event_cb(chart, lvgl_pause_plot, LV_EVENT_CLICKED, NULL);
    lv_obj_refresh_ext_draw_size(chart);

    ser = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);

    lv_chart_set_zoom_x(chart, 250);

    lv_obj_t *label = lv_label_create(tab1); //lv_scr_act()
    lv_label_set_text(label, "Instantaneous Power (Watts)");
    lv_obj_align_to(label, chart, LV_ALIGN_OUT_TOP_MID, 0, -5);


    /* Average Power chart*/
    chart2 = lv_chart_create(tab1); //lv_scr_act()
    lv_obj_set_size(chart2, 200, 125);
    lv_obj_align(chart2, LV_ALIGN_BOTTOM_LEFT, 25, 0);
    lv_chart_set_range(chart2, LV_CHART_AXIS_PRIMARY_Y, 0, 200);
    lv_chart_set_range(chart2, LV_CHART_AXIS_PRIMARY_X, 0, 20);
    lv_chart_set_update_mode(chart2, LV_CHART_UPDATE_MODE_SHIFT);
    // lv_chart_set_x_start_point(chart, ser2, 20);
    lv_chart_set_point_count(chart2, 20);
    lv_chart_set_axis_tick(chart2, LV_CHART_AXIS_PRIMARY_Y, 5, 3, 6, 5, true, 40);
    // lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_X, 5, 1, 10, 5, true, 20);
    cursor2 = lv_chart_add_cursor(chart2, lv_palette_main(LV_PALETTE_BLUE), LV_DIR_LEFT | LV_DIR_BOTTOM);

    /*Do not display points on the data*/
    lv_obj_set_style_size(chart2, 0, LV_PART_INDICATOR);
    lv_obj_add_event_cb(chart2, lvgl_pause_plot, LV_EVENT_CLICKED, NULL);
    lv_obj_refresh_ext_draw_size(chart2);

    ser2 = lv_chart_add_series(chart2, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_zoom_x(chart2, 250);
    lv_obj_t *label2 = lv_label_create(tab1); //vlv_scr_act()
    lv_label_set_text(label2, "Average Power (Watts)");
    lv_obj_align_to(label2, chart2, LV_ALIGN_OUT_TOP_MID, 0, -5);

    lv_obj_clear_flag(lv_tabview_get_content(tabview), LV_OBJ_FLAG_SCROLLABLE);    
    // lv_obj_scroll_to_view_recursive(label, LV_ANIM_OFF);






    if 1
    /* Text panels */

    // STYLE
    lv_style_init(&style_common);
    // lv_style_set_bg_color(&style_common, lv_color_white());
    // lv_style_set_bg_opa(&style_common, LV_OPA_50);
    lv_style_set_border_width(&style_common, 2);
    lv_style_set_border_color(&style_common, lv_color_black());
    lv_style_set_text_font(&style_common, &lv_font_montserrat_28);

    lv_style_init(&style_params);
    // lv_style_set_bg_color(&style_params, lv_color_white());
    // lv_style_set_bg_opa(&style_params, LV_OPA_50);
    // lv_style_set_border_width(&style_params, 2);
    // lv_style_set_border_color(&style_params, lv_color_black());
    lv_style_set_text_font(&style_params, &lv_font_montserrat_32);



    for(uint8_t nnn = 0; nnn<3; nnn++){
        params_panel[nnn] = lv_label_create(tab1);
        lv_obj_set_x(params_panel[nnn], PARAMS_PANEL_X + PARAMS_LABEL_DIST);
        lv_obj_set_y(params_panel[nnn], PARAMS_PANEL_Y + (nnn*PARAMS_PANEL_Y_DIST) + 3);
        lv_obj_add_style(params_panel[nnn],&style_params, 0);

        params_label[nnn] = lv_label_create(tab1);
        lv_obj_set_x(params_label[nnn], PARAMS_PANEL_X);
        lv_obj_set_y(params_label[nnn], PARAMS_PANEL_Y + (nnn*PARAMS_PANEL_Y_DIST));
        lv_obj_add_style(params_label[nnn],&style_common, 0);

        
    }
    lv_label_set_text_fmt(params_label[0], "Power");
    lv_label_set_text_fmt(params_label[1], "Average");
    lv_label_set_text_fmt(params_label[2], "Total");
    
    #endif
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
        vTaskDelay(10);
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

    if (touchX > screenWidth) touchX=screenWidth-1;
    else if (touchX < 0) touchX=0;
    if (touchY > screenHeight) touchY=screenHeight-1;
    else if (touchY < 0) touchX=0;


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