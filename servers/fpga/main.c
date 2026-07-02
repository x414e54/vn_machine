int main() {
    // Open shared framebuffer
    SharedMem fb = open_shared_memory("VN_FRAMEBUFFER", FB_SIZE);
    uint32_t* pixels = fb.ptr;

    // Open UART to FPGA
    Uart uart = uart_open("COM3");

    VNState vn = vn_init();

    while (running) {
        // Check FPGA for events (player input, timers, etc.)
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

        sleep_ms(16);
    }

    return 0;
}
