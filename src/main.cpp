#include <Arduino.h>
#include "LVGL_HMI.h"
#include<wifi_handler.h>

void setup()
{
    Serial.begin( 115200 ); /* prepare for possible serial debug */
    lvgl_hmi_init();
    // lv_demo_widgets(); 

    if (!pwanl_wifiinit())
        start_config_portal();
    
}


void loop()
{
    delay( 5 );
}


