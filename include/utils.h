#pragma once
#include <windows.h>
#include <iostream>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

enum RobotType {
    male,
    female
};

namespace InstrSet {
    const int instr_num = 9;

    enum InstrType {
        INBOX,
        OUTBOX,
        COPYFROM,
        COPYTO,
        ADD,
        SUB,
        BUMPUP,
        BUMPDN,
        JUMP
    };
}

class Record {
public:
    struct PlayerRecord {
        int level;
        bool valid; // 存档是否有效
        RobotType robot_type;
    } card[3];

    Record()
    {
        // 从文件中读档
        for(int i = 0; i < 3; i++)
        {
            std::string filename = "../record/" + std::to_string(i+1) + ".rec";
            std::ifstream fin(filename);
            if(!fin.is_open())
            {
                // 读档失败
                card[i].valid = false;
                continue;
            }

            // 读档成功
            card[i].valid = true;
            // 先给默认值
            card[i].level = 1;
            card[i].robot_type = RobotType::male;
            std::string line;
            while(!fin.eof())
            {
                std::getline(fin, line);
                // std::cout << line << std::endl;
                if(line.substr(0,7) == "level: ")
                {
                    card[i].level = std::stoi(line.substr(7));
                    // std::cout << card[i].level << std::endl;
                }
                else if(line.substr(0,12) == "robot_type: ")
                {
                    std::string type = line.substr(12);
                    if(type == "male")
                        card[i].robot_type = RobotType::male;
                    else if(type == "female")
                        card[i].robot_type = RobotType::female;
                }
            }
        }
    }

    void save()
    {
        for(int i = 0;i < 3;i++)
        {
            std::string filename = "../record/" + std::to_string(i+1) + ".rec";
            if(!card[i].valid)
            {
                // 无效存档
                if(fs::exists(filename)) fs::remove(filename);
                continue;
            }
            
            std::ofstream fout(filename);
            fout << "level: " << card[i].level << std::endl;
            if(card[i].robot_type == RobotType::male)
                fout << "robot_type: male" << std::endl;
            else
                fout << "robot_type: female" << std::endl;
        }
    }

};

class Robot {
public:
    int pos_x;
    int pos_y;
    RobotType type;

    Robot(RobotType _type, int _x = 0, int _y = 0)
    {
        type = _type;
        pos_x = _x;
        pos_y = _y;
    }

    void move(int _x, int _y)
    {
        pos_x = _x;
        pos_y = _y;
    }

    void set_cursor(int _x, int _y)
    {
        // 设置光标位置
        HANDLE hOut;
        COORD pos = { static_cast<SHORT>(_x), static_cast<SHORT>(_y) };
        hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleCursorPosition(hOut, pos);
    }

    void printRobot()
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        int cursorX = 0, cursorY = 0;
        // 获取光标位置
        if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
            cursorX = csbi.dwCursorPosition.X;
            cursorY = csbi.dwCursorPosition.Y;
        }
        if(type == RobotType::female)
        {
            // 将光标移动到指定位置并打印机器人
            set_cursor(pos_x, pos_y);
            std::cout << "@___@";
            set_cursor(pos_x, pos_y+1);
            std::cout << "(@^@)";
            set_cursor(pos_x, pos_y+2);
            std::cout << "/ ~ \\";
            set_cursor(pos_x, pos_y+3);
            std::cout << " | | ";
        }
        else
        {
            // 将光标移动到指定位置并打印机器人
            set_cursor(pos_x, pos_y);
            std::cout << "_____";
            set_cursor(pos_x, pos_y+1);
            std::cout << "|@ @|";
            set_cursor(pos_x, pos_y+2);
            std::cout << "/ ! \\";
            set_cursor(pos_x, pos_y+3);
            std::cout << " | | ";
        }
        // 将光标移动回原来的位置
        set_cursor(cursorX, cursorY);
    }
};