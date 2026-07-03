#include <jni.h>
#include "machine.h"

extern void machine_power_switch(unsigned char on)
{
    // Power on/off the machine
    if (on) {
        machine_init();
        machine_start();
    }
    else {
        machine_stop();
    }
}

extern void machine_reset_switch()
{
    // Reset the machine
    machine_stop();
    machine_init();
    machine_start();
}