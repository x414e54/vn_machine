VirtualConsole summary
VirtualConsole is a deterministic hardware simulator, written in C and compiled to WASM, that emulates a single “virtual console” appliance inside the browser or any WASM host.

Core responsibilities
Run signed .rtl:

Loads a validated, signed hardware personality file (.rtl).

Initializes internal machine state (CPU, buses, pipelines, scratchpads, timing model) strictly according to the .rtl description.

Refuses to start if the .rtl signature or machine hash is invalid.

Run signed .game:

Loads a validated, signed game capsule (.game) that contains only deterministic content (story, assets, pipelines, state machines) and a machine hash.

Verifies that the .game machine hash matches the currently loaded .rtl personality.

Refuses to start if the .game signature or machine hash is invalid.

Input model
Gamepad / Keyboard / Mouse API:

Exposes a simple C/WASM API for input events:

vc_set_gamepad_state(...)

vc_set_keyboard_state(...)

vc_set_mouse_state(...)

The host (JS) forwards browser input to these APIs.

The simulator consumes these inputs on each tick/frame and feeds them into the emulated hardware input ports defined by .rtl.

Power and control
Power On:

vc_power_on()

Resets all hardware state to a known, deterministic boot state.

Loads the current .rtl and .game into memory and prepares the supervisor CPU.

Power Off:

vc_power_off()

Halts execution, clears volatile state, and returns the simulator to an inert state.

No further cycles are executed until powered on again.

Reset Button:

vc_reset()

Performs a warm reset:

Keeps the loaded .rtl and .game.

Reinitializes machine state to boot conditions.

Restarts execution from the beginning of the game.

Execution model
Deterministic stepping:

Provides a main stepping API, e.g. vc_step_frame() or vc_step_cycles(n), which advances the simulated hardware by a fixed amount of time.

All behavior is fully deterministic given:

the loaded .rtl

the loaded .game

the input event stream.

Rendering / audio:

Exposes frame buffers and audio buffers via C/WASM APIs for the host to present:

vc_get_video_buffer()

vc_get_audio_buffer()

Implementation constraints
Language: C (portable, low‑level, deterministic).

Target: Compiled to WASM for browser and other WASM hosts.

Security:

Only runs signed .rtl and signed .game.

Never executes arbitrary code from content.

All logic is hardware‑shaped and sandboxed inside the WASM module.