#include <Arduino.h>
#include <WiFi.h>
#include <LVGL_HMI.h>
#include <wifi_handler.h>
#include <powermonitor.h>
#include <preprocessor.h>


const char *host = "poweranalytics";
uint32_t syncinterval = 0;

// extern uint8_t powermonitor_sync_data_no;

// #define powermonitor_sync_data_no ( sizeof(powermonitor_sync_data_name)/sizeof(powermonitor_sync_data_name[0]) )

powermonitor_data pwanl;

void setup()
{   
    
    WiFi.mode(WIFI_OFF);
    Serial.begin( 115200 ); /* prepare for possible serial debug */
    lvgl_hmi_init();
    // lv_demo_widgets(); 

    delay(1000);
    //ioinit
    //sanitycheck
    //rtcsync
    //wifiinit
    //syncdatawithserver
    //clearbuffer
    //displayerrorstat
    //displaypowerreadings




    powermonitor_init();
    if (!pwanl_wifiinit())
        start_config_portal();
    // for()
    // {

    // }
    
}



void loop()
{   
    powermonitor_task();
    
    //pwanl_sync() on intervals
    if (WiFi.status() == WL_CONNECTED && (millis() > syncinterval+5000))
        pwanl_sync(), syncinterval = millis();

    delay(500);
}


