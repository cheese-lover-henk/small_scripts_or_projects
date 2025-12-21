#pragma once

enum WindowEventType {
    KEYDOWN,
    KEYUP,
    MOUSEMOVE,
    MOUSEDOWN_L,
    MOUSEUP_L,
    MOUSEDOWN_R,
    MOUSEUP_R,
    CLOSE,
};

struct KeyEvent {
    int keycode;
};

struct MouseEventPos {
    int x;
    int y;
};

struct WindowEvent {
    WindowEventType type;
    union {
        KeyEvent key;
        MouseEventPos mouse;
    } pl;
};