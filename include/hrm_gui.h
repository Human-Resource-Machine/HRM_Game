#pragma once
#include <windows.h>
#include "utils.h"

class HRM_GUI
{
    public:
        // 成员变量
        bool cursor_visible;
        Record *record;

        // 成员函数
        HRM_GUI(bool _cursor_visible = false);
        ~HRM_GUI();
        void run();
        void welcome();
        void set_cursor(int _x, int _y);
        void clear_screen();
};