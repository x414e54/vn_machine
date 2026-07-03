#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <android/log.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "BUS", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "BUS", __VA_ARGS__)

static pthread_t bus_thread;
static unsigned char bus_initialized = 0;
static unsigned char bus_running = 0;

void* bus_module(void* arg);

void bus_init()
{
    if (bus_initialized || bus_running) {
        return;
    }

    bus_initialized = 1;
}

void bus_start()
{
    if (!bus_initialized || bus_running) {
        return;
    }

    bus_running = 1;
    pthread_create(&bus_thread, NULL, bus_module, NULL);
}

void bus_stop()
{
    if (!bus_running) {
        return;
    }

    bus_running = 0;
    pthread_join(bus_thread, NULL);
}

void* bus_module(void* arg)
{
    while(bus_running)
    {
        LOGI("I am a bus.");
        usleep(10000000);
//ask JNI for events
//forward events to bus
//get bus events
//forward to npu
//get npu events forward to bus.
    }

    return 0;
}