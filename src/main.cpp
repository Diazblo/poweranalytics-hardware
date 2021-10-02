
#include <Arduino.h>
#include "LVGL_HMI.h"

void setup()
{
    Serial.begin( 115200 ); /* prepare for possible serial debug */
    lvgl_hmi_init();
    // lv_demo_widgets(); 
    
}


void loop()
{
    delay( 5 );
}


