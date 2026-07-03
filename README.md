This system has essentialy three layers.

The editor, content compiler creator... HTML/JAVSCRIPT. This is a non propretary metaverse called "the internet".
The simulator/emulator... WASM driven in the browser.
The console... FPGA hardware native that can load .game files over HTTP.

| Layer | File Type | Role | Determinism |
| --- | --- | --- | --- |
| Producer | ``.html`` | Create content | Nondeterministic |
| Simulator | ``.wasm`` | Emulate console | Deterministic |
| Hardware | ``.c`` / ``.vhdl`` | Define console | Deterministic |
| Content | ``.game`` | Run on console | Deterministic |

.game file header

[MachineHash]      8 bytes   // hash(fpga_bitstream)
[ContentHash]      8 bytes   // hash(game content)
[HeaderSize]       4 bytes
[ContentSize]      8 bytes
[Checksum]         8 bytes
[Reserved]         8 bytes

.game files must not contain .rtl and it is forbidden to reconfigure the machine via a .game file.
The pairing of .game and .rtl where the .game file contains a hash of the .rtl but not the .rtl itself means that it doesnt become a recusive VM mess. It protects security and sandboxing.

.rtl must be signed and validated through a web‑of‑trust.
This preserves determinism, safety, and human control.
AI can design hardware, but cannot deploy hardware.
Humans remain the gatekeepers.

.game files are scanned for .rtl before signing with the game signing key.
.rtl files are scanned for content before sining with the rtl signing key.

Content cannot contain hardware.
Hardware cannot contain content.

README.md — Visual Novel Engine with LLM‑Driven Transitions
Overview
This project implements a Visual Novel Engine in C, with an integrated LLM inference loop.
The LLM receives a predefined context prompt and the current VN state, then outputs exactly one transition code.
The VN engine validates this transition and updates the scene accordingly.

Architecture
The system consists of three major components:

LLM Inference Layer (C)

Loads a local GGUF model (e.g., Gemma 3 1B).

Uses a fixed context prompt describing:

allowed transition codes

current scene

flags

player input (if any)

Produces one token representing the next transition.

VN Runtime (Bevy)

Renders:

background scene

character sprite

text box

Executes deterministic transitions based on the LLM’s output.

Player Input Loop

When the VN engine emits GET_PLAYER_TEXT, the frontend:

opens a text input field

accepts up to 256 characters

sends this text to the LLM as player_action

receives a transition code

applies it to the VN state machine

LLM Prompt Contract
The model must follow this strict output format:

Code
You are a visual novel transition selector.
Output exactly ONE transition code from the list below.

Allowed transitions:
{A, B, C, REST, INVALID, ...}

Current scene: {scene_id}
Flags: {flag_list}
Player action: "{player_text}"

Output one transition code only.
The model must output only the code, e.g.:

Code
A
VN State Machine
Each transition code maps to deterministic engine actions:

A → load next scene

B → branch to alternate scene

REST → remain on current frame

INVALID → fallback transition

etc.

Frontend
The Bevy UI includes:

background sprite

character sprite

text box

optional choice overlays

player text input overlay (256 chars max)

Goal
QwenCode 3.6 35B should generate:

C code for:

GGUF model loading

inference loop

prompt construction

transition validation

VN state machine

Bevy code for:

scene rendering

character rendering

text box UI

player input overlay

A clean integration between the deterministic VN engine and the LLM transition selector.