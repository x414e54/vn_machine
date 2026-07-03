let Module = Module || {};
let powerSwitch = null;
let resetSwitch = null;
let tick = null;
let sendInputFn = null;

function log(msg) {
    const el = document.getElementById('log');
    const time = new Date().toISOString().split('T')[1].split('.')[0];
    el.textContent += `[${time}] ${msg}\n`;
    el.scrollTop = el.scrollHeight;
}

Module.onRuntimeInitialized = () => {
    // Bind exported C functions
    powerSwitch = Module.cwrap('machine_power_switch', null, ['number']);
    resetSwitch = Module.cwrap('machine_reset_switch', null, []);
    tick = Module.cwrap('machine_tick', null, []);
    // Optional: input bridge from JS → C
    sendInputFn = Module.cwrap('machine_input', null, ['number']);

    log('WASM runtime initialized.');

    // Start render/tick loop
    function loop() {
        if (tick) tick();
        requestAnimationFrame(loop);
    }
    loop();
};

function consolePowerOn() {
    if (!powerSwitch) {
        log('PowerOn: runtime not ready.');
        return;
    }
    powerSwitch(1);
    log('Console power ON.');
}

function consolePowerOff() {
    if (!powerSwitch) {
        log('PowerOff: runtime not ready.');
        return;
    }
    powerSwitch(0);
    log('Console power OFF.');
}

function consoleReset() {
    if (!resetSwitch) {
        log('Reset: runtime not ready.');
        return;
    }
    resetSwitch();
    log('Console RESET.');
}

// Map virtual gamepad events to numeric codes for C
const INPUT_CODES = {
    UP: 1,
    DOWN: 2,
    LEFT: 3,
    RIGHT: 4,
    A: 5,
    B: 6,
    X: 7,
    Y: 8,
    START: 9,
    SELECT: 10
};

function sendInput(code) {
    const c = INPUT_CODES[code];
    log(`Input: ${code}`);
    if (sendInputFn && c !== undefined) {
        sendInputFn(c);
    }
}