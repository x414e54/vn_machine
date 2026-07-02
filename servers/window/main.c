int main() {
    // Create window
    Window win = create_window(800, 600);

    // Open shared framebuffer
    SharedMem fb = open_shared_memory("VN_FRAMEBUFFER", FB_SIZE);
    uint32_t* pixels = fb.ptr;

    while (running) {
        pump_os_events(&running);
        
        send_os_events_to_fpga("COM3");

        // Blit shared framebuffer to window
        blit_to_window(win, pixels);

        wait_for_vsync(win);
    }

    return 0;
}
