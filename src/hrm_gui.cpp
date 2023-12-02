#include"hrm_gui.h"
#include <iostream>
#include <windows.h>


void setConsoleFontAndSize(LPCWSTR fontName ,int size = 16) {
    CONSOLE_FONT_INFOEX fontInfo;
    fontInfo.cbSize = sizeof(fontInfo);
    GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &fontInfo);
    fontInfo.dwFontSize.X = 0;
    fontInfo.dwFontSize.Y = size;
    wcscpy_s(fontInfo.FaceName, fontName); // 设置字体名称
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &fontInfo);
}

HRM_GUI::HRM_GUI(bool _cursor_visible)
{
    std::cout << "正在进入游戏..." << std::endl;
    if(!_cursor_visible)
    {
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursor_info;
        GetConsoleCursorInfo(handle, &cursor_info);
        cursor_info.bVisible = false;
        SetConsoleCursorInfo(handle, &cursor_info);
    }
    record = new Record();
}

HRM_GUI::~HRM_GUI()
{
    std::cout << "正在退出游戏..." << std::endl;
    delete record;
}

void HRM_GUI::run()
{
    SetConsoleOutputCP(CP_UTF8);
    setConsoleFontAndSize(L"KaiTi");
    clear_screen();
    // 打印欢迎界面
    welcome();
}

void HRM_GUI::welcome()
{
    // 欢迎界面
    clear_screen();
    int border_x = 50;
    int border_y = 10;
    std::cout << "+";
    for(int i = 0; i < border_x - 1; i++)
    {
        std::cout << "-";
    }
    std::cout << "+" << std::endl << '|';
    set_cursor(border_x/2 - 12, 1);
    std::cout << "欢迎来到《程序员升职记》!";
    set_cursor(border_x, 1);
    std::cout << '|' << std::endl << '+';
    for(int i = 0; i < border_x - 1; i++)
    {
        std::cout << "-";
    }
    std::cout << '+' << std::endl << '|';
    set_cursor(border_x/2 - 8 ,3);
    std::cout << "请选择您的工作证:" << std::endl;
    set_cursor(border_x, 3);
    std::cout << '|' << std::endl << '+';
    for(int i = 0; i < border_x - 1; i++)
    {
        std::cout << "-";
    }
    std::cout << '+' << std::endl;

    Sleep(10000);
    clear_screen();
}

void HRM_GUI::set_cursor(int _x, int _y)
{
    // 设置光标位置
    HANDLE hOut;
    COORD pos = { static_cast<SHORT>(_x), static_cast<SHORT>(_y) };
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hOut, pos);
}

void HRM_GUI::clear_screen()
{
    // 清屏
    system("cls");
}
