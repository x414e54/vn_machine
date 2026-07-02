int main() {
    // Load GGUF model
    LlamaModel model = llama_load("gemma.gguf");

    while (running) {
        // Wait for AI request from FPGA interface
        AIRequest req = wait_for_request("AI_PIPE");

        // Build prompt from request
        char* prompt = build_prompt(req);

        // Infer one token
        char* raw = llama_infer(model, prompt, 1);

        // Validate transition code
        char code = validate_transition(raw);

        // Send back to FPGA interface
        send_transition(code, "AI_PIPE");
    }

    return 0;
}
