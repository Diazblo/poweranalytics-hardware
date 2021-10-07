/*

// #include "LVGL_HMI.h"
#include "lvgl.h"
extern lv_obj_t * tab2_chart;
extern void lvgl_hmi_init();

void draw_event_cb(lv_event_t * e)
{
    lv_obj_draw_part_dsc_t * dsc = lv_event_get_param(e);

    if(dsc->part == LV_PART_TICKS && dsc->id == LV_CHART_AXIS_PRIMARY_X) {
        const char * month[] = {"Jan", "Febr", "March", "Apr", "May", "Jun", "July", "Aug", "Sept", "Oct", "Nov", "Dec"};
        lv_snprintf(dsc->text, sizeof(dsc->text), "%s", month[dsc->value]);
    }
}
void event_init()
{
    lvgl_hmi_init();
    lv_obj_add_event_cb(tab2_chart, draw_event_cb, LV_EVENT_DRAW_PART_BEGIN, NULL);
}

*/