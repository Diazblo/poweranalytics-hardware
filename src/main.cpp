#include <Arduino.h>
#include <WiFi.h>
#include <LVGL_HMI.h>
#include <wifi_handler.h>
#include <powermonitor.h>
#include <preprocessor.h>


uint32_t syncinterval = 0;

powermonitor_data pwanl;
wifi_handler wifi_h;

void setup()
{
    delay(1000);
    WiFi.mode(WIFI_OFF);
    Serial.begin(115200); /* prepare for possible serial debug */


    #ifndef DEVMODE
    lvgl_hmi_init();
    powermonitor_init(&pwanl);
    #else
    #endif
    powermonitor_sync_data_add(wifi_h, pwanl.avgpower);
    powermonitor_sync_data_add(wifi_h, pwanl.powerfactor);
    powermonitor_sync_data_add(wifi_h, pwanl.totalpower);
    powermonitor_sync_data_add(wifi_h, pwanl.esttotalpower);

    if (!wifi_init())
        start_config_portal();
        
}

void loop()
{
    #ifndef DEVMODE
    #else
    powermonitor_task();
    #endif
    // powermonitor_task();
    //pwanl_sync() on intervals
    if (WiFi.status() == WL_CONNECTED && (millis() > syncinterval+20000)) wifi_h.pwanl_sync(), syncinterval = millis();
    // if ((millis() > syncinterval+100))
    //     lvgl_plot(), syncinterval = millis();
    server_loop();
    delay(5000);
}
