#include <powermonitor.h>
#include <Arduino.h>


// powermonitor_data phase1;

#define MAX_STRUCT_INST 3
uint8_t struct_ptr_cnt = 0;
powermonitor_data *struct_ptr_array[MAX_STRUCT_INST];

EnergyMonitor emon_ptr[MAX_STRUCT_INST];

TaskHandle_t Task_powermonitor;
void powermonitortask(void *parameter)
{
    for (;;)
    {
        emon_ptr[0].calcVI(20,200);         // Calculate all. No.of half wavelengths (crossings), time-out
        Serial.printf("%d\t", emon_ptr[0].numberOfSamples);
        emon_ptr[0].serialprint();           // Print out all variables (realpower,   power, Vrms, Irms, power factor)
        powermonitor_task();
        vTaskDelay(10);
    }
}
void powermonitor_init(powermonitor_data * self)
{   
    uint8_t pwanl_arg1 = 34;
    uint8_t pwanl_arg2 = 35;

    // analogSetSamples(1);
    // analogSetCycles(32);
    analogSetAttenuation(ADC_0db);
    analogSetClockDiv(8);
    analogReadResolution(12);
    analogSetWidth(12);

    adcAttachPin(pwanl_arg1);
    adcAttachPin(pwanl_arg2);
    emon_ptr[struct_ptr_cnt].voltage(pwanl_arg1, 307, 1.7);  // Voltage: input pin, calibration, phase_shift
    emon_ptr[struct_ptr_cnt].current(pwanl_arg2, 8.2); 
    struct_ptr_array[struct_ptr_cnt++] = self;

    for(uint8_t i=0; i<30;   i++)
        emon_ptr[0].calcVI(20,200);
    xTaskCreatePinnedToCore(powermonitortask, "powermonitor", 10000, NULL, 2, &Task_powermonitor, 1);
}

#define ESTIMATION_RATE 0.700 //in kWh
double pw_power = 0;
double total_power = 0;
double avg_power = 0;
double rms_power_sum = 0;

uint32_t total_power_cnt = 0;
double  total_kwh = 0;
#define RATE_MULT 1 *3600
// #define RATE_MULT 0.01666667 *3600
unsigned long total_power_timer = 0;

#define TOTAL_POWER_INTERVAL 10*1000
double total_power_mult = 3600/(TOTAL_POWER_INTERVAL/1000);

void powermonitor_task()
{
    #if 0
    float realPower       = ph1.realPower;        //extract Real Power into variable
    float apparentPower   = ph1.apparentPower;    //extract Apparent Power into variable
    float powerFActor     = ph1.powerFactor;      //extract Power Factor into Variable
    float supplyVoltage   = ph1.Vrms;             //extract Vrms into Variable
    float Irms            = ph1.Irms;             //extract Irms into Variable
    #endif

    // pw_power = random(0, 700);
    // if(!(total_power_cnt % 20)) pw_power = random(1000, 1600);

    //Totals power from all phases
    pw_power = emon_ptr[0].apparentPower;

    rms_power_sum = ( (pw_power*pw_power*0.03) + (rms_power_sum*0.97) );
    struct_ptr_array[0] -> rmspower   = sqrt(rms_power_sum);

    avg_power = ( (pw_power*0.03) + (avg_power*0.97) );
    
    // adds smoothed power value to total variable divided by interval
    if ((millis() > total_power_timer+TOTAL_POWER_INTERVAL)){
        total_power_timer = millis();

        #if 1
        total_power += (avg_power / total_power_mult);
        struct_ptr_array[0] -> esttotalpower += (ESTIMATION_RATE / total_power_mult);
        struct_ptr_array[0] -> saving = ((struct_ptr_array[0] -> esttotalpower) - (struct_ptr_array[0] -> totalpower));
        #else
        total_power_cnt++; //add first to start with 1
        double total_power_frac = (double)(total_power_cnt-1)/total_power_cnt;
        total_power = ( (struct_ptr_array[0] -> rmspower) /total_power_cnt) + total_power*total_power_frac;

        double multt = (total_power_cnt)/(RATE_MULT);
        total_kwh = total_power * multt;
        #endif
        struct_ptr_array[0] -> totalpower = total_power/1000;
    }

    struct_ptr_array[0] -> instpower  = pw_power;
    struct_ptr_array[0] -> avgpower   = avg_power;
    struct_ptr_array[0] -> powerfactor  = emon_ptr[0].powerFactor;
    // Serial.printf("total average: %f\talt: %f \t%d \n", total_power, total_power_frac, total_power_cnt);
}
