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
    std::cout << "正在疯狂加载配置文件ing..." << std::endl;
    hide_cursor();
    record = new Record();
    HWND console = GetConsoleWindow();
    RECT rect;
    GetWindowRect(console, &rect);

    MoveWindow(console, rect.left, rect.top, 800, 600, TRUE); // 设置窗口大小为 800x600
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
    // 欢迎界面
    int continue_last = welcome();
    clear_screen();
    // 角色创建
    if(!continue_last)
    {
        // 删除旧档
        record->card[record_id].valid = false;
        record->save();
        new_record();
    }
}

void HRM_GUI::new_record()
{
    // 新建存档界面
    int border_x = 50;
    int border_y = 10;
    std::cout << "+";
    for(int i = 0; i < border_x - 1; i++)
    {
        std::cout << "-";
    }
    std::cout << "+" << std::endl << '|';
    set_cursor(border_x/2 - 12, 1);
    std::cout << "创建新角色";
    set_cursor(border_x, 1);
    std::cout << '|' << std::endl << '+';
    for(int i = 0; i < border_x - 1; i++)
    {
        std::cout << "-";
    }
    std::cout << '+' << std::endl << '|';
    set_cursor(1 ,3);
    std::cout << "我们给您拍摄了工作照，哪一张您更满意呢：";
    coord last_pos1 = cursor_now();
    set_cursor(border_x, 3);
    std::cout << '|' << std::endl << '+';
    for(int i = 0; i < (border_x - 1) / 2; i++)
    {
        std::cout << "-";
    }
    std::cout << '+';
    for(int i = (border_x-1)/2 + 1; i < border_x - 1; i++)
    {
        std::cout << "-";
    }
    std::cout << '+' << std::endl;
    for(int i = 0; i < 9; i++)
    {
        std::cout << '|';
        for(int j = 0; j < (border_x - 1) / 2; j++)
        {
            std::cout << ' ';
        }
        std::cout << '|';
        for(int j = (border_x-1)/2 + 1; j < border_x - 1; j++)
        {
            std::cout << ' ';
        }
        std::cout << '|' << std::endl;
    }
    std::cout << '+';
    for(int i = 0; i < (border_x - 1) / 2; i++)
    {
        std::cout << "-";
    }
    std::cout << '+';
    for(int i = (border_x-1)/2 + 1; i < border_x - 1; i++)
    {
        std::cout << "-";
    }
    std::cout << '+' << std::endl;
    std::cout << "|请输入您的昵称：（不超过8个英文字符）";
    coord last_pos2 = cursor_now();
    set_cursor(border_x, last_pos2.y);
    std::cout << '|' << std::endl << '+';
    for(int i = 0; i < border_x - 1; i++)
    {
        std::cout << "-";
    }
    std::cout << '+' << std::endl;
    // 绘制工作照
    Robot *robot1 = new Robot(RobotType::male);
    Robot *robot2 = new Robot(RobotType::female);
    set_cursor(border_x/4, 5);
    std::cout << "1. ";


    // 接受用户输入
    set_cursor(last_pos1.x, last_pos1.y);
    RobotType type;
    while(1)
    {
        char choice[100];
        std::cin >> choice;
        if(choice[0]=='1'&&choice[1]=='\0')
        {
            type = RobotType::male;
            break;
        }
        else if(choice[0]=='2'&&choice[1]=='\0')
        {
            type = RobotType::female;
            break;
        }
        clear_line(last_pos1.y);
        set_cursor(border_x, last_pos1.y);
        std::cout << '|';
        set_cursor(0, last_pos1.y);
        std::cout << "|输入有误，请重新选择工作照：";
    }

    set_cursor(last_pos2.x, last_pos2.y);
    std::string name;
    while(1)
    {
        char choice[100];
        std::cin >> choice;
        if(strlen(choice) <= 8)
        {
            name = choice;
            break;
        }
        clear_line(last_pos2.y);
        set_cursor(border_x, last_pos2.y);
        std::cout << '|';
        set_cursor(0, last_pos2.y);
        std::cout << "|输入有误，请重新输入昵称：";
    }

    // 保存存档
    if(type == RobotType::male)
        robot = robot1;
    else
        robot = robot2;
    record->card[record_id].valid = true;
    record->card[record_id].level = 1;
    record->card[record_id].robot_type = type;
    record->card[record_id].name = name;
    record->save();
}

