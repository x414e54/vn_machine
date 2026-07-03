#include "bus.h"
#include "fpga.h"
#include "npu.h"

static unsigned char machine_initialized = 0;
static unsigned char machine_running = 0;

void machine_init()
{
    if (machine_initialized || machine_running) {
        return;
    }

    bus_init();
    fpga_init();
    npu_init();
    machine_initialized = 1;
}

void machine_start()
{
    if (!machine_initialized || machine_running) {
        return;
    }

    bus_start();
    fpga_start();
    npu_start();
    machine_running = 1;
}

void machine_stop()
{
    if (!machine_running) {
        return;
    }

    bus_stop();
    fpga_stop();
    npu_stop();
    machine_running = 1;
}