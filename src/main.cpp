#include <Arduino.h>
#include <WiFi.h>
#include "LVGL_HMI.h"
#include <wifi_handler.h>

const char *host = "poweranalytics";

void setup()
{
    Serial.begin( 115200 ); /* prepare for possible serial debug */
    lvgl_hmi_init();
    // lv_demo_widgets(); 


    //ioinit
    //sanitycheck
    //rtcsync
    //wifiinit
    //syncdatawithserver
    //clearbuffer
    //displayerrorstat
    //displaypowerreadings
    if (!pwanl_wifiinit())
        start_config_portal();
    
}


void loop()
{
    //pwanl_sync() on intervals
    if (WiFi.status() == WL_CONNECTED)
        pwanl_sync();
    delay(3000);
}


