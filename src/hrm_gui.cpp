#include"hrm_gui.h"
#include <iostream>
#include <windows.h>
#include "utils.h"

#include "debug.h"

void setConsoleFontAndSize(LPCWSTR fontName, int size = 16) {
    CONSOLE_FONT_INFOEX fontInfo;
    fontInfo.cbSize = sizeof(fontInfo);
    GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &fontInfo);
    fontInfo.dwFontSize.X = 0;
    fontInfo.dwFontSize.Y = size;
    wcscpy_s(fontInfo.FaceName, fontName); // 设置字体名称
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &fontInfo);
}

HRM_GUI::HRM_GUI(bool _cursor_visible) {
    SetConsoleOutputCP(CP_UTF8);
    setConsoleFontAndSize(L"KaiTi");
    std::cout << "正在疯狂加载配置文件ing..." << std::endl;
    hide_cursor();
    record = new Record();
    HWND console = GetConsoleWindow();
    RECT rect;
    GetWindowRect(console, &rect);

    MoveWindow(console, rect.left, rect.top, 800, 600, TRUE); // 设置窗口大小为 800x600
    Sleep(1000);
}

HRM_GUI::~HRM_GUI() {
    std::cout << "正在退出游戏..." << std::endl;
    delete record;
    delete robot;
}

