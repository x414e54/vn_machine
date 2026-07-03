#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <android/log.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "NPU", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "NPU", __VA_ARGS__)

static pthread_t npu_thread;
static unsigned char npu_initialized = 0;
static unsigned char npu_running = 0;

void* npu_module(void* arg);

void npu_init()
{
    if (npu_initialized || npu_running) {
        return;
    }

    npu_initialized = 1;
}

void npu_start()
{
    if (!npu_initialized || npu_running) {
        return;
    }

    npu_running = 1;
    pthread_create(&npu_thread, NULL, npu_module, NULL);
}

void npu_stop()
{
    if (!npu_running) {
        return;
    }

    npu_running = 0;
    pthread_join(npu_thread, NULL);
}

void* npu_module(void* arg)
{
    while(npu_running)
    {
        LOGI("I am an NPU.");
        usleep(10000000);
    }

    return 0;
}
/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "llama.h"   // from llama.cpp

// -----------------------------
// npu IPC stubs (replace these)
// -----------------------------
typedef struct {
    char input[256];
} AIRequest;

AIRequest wait_for_request(const char *pipe_name) {
    AIRequest req;
    // TODO: replace with real pipe/UART read
    strcpy(req.input, "test");
    return req;
}

void send_transition(char code, const char *pipe_name) {
    // TODO: replace with real pipe/UART write
    printf("Sending transition: %c\n", code);
}

// -----------------------------
// Prompt builder
// -----------------------------
char *build_prompt(AIRequest req) {
    static char buf[512];
    snprintf(buf, sizeof(buf),
             "You are a classifier. Output exactly one of: A, B, Z.\n"
             "Input: %s\n"
             "Output:", req.input);
    return buf;
}

// -----------------------------
// Transition validator
// -----------------------------
char validate_transition(const char *raw) {
    // raw may contain whitespace or multiple tokens
    for (int i = 0; raw[i]; i++) {
        if (raw[i] == 'A' || raw[i] == 'B' || raw[i] == 'Z') {
            return raw[i];
        }
    }
    return 'Z'; // fallback
}

// -----------------------------
// Main NPU loop
// -----------------------------
int main() {
    // -----------------------------
    // Load GGUF model
    // -----------------------------
    struct llama_model_params mparams = llama_model_default_params();
    struct llama_context_params cparams = llama_context_default_params();

    mparams.n_gpu_layers = 0;     // CPU only
    cparams.n_ctx = 512;          // small context
    cparams.seed = 1234;          // deterministic

    struct llama_model *model =
            llama_load_model_from_file("gemma.gguf", mparams);

    if (!model) {
        fprintf(stderr, "Failed to load model\n");
        return 1;
    }

    struct llama_context *ctx =
            llama_new_context_with_model(model, cparams);

    if (!ctx) {
        fprintf(stderr, "Failed to create context\n");
        return 1;
    }

    bool running = true;

    while (running) {
        // -----------------------------
        // Wait for npu request
        // -----------------------------
        AIRequest req = wait_for_request("AI_PIPE");

        // -----------------------------
        // Build prompt
        // -----------------------------
        char *prompt = build_prompt(req);

        // -----------------------------
        // Tokenize prompt
        // -----------------------------
        llama_reset(ctx);
        llama_token tokens[256];
        int ntokens = llama_tokenize(model, prompt, tokens, 256, true);

        // -----------------------------
        // Feed prompt into model
        // -----------------------------
        if (llama_eval(ctx, tokens, ntokens, 0, 0) != 0) {
            fprintf(stderr, "llama_eval failed\n");
            continue;
        }

        // -----------------------------
        // Infer exactly one token
        // -----------------------------
        struct llama_sampling_params sparams = {
                .temp = 0.0f,     // deterministic
                .top_k = 1,
                .top_p = 0.1f,
                .repeat_penalty = 1.0f,
        };

        llama_token tok = llama_sampling_sample(ctx, &sparams);

        // Decode token to string
        const char *raw = llama_token_to_str(model, tok);

        // -----------------------------
        // Validate transition code
        // -----------------------------
        char code = validate_transition(raw);

        // -----------------------------
        // Send back to npu
        // -----------------------------
        send_transition(code, "AI_PIPE");
    }

    llama_free(ctx);
    llama_free_model(model);

    return 0;
}
*/