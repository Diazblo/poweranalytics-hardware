#include <Arduino.h>
#include <WiFi.h>
#include <LVGL_HMI.h>
#include <wifi_handler.h>
#include <powermonitor.h>
#include <preprocessor.h>


const char *host = "poweranalytics";
uint32_t syncinterval = 0;

powermonitor_data pwanl;
wifi_handler wifi_h;

void setup()
{   
    
    WiFi.mode(WIFI_OFF);
    Serial.begin( 115200 ); /* prepare for possible serial debug */

    lvgl_hmi_init();

    delay(1000);

    powermonitor_init(&pwanl);
    powermonitor_sync_data_add(wifi_h, pwanl.instpower);
    powermonitor_sync_data_add(wifi_h, pwanl.totalpower);
    powermonitor_sync_data_add(wifi_h, pwanl.avgpower);
    powermonitor_sync_data_add(wifi_h, pwanl.powerfactor);


    // if (!wifi_init()) start_config_portal();
    // for()
    // {

    // }
    
}



void loop()
{   
    powermonitor_task();  
    //pwanl_sync() on intervals
    // if (WiFi.status() == WL_CONNECTED && (millis() > syncinterval+5000))
    //     wifi_h.pwanl_sync(), syncinterval = millis();

    if ((millis() > syncinterval+100))
        lvgl_plot(), syncinterval = millis();


    delay(1);
}


