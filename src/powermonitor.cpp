#include <powermonitor.h>

EnergyMonitor ph1;
powermonitor_data phase1;

void powermonitor_init()
{
    ph1.voltage(34, 234.26, 1.7);  // Voltage: input pin, calibration, phase_shift
    ph1.current(35, 111.1); 

}


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


    phase1.instpower = random(200);

}
