#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <android/log.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "FPGA", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "FPGA", __VA_ARGS__)

static pthread_t fpga_thread;
static unsigned char fpga_initialized = 0;
static unsigned char fpga_running = 0;

void* fpga_module(void* arg);

void fpga_init()
{
    if (fpga_initialized || fpga_running) {
        return;
    }

    fpga_initialized = 1;
}

void fpga_start()
{
    if (!fpga_initialized || fpga_running) {
        return;
    }

    fpga_running = 1;
    pthread_create(&fpga_thread, NULL, fpga_module, NULL);
}

void fpga_stop()
{
    if (!fpga_running) {
        return;
    }

    fpga_running = 0;
    pthread_join(fpga_thread, NULL);
}

void* fpga_module(void* arg)
{
    /* Open shared framebuffer
    SharedMem fb = open_shared_memory("VN_FRAMEBUFFER", FB_SIZE);
    uint32_t* pixels = fb.ptr;

    // Open UART to FPGA
    Uart uart = uart_open("COM3");

    VNState vn = vn_init();*/

    while (fpga_running) {
        LOGI("I am an FPGA.");
        usleep(10000000);
    /*    // Check FPGA for events (player input, timers, etc.)
        FPGAEvent ev = uart_read_event(uart);

        // Update VN state machine
        vn_update(&vn, ev);

        // If VN logic needs AI help
        if (vn_needs_ai(&vn)) {
            send_ai_request(vn, "AI_PIPE");
        }

        // If AI responded with a transition code
        if (ai_has_response("AI_PIPE")) {
            char code = ai_read_transition("AI_PIPE");
            vn_apply_transition(&vn, code);
        }

        // Render framebuffer into shared memory
        vn_render(&vn, pixels);

        sleep_ms(16);*/
    }

    return 0;
}