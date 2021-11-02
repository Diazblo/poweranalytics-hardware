#include "LVGL_HMI.h"
#include <powermonitor.h>
extern powermonitor_data pwanl;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 0);
TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */

// #define INDEPENDANTFLUSHING

static lv_obj_t *tab1_chart;
static lv_chart_series_t *tab1_ser;
static lv_chart_cursor_t *cursor;
static lv_obj_t *tab1_chart2;
static lv_chart_series_t *tab1_ser2;
static lv_chart_series_t *tab1_ser3;
static lv_chart_cursor_t *cursor2;

lv_obj_t * tab2_chart;

#ifdef LABEL
#endif

#define CHART_MAX 100

#define CHART_X 25
#define CHART_Y 10
#define CHART_HEIGHT 125
#define CHART_WIDTH 150
#define CHART_DISTANCE 35

#define PARAMS_PANEL_DISTANCE 15
#define PARAMS_PANEL_X CHART_X+CHART_WIDTH+PARAMS_PANEL_DISTANCE
#define PARAMS_PANEL_DIV 95
#define PARAMS_PANEL_HEIGHT 278
#define PARAMS_PANEL_WIDTH 480-45-(PARAMS_PANEL_X)
#define PARAMS_PANEL_Y CHART_Y

#define PARAMS_PANEL_Y_DIST 36
#define PARAMS_LABEL_DIST 120

#ifdef LABEL
lv_obj_t * params_panel[3];
lv_obj_t * params_label[3];
static lv_style_t style_common;
static lv_style_t style_params;
#else
lv_obj_t * table;
#endif


void chart_auto_range( lv_obj_t *tab1_chart, lv_chart_series_t *ser_auto_range){
    int16_t var_type_max = (1<<15) - 1;
    int16_t chart_auto_range_max = 0, chart_auto_range_min = var_type_max;

    for(uint8_t rr = 0; rr < CHART_MAX; rr++){
        if((chart_auto_range_max < (ser_auto_range->y_points[rr])) && (ser_auto_range->y_points[rr]) < var_type_max) chart_auto_range_max = (ser_auto_range->y_points[rr]);
        if(chart_auto_range_min > (ser_auto_range->y_points[rr])) chart_auto_range_min = (ser_auto_range->y_points[rr]);
    }
    lv_chart_set_range(tab1_chart, LV_CHART_AXIS_PRIMARY_Y, chart_auto_range_min, chart_auto_range_max+10);//(chart_auto_range_max*0.1)
}
void chart_auto_range( lv_obj_t *tab1_chart, lv_chart_series_t *ser_auto_range, lv_chart_series_t *ser_auto_range1){
    int16_t var_type_max = (1<<15) - 1;
    int16_t chart_auto_range_max = 0, chart_auto_range_min = var_type_max;

    for(uint8_t rr = 0; rr < CHART_MAX; rr++){
        if((chart_auto_range_max < (ser_auto_range->y_points[rr])) && (ser_auto_range->y_points[rr]) < var_type_max) chart_auto_range_max = (ser_auto_range->y_points[rr]);
        if(chart_auto_range_min > (ser_auto_range->y_points[rr])) chart_auto_range_min = (ser_auto_range->y_points[rr]);
        if((chart_auto_range_max < (ser_auto_range1->y_points[rr])) && (ser_auto_range1->y_points[rr]) < var_type_max) chart_auto_range_max = (ser_auto_range1->y_points[rr]);
        if(chart_auto_range_min > (ser_auto_range1->y_points[rr])) chart_auto_range_min = (ser_auto_range1->y_points[rr]);
    }
    lv_chart_set_range(tab1_chart, LV_CHART_AXIS_PRIMARY_Y, chart_auto_range_min, chart_auto_range_max+10);//(chart_auto_range_max*0.1)
}

bool pause_plot = 0;
unsigned long pause_plot_wait = 0;

