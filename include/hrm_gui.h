#pragma once

#include <windows.h>
#include "utils.h"

class coord {
public:
    int x;
    int y;

    coord(int _x = 0, int _y = 0) {
        x = _x;
        y = _y;
    }
};

class HRM_GUI {
public:
    bool cursor_visible;
    Record *record;
    Robot *robot;
    int record_id;

    HRM_GUI(bool _cursor_visible = false);

    ~HRM_GUI();

    bool run();

    bool welcome();

    void set_cursor(int _x, int _y);

    void reset_cursor();

    void clear_screen();

    void hide_cursor();

    coord cursor_now();

    void clear_line(int _y);

    void new_record();  // 新建存档
};