void HRM_GUI::run() {
    clear_screen();
    // 欢迎界面
    int continue_last = welcome();
    clear_screen();
    // 角色创建
    if (!continue_last) {
        // 删除旧档
        record->card[record_id].valid = false;
        record->save();
        new_record();
    }
    std::cin.get();

    std::cout << "choose a level: " << std::endl;
    int level=2;
    std::cin >> level;
    std::cin.get();
    clear_screen();



    //=============

    auto readJsonFile = [](const std::string &filename) -> Json::Value {
        //1.打开文件
        std::ifstream ifile;
        ifile.open(filename);

        //2.创建json读工厂对象
        Json::CharReaderBuilder ReaderBuilder;
        ReaderBuilder["emitUTF8"] = true;//utf8支持，不加这句，utf8的中文字符会编程\uxxx

        //3.创建json对象，等会要返回这个对象
        Json::Value root;

        //4.把文件转变为json对象，要用到上面的三个变量,数据写入root
        std::string strerr;
        bool ok = Json::parseFromStream(ReaderBuilder, ifile, &root, &strerr);
        if (!ok) {
            std::cerr << "json解析错误";
        }
        //5.返回存有数据的json，这个json对象已经能用了
        return root;
    };

    auto read = [&readJsonFile](int level, std::vector<int> &input, std::vector<int> &output, int &ground,
                                std::vector<std::string> &instructions) -> void {
        using std::vector;
        using std::string;
        input.clear();
        output.clear();
        instructions.clear();
        auto root = readJsonFile(string("../level/") + std::to_string(level) + string(".json"));
        ground = root["ground"].asInt();
        for (const auto &i: root["instructions"]) {
            auto s = i.asString();
            instructions.push_back(s);
        }
        for (const auto &i: root["input"]) {
            auto s = i.asInt();
            input.push_back(s);
        }
        for (const auto &i: root["output"]) {
            auto s = i.asInt();
            output.push_back(s);
        }
    };
    //=============

    // 准备工作：期望的输入输出、地面的大小、可用的指令
    std::vector<int> input{};
    std::vector<int> output{};
    int ground;
    std::vector<std::string> available_instructions{};
    // 读入输入输出、地面的大小、可用的指令
    read(level, input, output, ground, available_instructions);

    // 读入编写的指令
    std::cout << "input:" << std::endl;
    std::string s;
    code_manager r;
    r.input_ = input;
    r.ground_ = std::vector<int>(ground, 0);
    while (getline(std::cin, s)) {
        if (s == "") break;
        r.add_instruction(s);
    }
    // TODO: 检查指令是否合法
    // 运行
    clear_screen();
    robot->ingame = true;

    Curtain c{};
    robot->move(6, 11);
    {
        {
            // 画机器人
            clear_screen();
            robot->print();
        }
        {
            c.set_state(r.input_, r.output_, r.ground_, r.instruction_, r.pc_);
            c.print();
            cin.get();
            clear_screen();
        }
        {
            // 移动机器人
            int x = robot->pos_x;
            int y = robot->pos_y;
            int target_x;
            int target_y;
            if (r.instruction_[r.pc_]->get_type() == InstrSet::INBOX) {
                target_x = 6;
                target_y = 9;

            } else if (r.instruction_[r.pc_]->get_type() == InstrSet::OUTBOX) {
                target_x = 74;
                target_y = 9;
            }

            int dir_x = (target_x - x == 0) ? 0 : (target_x - x > 0) ? 1 : -1;
            int dir_y = (target_y - y == 0) ? 0 : (target_y - y > 0) ? 1 : -1;
            while (target_x != x or target_y != y) {
                x = (target_x == x) ? x : x + dir_x;
                y = (target_y == y) ? y : y + dir_y;
                robot->move(x, y);
                clear_screen();
                robot->print();
                c.print();
                Sleep(5);
            }

            if (r.instruction_[r.pc_]->get_type() == InstrSet::INBOX) {
                robot->enable = true;
            }
        }
    }


    while (!r.finished()) {
        // 执行一步
        r.step();
        {
            // 获得执行完后机器人手里的数、
            robot->hand = r.hand_;
        }
        {
            {
                // 画机器人
                clear_screen();
                robot->print();
            }
            {
                c.set_state(r.input_, r.output_, r.ground_, r.instruction_, r.pc_);
                c.print();
                cin.get();
                clear_screen();
            }
            {
                if (r.finished()) {
                    continue;
                }
                // 移动机器人
                int x = robot->pos_x;
                int y = robot->pos_y;
                int target_x;
                int target_y;
                if (r.instruction_[r.pc_]->get_type() == InstrSet::INBOX) {
                    target_x = 6;
                    target_y = 9;

                } else if (r.instruction_[r.pc_]->get_type() == InstrSet::OUTBOX) {
                    target_x = 74;
                    target_y = 9;
                } else if (r.instruction_[r.pc_]->get_type() == InstrSet::COPYTO or
                           r.instruction_[r.pc_]->get_type() == InstrSet::COPYFROM or
                           r.instruction_[r.pc_]->get_type() == InstrSet::ADD or
                           r.instruction_[r.pc_]->get_type() == InstrSet::SUB) {
                    target_x = 10 + 5 * r.instruction_[r.pc_]->x_;
                    target_y = 6;
                }

                int dir_x = (target_x - x == 0) ? 0 : (target_x - x > 0) ? 1 : -1;
                int dir_y = (target_y - y == 0) ? 0 : (target_y - y > 0) ? 1 : -1;
                while (target_x != x or target_y != y) {
                    x = (target_x == x) ? x : x + dir_x;
                    y = (target_y == y) ? y : y + dir_y;
                    robot->move(x, y);
                    clear_screen();
                    robot->print();
                    c.print();
                    Sleep(5);
                }

                if (r.instruction_[r.pc_]->get_type() == InstrSet::INBOX) {
                    robot->enable = true;
                }
            }
        }
    }
    clear_screen();

    if (r.success(output)) {
        cout << "success";
    } else {
        cout << "fail";
    }
    cin.get();


}