void lvgl_pause_plot(lv_event_t * event)
{
    pause_plot = 1;
    pause_plot_wait = millis();
    lv_chart_set_zoom_x(tab1_chart, 500);
    lv_chart_set_zoom_x(tab1_chart2, 500);
    lv_obj_scroll_to_x(tab1_chart, 200, LV_ANIM_ON);
    lv_obj_scroll_to_x(tab1_chart2, 200, LV_ANIM_ON);

}
uint8_t plot_div = 10;
uint16_t peak_power = 0;
void lvgl_plot()
{
    // for(uint8_t i=0; i<powermonitorno; i++){
    //         httpRequestData += "\"" + String(powermonitor_sync_data_name[i][10]) + "\":\"" + String(*powermonitor_sync_ptr[i]) + "\",";
    //     }

    if (!pause_plot)
    {   
        #ifdef LABEL
        lv_label_set_text_fmt(params_panel[0], ":%d", (int)pwanl.instpower);
        lv_label_set_text_fmt(params_panel[1], ":%d", );
        lv_label_set_text_fmt(params_panel[2], ":%d", (int)pwanl.totalpower);
        #else
        #ifdef SAVINGPANEL
        lv_table_set_cell_value_fmt(table, 0, 1, "%d W", (int)pwanl.instpower);
        lv_table_set_cell_value_fmt(table, 1, 1, "%d W", (int)pwanl.rmspower);
        String power_string = String(pwanl.totalpower, 4) + " kW";
        lv_table_set_cell_value(table, 2, 1, power_string.c_str() );
        lv_table_set_cell_value(table, 3, 1, String(pwanl.powerfactor).c_str());
        // lv_table_set_cell_value_fmt(table, 2, 1, "%d kWh", (int)pwanl.totalpower);
        // lv_table_set_cell_value_fmt(table, 3, 1, "%d", (int)(pwanl.powerfactor*100.0));
        #else
        // lv_table_set_cell_value_fmt(table, 0, 1, "%d kWh", (int)pwanl.avgpower);
        String avgpower_string = String((pwanl.avgpower/1000), 3) + " kWh";
        lv_table_set_cell_value(table, 0, 1, avgpower_string.c_str() );

        String power_string = String(pwanl.totalpower, 3) + " kW";
        lv_table_set_cell_value(table, 1, 1, power_string.c_str() );

        String est_power_string = String(pwanl.esttotalpower, 3) + " kW";
        lv_table_set_cell_value(table, 2, 1, est_power_string.c_str() );

        String saving_string = String(pwanl.saving, 3) + " kW";
        lv_table_set_cell_value(table, 3, 1, saving_string.c_str() );
        #endif

        lv_obj_invalidate(table); //workaround to update table
        // lv_event_send(table, LV_EVENT_DRAW_PART_END, NULL);
        // refr_size(table);
        #endif


        if (pwanl.instpower > peak_power){
            peak_power = pwanl.instpower;
            
        }
        if(! plot_div-- ){
            plot_div = 10;
            lv_chart_set_next_value(tab1_chart, tab1_ser, peak_power);
            peak_power = 0;
            #ifdef SAVINGPANEL
            lv_chart_set_next_value(tab1_chart, tab1_ser2, pwanl.rmspower);
            #else
            lv_chart_set_next_value(tab1_chart, tab1_ser2, (uint16_t)(pwanl.totalpower*1000));
            lv_chart_set_next_value(tab1_chart, tab1_ser3, (uint16_t)(pwanl.esttotalpower*1000));
            // chart_auto_range(tab1_chart2, tab1_ser3);
            #endif

            chart_auto_range(tab1_chart, tab1_ser);
            chart_auto_range(tab1_chart2, tab1_ser2, tab1_ser3);
            
            lv_chart_refresh(tab1_chart);
            lv_chart_refresh(tab1_chart2);
        }


    }
    else if ((millis() > pause_plot_wait + 5000))
    {
        pause_plot = 0;
        lv_chart_set_zoom_x(tab1_chart, 250);
        lv_chart_set_zoom_x(tab1_chart2, 250);
        lv_obj_scroll_to_x(tab1_chart, 0, LV_ANIM_ON);
        lv_obj_scroll_to_x(tab1_chart2, 0, LV_ANIM_ON);
    }
}



