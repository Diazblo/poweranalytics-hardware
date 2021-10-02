#ifndef POWERMONITOR_H
#define POWERMONITOR_H

#include "EmonLib.h"

struct powermonitor_data
{
    double instpower;
    double totalpower;
    double powerfactor;
    double avgpower;
    double vrms;
    double btrbtbrtb;
    double yhyttbt;
};

void powermonitor_init();
void powermonitor_task();

#endif /* POWERMONITOR_H */