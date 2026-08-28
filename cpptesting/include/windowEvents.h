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
    RESIZE,
};

struct KeyEvent {
    int keycode;
};

struct MouseEventPos {
    int x;
    int y;
};

struct SizeXY {
    int x;
    int y;
};

struct WindowEvent {
    WindowEventType type;
    union {
        KeyEvent key;
        MouseEventPos mouse;
        SizeXY size;
    } pl;
};

enum WindowCommandType {
    PAINT_FRAME,
    CLOSE_WINDOW,
};

struct WindowCommand {
    WindowCommandType type;
};