void draw_event_cb(lv_event_t * e)
{
    lv_obj_draw_part_dsc_t * dsc = lv_event_get_param(e);

    if(dsc->part == LV_PART_TICKS && dsc->id == LV_CHART_AXIS_PRIMARY_X) {
        const char * month[] = {"Jan", "Febr", "March", "Apr", "May", "Jun", "July", "Aug", "Sept", "Oct", "Nov", "Dec"};
        lv_snprintf(dsc->text, sizeof(dsc->text), "%s", month[dsc->value]);
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
    lv_obj_t *tab1 = lv_tabview_add_tab(tabview, " ");
    lv_obj_t *tab2 = lv_tabview_add_tab(tabview, " ");
    lv_obj_t *tab3 = lv_tabview_add_tab(tabview, " ");


    /*
    * TAB 1
    */
    /* Peak Power chart */
    tab1_chart = lv_chart_create(tab1); //lv_scr_act()
    lv_obj_set_size(tab1_chart, CHART_WIDTH, CHART_HEIGHT);
    lv_obj_align(tab1_chart, LV_ALIGN_TOP_LEFT, CHART_X, CHART_Y);
    lv_chart_set_range(tab1_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 1600);
    lv_chart_set_range(tab1_chart, LV_CHART_AXIS_PRIMARY_X, 0, CHART_MAX);
    lv_chart_set_update_mode(tab1_chart, LV_CHART_UPDATE_MODE_SHIFT);
    // lv_chart_set_x_start_point(tab1_chart, tab1_ser, 20);
    lv_chart_set_point_count(tab1_chart, CHART_MAX);
    lv_chart_set_axis_tick(tab1_chart, LV_CHART_AXIS_PRIMARY_Y, 5, 3, 6, 5, true, 40);
    // lv_chart_set_axis_tick(tab1_chart, LV_CHART_AXIS_PRIMARY_X, 5, 1, 10, 5, true, 20);
    cursor = lv_chart_add_cursor(tab1_chart, lv_palette_main(LV_PALETTE_BLUE), LV_DIR_LEFT | LV_DIR_BOTTOM);

    /*Do not display points on the data*/
    lv_obj_set_style_size(tab1_chart, 0, LV_PART_INDICATOR);
    lv_obj_add_event_cb(tab1_chart, lvgl_pause_plot, LV_EVENT_RELEASED, NULL);
    lv_obj_refresh_ext_draw_size(tab1_chart);

    tab1_ser = lv_chart_add_series(tab1_chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);

    lv_chart_set_zoom_x(tab1_chart, 250);

    lv_obj_t *label = lv_label_create(tab1); //lv_scr_act()
    lv_label_set_text(label, "Peak Power (Watts)");
    lv_obj_align_to(label, tab1_chart, LV_ALIGN_OUT_TOP_MID, 0, -5);


    /* Second Power chart*/
    tab1_chart2 = lv_chart_create(tab1); //lv_scr_act()
    lv_obj_set_size(tab1_chart2, CHART_WIDTH, CHART_HEIGHT);
    lv_obj_align(tab1_chart2, LV_ALIGN_TOP_LEFT, CHART_X, (CHART_HEIGHT+CHART_DISTANCE) );
    lv_chart_set_range(tab1_chart2, LV_CHART_AXIS_PRIMARY_Y, 0, 1600);
    lv_chart_set_range(tab1_chart2, LV_CHART_AXIS_PRIMARY_X, 0, CHART_MAX);
    lv_chart_set_update_mode(tab1_chart2, LV_CHART_UPDATE_MODE_SHIFT);
    // lv_chart_set_x_start_point(tab1_chart, tab1_ser2, 20);
    lv_chart_set_point_count(tab1_chart2, CHART_MAX);
    lv_chart_set_axis_tick(tab1_chart2, LV_CHART_AXIS_PRIMARY_Y, 5, 3, 6, 5, true, 40);
    // lv_chart_set_axis_tick(tab1_chart, LV_CHART_AXIS_PRIMARY_X, 5, 1, 10, 5, true, 20);
    cursor2 = lv_chart_add_cursor(tab1_chart2, lv_palette_main(LV_PALETTE_BLUE), LV_DIR_LEFT | LV_DIR_BOTTOM);

    /*Do not display points on the data*/
    lv_obj_set_style_size(tab1_chart2, 0, LV_PART_INDICATOR);
    lv_obj_add_event_cb(tab1_chart2, lvgl_pause_plot, LV_EVENT_RELEASED, NULL);
    lv_obj_refresh_ext_draw_size(tab1_chart2);

    tab1_ser2 = lv_chart_add_series(tab1_chart2, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_zoom_x(tab1_chart2, 250);
    lv_obj_t *label2 = lv_label_create(tab1); //vlv_scr_act()

    // lv_obj_scroll_to_view_recursive(label, LV_ANIM_OFF);
    #if SAVINGPANEL
    lv_label_set_text(label2, "RMS Power (Watts)");
    #else
    tab1_ser3 = lv_chart_add_series(tab1_chart2, lv_palette_main(LV_PALETTE_LIGHT_BLUE), LV_CHART_AXIS_PRIMARY_Y);
    lv_label_set_text(label2, "Total Usage (Watts)");
    #endif

    lv_obj_align_to(label2, tab1_chart2, LV_ALIGN_OUT_TOP_MID, 0, -5);
    lv_obj_clear_flag(lv_tabview_get_content(tabview), LV_OBJ_FLAG_SCROLLABLE);    

   

    

    #ifdef LABEL
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
    #else

     // STYLE
    static lv_style_t style_table;
    lv_style_init(&style_table);
    lv_style_set_text_font(&style_table, &lv_font_montserrat_16);
    
     /* Text Table*/
    table = lv_table_create(tab1);
    lv_obj_set_height(table, PARAMS_PANEL_HEIGHT);
    lv_obj_set_width(table, PARAMS_PANEL_WIDTH);
    lv_table_set_col_width(table, 0, PARAMS_PANEL_DIV);
    lv_table_set_col_width(table, 1, PARAMS_PANEL_WIDTH-PARAMS_PANEL_DIV+50);
    // lv_table_set_row_(table, 0, 100);
    lv_obj_add_flag(table, LV_STYLE_LINE_ROUNDED);
    lv_obj_set_scrollbar_mode(table, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_style(table,&style_table, 0);

    #if 0
    lv_table_set_cell_value(table, 0, 0, "Peak");
    lv_table_set_cell_value(table, 1, 0, "RMS");
    lv_table_set_cell_value(table, 2, 0, "Total");
    lv_table_set_cell_value(table, 3, 0, "PF");

    lv_table_set_cell_value(table, 0, 1, "0");
    lv_table_set_cell_value(table, 1, 1, "0");
    lv_table_set_cell_value(table, 2, 1, "10");
    lv_table_set_cell_value(table, 3, 1, "10");
    #else
    lv_table_set_cell_value(table, 0, 0, "Rate");
    lv_table_set_cell_value(table, 1, 0, "Total");
    lv_table_set_cell_value(table, 2, 0, "Est. Total");
    lv_table_set_cell_value(table, 3, 0, "Saving");

    lv_table_set_cell_value(table, 0, 1, "0");
    lv_table_set_cell_value(table, 1, 1, "0");
    lv_table_set_cell_value(table, 2, 1, "0");
    lv_table_set_cell_value(table, 3, 1, "0");
    #endif

    lv_obj_set_x(table, PARAMS_PANEL_X);
    lv_obj_set_y(table, PARAMS_PANEL_Y);

    /* Style */
    static lv_style_t style_common;
    lv_style_init(&style_common);
    
    lv_style_set_text_font(&style_common, &lv_font_montserrat_18);
    lv_obj_add_style(table, &style_common, 0);


    #endif



    /*
    * TAB 2
    */
    
    tab2_chart = lv_chart_create( tab2);
    lv_obj_set_size(tab2_chart, 480-100, 320-100);
    lv_obj_center(tab2_chart);
    lv_chart_set_type(tab2_chart, LV_CHART_TYPE_BAR);
    lv_chart_set_range(tab2_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
    lv_chart_set_range(tab2_chart, LV_CHART_AXIS_SECONDARY_Y, 0, 400);
    lv_chart_set_point_count(tab2_chart, 12);
    lv_obj_add_event_cb(tab2_chart, draw_event_cb, LV_EVENT_DRAW_PART_BEGIN, NULL);

    /*Add ticks and label to every axis*/
    lv_chart_set_axis_tick(tab2_chart, LV_CHART_AXIS_PRIMARY_X, 10, 5, 12, 3, true, 40);
    lv_chart_set_axis_tick(tab2_chart, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 6, 2, true, 50);
    lv_chart_set_axis_tick(tab2_chart, LV_CHART_AXIS_SECONDARY_Y, 10, 5, 3, 4, true, 50);

    static lv_chart_cursor_t *cursor____;
    cursor____ = lv_chart_add_cursor(tab2_chart, lv_palette_main(LV_PALETTE_BLUE), LV_DIR_LEFT | LV_DIR_BOTTOM);

    lv_obj_t *label___ = lv_label_create(tab2); //lv_scr_act()
    lv_label_set_text(label___, "Monthly Consumption (kWh)");
    lv_obj_align_to(label___, tab2_chart, LV_ALIGN_OUT_TOP_MID, 0, -5);

    /*Zoom in a little in X*/
    lv_chart_set_zoom_x(tab2_chart, 800);

    /*Add two data series*/
    lv_chart_series_t * ser1 = lv_chart_add_series(tab2_chart, lv_palette_lighten(LV_PALETTE_GREEN, 2), LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_series_t * ser2 = lv_chart_add_series(tab2_chart, lv_palette_darken(LV_PALETTE_GREEN, 2), LV_CHART_AXIS_SECONDARY_Y);

    /*Set the next points on 'ser1'*/
    lv_chart_set_next_value(tab2_chart, ser1, 31);
    lv_chart_set_next_value(tab2_chart, ser1, 66);
    lv_chart_set_next_value(tab2_chart, ser1, 10);
    lv_chart_set_next_value(tab2_chart, ser1, 89);
    lv_chart_set_next_value(tab2_chart, ser1, 63);
    lv_chart_set_next_value(tab2_chart, ser1, 56);
    lv_chart_set_next_value(tab2_chart, ser1, 32);
    lv_chart_set_next_value(tab2_chart, ser1, 35);
    lv_chart_set_next_value(tab2_chart, ser1, 57);
    lv_chart_set_next_value(tab2_chart, ser1, 85);
    lv_chart_set_next_value(tab2_chart, ser1, 22);
    lv_chart_set_next_value(tab2_chart, ser1, 58);

    lv_coord_t * ser2_array =  lv_chart_get_y_array(tab2_chart, ser2);
    /*Directly set points on 'ser2'*/
    ser2_array[0] = 92;
    ser2_array[1] = 71;
    ser2_array[2] = 61;
    ser2_array[3] = 15;
    ser2_array[4] = 21;
    ser2_array[5] = 35;
    ser2_array[6] = 35;
    ser2_array[7] = 58;
    ser2_array[8] = 31;
    ser2_array[9] = 53;
    ser2_array[10] = 33;
    ser2_array[11] = 73;

    lv_chart_refresh(tab2_chart); /*Required after direct set*/
   
}


lv_color_t *color_p;
lv_area_t area;
lv_disp_drv_t *disp;

uint32_t lvgl_plot_interval = 0;
TaskHandle_t Task1;
void codeForTask1(void *parameter)
{
    for (;;)
    {
        // if ((millis() > lvgl_plot_interval+100))
            lvgl_plot(), lvgl_plot_interval = millis();

        lv_timer_handler();
        vTaskDelay(10);
    }
}

#ifdef INDEPENDANTFLUSHING
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
#endif

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *dispp, const lv_area_t *areaa, lv_color_t *color_pp)
{   
    #ifdef INDEPENDANTFLUSHING
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
    #else
    uint32_t w = (areaa -> x2 - areaa -> x1 + 1);
    uint32_t h = (areaa -> y2 - areaa -> y1 + 1);
    tft.startWrite();
    tft.setAddrWindow(areaa -> x1, areaa -> y1, w, h);
    tft.pushColors((uint16_t *)&color_pp->full, w * h, true);
    tft.endWrite();
    lv_disp_flush_ready(dispp);
    #endif
}

/*Read the touchpad*/
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
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

    xTaskCreatePinnedToCore(codeForTask1, "LVGL", 10000, NULL, 1, &Task1, 1);

    #ifdef INDEPENDANTFLUSHING
    xTaskCreatePinnedToCore(codeForTask2, "LVGL flush", 10000, NULL, 1, &Task2, 1);
    #endif
}