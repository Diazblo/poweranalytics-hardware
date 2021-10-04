#include <powermonitor.h>

EnergyMonitor ph1;
// powermonitor_data phase1;

#define MAX_STRUCT_INST 3
uint8_t struct_ptr_cnt = 0;
powermonitor_data *struct_ptr_array[MAX_STRUCT_INST];

void powermonitor_init(powermonitor_data * self)
{
    ph1.voltage(34, 234.26, 1.7);  // Voltage: input pin, calibration, phase_shift
    ph1.current(35, 111.1); 
    struct_ptr_array[struct_ptr_cnt++] = self;
    
}

uint8_t pw_power = 0;
double total_power = 0;
double avg_power = 0;
uint32_t total_power_cnt = 0;

void powermonitor_task()
{
    #if 0
    ph1.calcVI(20,2000);         // Calculate all. No.of half wavelengths (crossings), time-out
    ph1.serialprint();           // Print out all variables (realpower, apparent power, Vrms, Irms, power factor)

    float realPower       = ph1.realPower;        //extract Real Power into variable
    float apparentPower   = ph1.apparentPower;    //extract Apparent Power into variable
    float powerFActor     = ph1.powerFactor;      //extract Power Factor into Variable
    float supplyVoltage   = ph1.Vrms;             //extract Vrms into Variable
    float Irms            = ph1.Irms;             //extract Irms into Variable
    #endif

    pw_power = random(0, 250);
    avg_power = ( (pw_power*0.01) + (avg_power*0.99) );
    total_power += 0.0002*pw_power;
    
    
    struct_ptr_array[0] -> instpower  = pw_power;
    struct_ptr_array[0] -> avgpower   = avg_power;
    struct_ptr_array[0] -> totalpower = total_power;

}