bool HRM_GUI::welcome()
{
    // 欢迎界面
    int border_x = 50;
    int border_y = 10;
    std::cout << "+";
    for(int i = 0; i < border_x - 1; i++)
    {
        std::cout << "-";
    }
    std::cout << "+" << std::endl << '|';
    set_cursor(border_x/2 - 12, 1);
    std::cout << "欢迎来到《人力资源机》!";
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

    for(int i = 0; i < border_x-1; i++)
    {
        std::cout << '-';
    }
    std::cout << '+' << std::endl << '+';

    int separator1 = border_x / 3 - 1;
    int separator2 = border_x / 3 * 2 - 1;
    for(int i = 0; i < border_x-1; i++)
    {
        if(i == separator1 || i == separator2)
            std::cout << '+';
        else
            std::cout << '-';
    }
    std::cout << '+' << std::endl;

    // 绘制三张工作证
    // 绘制边框
    for(int i = 0; i < 9; i++)
    {
        std::cout << '|';
        for(int j = 0; j < border_x - 1; j++)
        {
            if(j == separator1 || j == separator2)
                std::cout << '|';
            else
                std::cout << ' ';
        }
        std::cout << '|' << std::endl;
    }
    std::cout << '+';
    int last_y = cursor_now().y;
    for(int i = 0; i < border_x - 1; i++)
    {
        if(i == separator1 || i == separator2)
            std::cout << '+';
        else
            std::cout << '-';
    }
    std::cout << '+' << std::endl;
    // 绘制工作证
    Robot *robot1;
    Robot *robot2;
    Robot *robot3;
    set_cursor(5,6);
    std::cout << " 存档一";
    if(record->card[0].valid)
    {
        robot1 = new Robot(record->card[0].robot_type);
        set_cursor(2, 7);
        std::cout << "最高关卡：" << record->card[0].level;
        set_cursor(2, 8);
        std::cout << "昵称：" << record->card[0].name;
        set_cursor(2, 9);
        std::cout << "工作照：";
        robot1->move(6, 11);
        robot1->printRobot();
    }
    else
    {
        set_cursor(5, 7);
        std::cout << "暂无存档";
    }
    set_cursor(5 + border_x/3, 6);
    std::cout << " 存档二";
    if(record->card[1].valid)
    {
        robot1 = new Robot(record->card[1].robot_type);
        set_cursor(2 + border_x/3, 7);
        std::cout << "最高关卡：" << record->card[1].level;
        set_cursor(2 + border_x/3, 8);
        std::cout << "昵称：" << record->card[1].name;
        set_cursor(2 + border_x/3, 9);
        std::cout << "工作照：";
        robot1->move(6 + border_x/3, 11);
        robot1->printRobot();
    }
    else
    {
        set_cursor(5 + border_x/3, 7);
        std::cout << "暂无存档";
    }
    set_cursor(6 + border_x/3*2, 6);
    std::cout << " 存档三";
    if(record->card[2].valid)
    {
        robot1 = new Robot(record->card[2].robot_type);
        set_cursor(2 + 2*border_x/3, 7);
        std::cout << "最高关卡：" << record->card[2].level;
        set_cursor(2 + 2*border_x/3, 8);
        std::cout << "昵称：" << record->card[2].name;
        set_cursor(2 + 2*border_x/3, 9);
        std::cout << "工作照：";
        robot1->move(6 + 2*border_x/3, 11);
        robot1->printRobot();
    }
    else
    {
        set_cursor(6 + border_x/3*2, 7);
        std::cout << "暂无存档";
    }
    hide_cursor();

    // 选择存档
    set_cursor(0, last_y + 2);
    std::cout << '+';
    int new_y = cursor_now().y;
    for(int i = 0; i < border_x - 1; i++)
    {
        std::cout << '-';
    }
    std::cout << '+';
    set_cursor(border_x, last_y + 1);
    std::cout << '|';
    set_cursor(0, last_y + 1);
    std::cout << "|请选择存档：";
    int card_id;
    while(1)
    {
        char choice[100];
        std::cin >> choice;
        if(choice[0]>='1'&&choice[0]<='3'&&choice[1]=='\0')
        {
            card_id = choice[0] - '0' - 1;
            break;
        }
        clear_line(last_y + 1);
        set_cursor(border_x, last_y + 1);
        std::cout << '|';
        set_cursor(0, last_y + 1);
        std::cout << "|输入有误，请重新选择存档：";
    }
    
    record_id = card_id;
    bool continue_last;
    if(record->card[card_id].valid)
    {
        set_cursor(border_x, last_y + 3);
        std::cout << '|';
        set_cursor(0, last_y + 4);
        std::cout << "|( 0 : 继续存档, 1 : 新建存档)";
        set_cursor(border_x, last_y + 4);
        std::cout << '|' << std::endl << '+';
        new_y = cursor_now().y;
        for(int i = 0; i < border_x; i++)
        {
            std::cout << '-';
        }
        std::cout << '+';
        set_cursor(0, last_y + 3);
        std::cout << "|继续上次的存档还是新建存档？：";
        while(1)
        {
            char choice[100];
            std::cin >> choice;
            if(choice[0]>='0'&&choice[0]<='1'&&choice[1]=='\0')
            {
                if(choice[0] == '0')
                {
                    // 继续存档
                    continue_last = true;
                    break;
                }
                else
                {
                    // 新建存档
                    continue_last = false;
                    break;
                }
            }
            clear_line(last_y + 3);
            set_cursor(border_x, last_y + 3);
            std::cout << '|';
            set_cursor(0, last_y + 3);
            std::cout << "|输入有误，请重新选择继续存档或者新建存档：";
        }
    }
    else
    {
        continue_last = false;
    }

    set_cursor(0, new_y + 1);
    if(continue_last)
        {
            if(card_id == 0) robot = robot1;
            else if(card_id == 1) robot = robot2;
            else robot = robot3;
            std::cout << "加载存档中...请稍候";
        }
    else
        std::cout << "新建存档中...请稍候";
    Sleep(1000);
    return continue_last;
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
    reset_cursor();
}

void HRM_GUI::hide_cursor()
{
    if(cursor_visible) return;
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursor_info;
    GetConsoleCursorInfo(handle, &cursor_info);
    cursor_info.bVisible = false;
    SetConsoleCursorInfo(handle, &cursor_info);
}

coord HRM_GUI::cursor_now()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int cursorX = 0, cursorY = 0;
    // 获取光标位置
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        cursorX = csbi.dwCursorPosition.X;
        cursorY = csbi.dwCursorPosition.Y;
    }
    return coord(cursorX, cursorY);
}

// 清除指定行的输出
void HRM_GUI::clear_line(int line) {
    COORD cursorPosition;
    cursorPosition.X = 0; // 将光标移动到行的开头
    cursorPosition.Y = line; // 指定要清除的行号

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsole, cursorPosition); // 将光标移动到指定位置

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);

    DWORD written;
    FillConsoleOutputCharacter(hConsole, ' ', csbi.dwSize.X, cursorPosition, &written); // 用空格覆盖整行输出
    SetConsoleCursorPosition(hConsole, cursorPosition); // 将光标移动回行的开头
}

void HRM_GUI::reset_cursor()
{
    set_cursor(0, 0);
}