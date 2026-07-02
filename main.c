int main() {
    Process window;
    Process fpga;
    Process npu;

    // --- Spawn all servers ---
    window = spawn_process("window.exe");
    fpga   = spawn_process("fpga.exe");
    npu    = spawn_process("npu.exe");

    log("All servers launched.");

    // --- Main supervision loop ---
    while (1) {

        // If window dies, shut down everything
        if (!is_running(window)) {
            log("Window server terminated. Shutting down system.");

            terminate_process(fpga);
            terminate_process(npu);

            break; // exit server manager
        }

        // If FPGA dies, restart it
        if (!is_running(fpga)) {
            log("FPGA server died. Restarting...");
            fpga = spawn_process("fpga.exe");
        }

        // If NPU dies, restart it
        if (!is_running(npu)) {
            log("NPU server died. Restarting...");
            npu = spawn_process("npu.exe");
        }

        sleep_ms(500); // Poll twice per second
    }

    log("Server manager exiting.");
    return 0;
}