void HRM_GUI::new_record() {
    // 新建存档界面
    int border_x = 50;
    int border_y = 10;
    std::cout << "+";
    for (int i = 0; i < border_x - 1; i++) {
        std::cout << "-";
    }
    std::cout << "+" << std::endl << '|';
    set_cursor(border_x / 2 - 4, 1);
    std::cout << "创建新角色";
    set_cursor(border_x, 1);
    std::cout << '|' << std::endl << '+';
    for (int i = 0; i < border_x - 1; i++) {
        std::cout << "-";
    }
    std::cout << '+' << std::endl << '|';
    set_cursor(1, 3);
    std::cout << "我们给您拍摄了工作照，哪一张您更满意呢：";
    coord last_pos1 = cursor_now();
    set_cursor(border_x, 3);
    std::cout << '|' << std::endl << '+';
    for (int i = 0; i < (border_x - 1) / 2; i++) {
        std::cout << "-";
    }
    std::cout << '+';
    for (int i = (border_x - 1) / 2 + 1; i < border_x - 1; i++) {
        std::cout << "-";
    }
    std::cout << '+' << std::endl;
    for (int i = 0; i < 9; i++) {
        std::cout << '|';
        for (int j = 0; j < (border_x - 1) / 2; j++) {
            std::cout << ' ';
        }
        std::cout << '|';
        for (int j = (border_x - 1) / 2 + 1; j < border_x - 1; j++) {
            std::cout << ' ';
        }
        std::cout << '|' << std::endl;
    }
    std::cout << '+';
    for (int i = 0; i < (border_x - 1) / 2; i++) {
        std::cout << "-";
    }
    std::cout << '+';
    for (int i = (border_x - 1) / 2 + 1; i < border_x - 1; i++) {
        std::cout << "-";
    }
    std::cout << '+' << std::endl;
    std::cout << "|请输入您的昵称：（不超过8个英文字符）";
    coord last_pos2 = cursor_now();
    set_cursor(border_x, last_pos2.y);
    std::cout << '|' << std::endl << '+';
    for (int i = 0; i < border_x - 1; i++) {
        std::cout << "-";
    }
    std::cout << '+' << std::endl;
    // 绘制工作照
    Robot *robot1 = new Robot(RobotType::male);
    Robot *robot2 = new Robot(RobotType::female);
    set_cursor(border_x / 4, 5);
    std::cout << "1. ";
    robot1->move(border_x / 4 - 2, 7);
    robot1->print();
    set_cursor(border_x / 4 * 3, 5);
    std::cout << "2. ";
    robot2->move(border_x / 4 * 3 - 2, 7);
    robot2->print();


    // 接受用户输入
    set_cursor(last_pos1.x, last_pos1.y);
    RobotType type;
    while (1) {
        char choice[100];
        std::cin >> choice;
        if (choice[0] == '1' && choice[1] == '\0') {
            type = RobotType::male;
            break;
        } else if (choice[0] == '2' && choice[1] == '\0') {
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
    while (1) {
        char choice[100];
        std::cin >> choice;
        if (strlen(choice) <= 8) {
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
    if (type == RobotType::male)
        robot = robot1;
    else
        robot = robot2;
    record->card[record_id].valid = true;
    record->card[record_id].level = 1;
    record->card[record_id].robot_type = type;
    record->card[record_id].name = name;
    record->save();
}

bool HRM_GUI::welcome() {
    // 欢迎界面
    int border_x = 50;
    int border_y = 10;
    std::cout << "+";
    for (int i = 0; i < border_x - 1; i++) {
        std::cout << "-";
    }
    std::cout << "+" << std::endl << '|';
    set_cursor(border_x / 2 - 12, 1);
    std::cout << "欢迎来到《人力资源机》!";
    set_cursor(border_x, 1);
    std::cout << '|' << std::endl << '+';
    for (int i = 0; i < border_x - 1; i++) {
        std::cout << "-";
    }
    std::cout << '+' << std::endl << '|';
    set_cursor(border_x / 2 - 8, 3);
    std::cout << "请选择您的工作证" << std::endl;
    set_cursor(border_x, 3);
    std::cout << '|' << std::endl << '+';

    for (int i = 0; i < border_x - 1; i++) {
        std::cout << '-';
    }
    std::cout << '+' << std::endl << '+';

    int separator1 = border_x / 3 - 1;
    int separator2 = border_x / 3 * 2 - 1;
    for (int i = 0; i < border_x - 1; i++) {
        if (i == separator1 || i == separator2)
            std::cout << '+';
        else
            std::cout << '-';
    }
    std::cout << '+' << std::endl;

    // 绘制三张工作证
    // 绘制边框
    for (int i = 0; i < 9; i++) {
        std::cout << '|';
        for (int j = 0; j < border_x - 1; j++) {
            if (j == separator1 || j == separator2)
                std::cout << '|';
            else
                std::cout << ' ';
        }
        std::cout << '|' << std::endl;
    }
    std::cout << '+';
    int last_y = cursor_now().y;
    for (int i = 0; i < border_x - 1; i++) {
        if (i == separator1 || i == separator2)
            std::cout << '+';
        else
            std::cout << '-';
    }
    std::cout << '+' << std::endl;
    // 绘制工作证
    Robot *robot1;
    Robot *robot2;
    Robot *robot3;
    set_cursor(5, 6);
    std::cout << " 存档一";
    if (record->card[0].valid) {
        robot1 = new Robot(record->card[0].robot_type);
        set_cursor(2, 7);
        std::cout << "最高关卡：" << record->card[0].level;
        set_cursor(2, 8);
        std::cout << "昵称：" << record->card[0].name;
        set_cursor(2, 9);
        std::cout << "工作照：";
        robot1->move(6, 11);
        robot1->print();
    } else {
        set_cursor(5, 7);
        std::cout << "暂无存档";
    }
    set_cursor(5 + border_x / 3, 6);
    std::cout << " 存档二";
    if (record->card[1].valid) {
        robot2 = new Robot(record->card[1].robot_type);
        set_cursor(2 + border_x / 3, 7);
        std::cout << "最高关卡：" << record->card[1].level;
        set_cursor(2 + border_x / 3, 8);
        std::cout << "昵称：" << record->card[1].name;
        set_cursor(2 + border_x / 3, 9);
        std::cout << "工作照：";
        robot2->move(6 + border_x / 3, 11);
        robot2->print();
    } else {
        set_cursor(5 + border_x / 3, 7);
        std::cout << "暂无存档";
    }
    set_cursor(6 + border_x / 3 * 2, 6);
    std::cout << " 存档三";
    if (record->card[2].valid) {
        robot3 = new Robot(record->card[2].robot_type);
        set_cursor(2 + 2 * border_x / 3, 7);
        std::cout << "最高关卡：" << record->card[2].level;
        set_cursor(2 + 2 * border_x / 3, 8);
        std::cout << "昵称：" << record->card[2].name;
        set_cursor(2 + 2 * border_x / 3, 9);
        std::cout << "工作照：";
        robot3->move(6 + 2 * border_x / 3, 11);
        robot3->print();
    } else {
        set_cursor(6 + border_x / 3 * 2, 7);
        std::cout << "暂无存档";
    }
    hide_cursor();

    // 选择存档
    set_cursor(0, last_y + 2);
    std::cout << '+';
    int new_y = cursor_now().y;
    for (int i = 0; i < border_x - 1; i++) {
        std::cout << '-';
    }
    std::cout << '+';
    set_cursor(border_x, last_y + 1);
    std::cout << '|';
    set_cursor(0, last_y + 1);
    std::cout << "|请选择存档：";
    int card_id;
    while (1) {
        char choice[100];
        std::cin >> choice;
        if (choice[0] >= '1' && choice[0] <= '3' && choice[1] == '\0') {
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
    if (record->card[card_id].valid) {
        set_cursor(border_x, last_y + 3);
        std::cout << '|';
        set_cursor(0, last_y + 4);
        std::cout << "|( 0 : 继续存档, 1 : 新建存档)";
        set_cursor(border_x, last_y + 4);
        std::cout << '|' << std::endl << '+';
        new_y = cursor_now().y;
        for (int i = 0; i < border_x; i++) {
            std::cout << '-';
        }
        std::cout << '+';
        set_cursor(0, last_y + 3);
        std::cout << "|继续上次的存档还是新建存档？：";
        while (1) {
            char choice[100];
            std::cin >> choice;
            if (choice[0] >= '0' && choice[0] <= '1' && choice[1] == '\0') {
                if (choice[0] == '0') {
                    // 继续存档
                    continue_last = true;
                    break;
                } else {
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
    } else {
        continue_last = false;
    }

    set_cursor(0, new_y + 1);
    if (continue_last) {
        if (card_id == 0) robot = robot1;
        else if (card_id == 1) robot = robot2;
        else robot = robot3;
        std::cout << "加载存档中...请稍候";
    } else
        std::cout << "新建存档中...请稍候";
    Sleep(1000);
    return continue_last;
}

void HRM_GUI::set_cursor(int _x, int _y) {
    // 设置光标位置
    HANDLE hOut;
    COORD pos = {static_cast<SHORT>(_x), static_cast<SHORT>(_y)};
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hOut, pos);
}

void HRM_GUI::clear_screen() {
    // 清屏
    system("cls");
    reset_cursor();
}

void HRM_GUI::hide_cursor() {
    if (cursor_visible) return;
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursor_info;
    GetConsoleCursorInfo(handle, &cursor_info);
    cursor_info.bVisible = false;
    SetConsoleCursorInfo(handle, &cursor_info);
}

coord HRM_GUI::cursor_now() {
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

void HRM_GUI::reset_cursor() {
    set_cursor(0, 0);
}