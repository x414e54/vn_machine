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