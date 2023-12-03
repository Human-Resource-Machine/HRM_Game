#pragma once

#include "json.h"
#include <bits/stdc++.h>

namespace fs = std::filesystem;

enum RobotType {
    male,
    female
};

namespace InstrSet {
    const int instr_num = 8;

    enum InstrType {
        INBOX,
        OUTBOX,
        COPYFROM,
        COPYTO,
        ADD,
        SUB,
        JUMP,
        JUMPIFZERO
    };


    class instruction {
    public:
        int x_;

        explicit instruction(int x = 0) : x_(x) {}

        virtual void print() const = 0;

        virtual std::string to_string() const = 0;


        virtual void accept(std::vector<int> &ground,
                            int &hand,
                            std::vector<int> &input,
                            std::vector<int> &output,
                            int &pc) = 0;

        virtual InstrSet::InstrType get_type() const = 0;
    };


    class inbox : public instruction {
    public:
        void print() const override {
            std::cout << "inbox" << std::endl;
        }

        [[nodiscard]] std::string to_string() const override {
            return "inbox";
        }

        void accept(std::vector<int> &ground,
                    int &hand,
                    std::vector<int> &input,
                    std::vector<int> &output,
                    int &pc) override {
            hand = input.front();
            input.erase(input.begin());
            pc += 1;
        }

        InstrSet::InstrType get_type() const override {
            return InstrSet::INBOX;
        }

    };

    class outbox : public instruction {
    public:
        void print() const override {
            std::cout << "outbox" << std::endl;
        }

        [[nodiscard]] std::string to_string() const override {
            return "outbox";
        }

        void accept(std::vector<int> &ground,
                    int &hand,
                    std::vector<int> &input,
                    std::vector<int> &output,
                    int &pc) override {
            output.push_back(hand);
            pc += 1;
        }

        InstrSet::InstrType get_type() const override {
            return InstrSet::OUTBOX;
        }
    };

    class add : public instruction {
    public:
        explicit add(int x) : instruction(x) {}

        void print() const override {
            std::cout << "add" << ' ' << x_ << std::endl;
        }

        [[nodiscard]] std::string to_string() const override {
            return std::string("add ") + std::to_string(x_);
        }

        void accept(std::vector<int> &ground,
                    int &hand,
                    std::vector<int> &input,
                    std::vector<int> &output,
                    int &pc) override {
            hand += ground[x_];
            pc += 1;
        }

        InstrSet::InstrType get_type() const override {
            return InstrSet::ADD;
        }

    };

    class sub : public instruction {
    public:
        sub(int x) : instruction(x) {}

        void print() const override {
            std::cout << "sub" << ' ' << x_ << std::endl;
        }

        [[nodiscard]] std::string to_string() const override {
            return std::string("sub ") + std::to_string(x_);
        }

        void accept(std::vector<int> &ground,
                    int &hand,
                    std::vector<int> &input,
                    std::vector<int> &output,
                    int &pc) override {
            hand -= ground[x_];
            pc += 1;
        }

        InstrSet::InstrType get_type() const override {
            return InstrSet::SUB;
        }
    };

    class copyto : public instruction {
    public:
        explicit copyto(int x) : instruction(x) {
        }

        void print() const override {
            std::cout << "copyto" << ' ' << x_ << std::endl;
        }

        [[nodiscard]] std::string to_string() const override {
            return std::string("copyto ") + std::to_string(x_);
        }

        void accept(std::vector<int> &ground,
                    int &hand,
                    std::vector<int> &input,
                    std::vector<int> &output,
                    int &pc) override {
            ground[x_] = hand;
            pc += 1;
        }

        InstrSet::InstrType get_type() const override {
            return InstrSet::COPYTO;
        }
    };

    class copyfrom : public instruction {
    public:
        copyfrom(int x) : instruction(x) {

        }

        void print() const override {
            std::cout << "copyfrom" << ' ' << x_ << std::endl;
        }

        std::string to_string() const override {
            return std::string("copyfrom ") + std::to_string(x_);
        }

        void accept(std::vector<int> &ground,
                    int &hand,
                    std::vector<int> &input,
                    std::vector<int> &output,
                    int &pc) override {
            hand = ground[x_];
            pc += 1;
        }

        InstrSet::InstrType get_type() const override {
            return InstrSet::COPYFROM;
        }
    };

    class jump : public instruction {
    public:
        jump(int x) : instruction(x) {}

        void print() const override {
            std::cout << "jump" << ' ' << x_ << std::endl;
        }

        std::string to_string() const override {
            return std::string("jump ") + std::to_string(x_);
        }

        void accept(std::vector<int> &ground,
                    int &hand,
                    std::vector<int> &input,
                    std::vector<int> &output,
                    int &pc) override {
            pc = x_;
        }

        InstrSet::InstrType get_type() const override {
            return InstrSet::JUMP;
        }
    };

    class jumpifzero : public instruction {
    public:
        explicit jumpifzero(int x) : instruction(x) {
        }

        void print() const override {
            std::cout << "jumpifzero" << ' ' << x_ << std::endl;
        }

        [[nodiscard]] std::string to_string() const override {

            return std::string("jumpifzero ") + std::to_string(x_);
        }

        void accept(std::vector<int> &ground,
                    int &hand,
                    std::vector<int> &input,
                    std::vector<int> &output,
                    int &pc) override {
            if (hand == 0) {
                pc = x_;

            } else {
                pc += 1;
            }
        }

        InstrSet::InstrType get_type() const override {
            return InstrSet::JUMPIFZERO;
        }
    };


}

class Record {
public:
    struct PlayerRecord {
        int level;
        bool valid; // 存档是否有效
        std::string name; // 应当是一个长度不超过 8 的字符串
        RobotType robot_type;
    } card[3];

    Record() {
        // 从文件中读档
        for (int i = 0; i < 3; i++) {
            std::string filename = "../record/" + std::to_string(i + 1) + ".rec";
            std::ifstream fin(filename);
            if (!fin.is_open()) {
                // 读档失败
                card[i].valid = false;
                continue;
            }

            // 读档成功
            card[i].valid = true;
            // 先给默认值
            card[i].level = 1;
            card[i].robot_type = RobotType::male;
            card[i].name = "default_name";
            std::string line;
            while (!fin.eof()) {
                std::getline(fin, line);
                // std::cout << line << std::endl;
                if (line.substr(0, 7) == "level: ") {
                    card[i].level = std::stoi(line.substr(7));
                    // std::cout << card[i].level << std::endl;
                } else if (line.substr(0, 12) == "robot_type: ") {
                    std::string type = line.substr(12);
                    if (type == "male")
                        card[i].robot_type = RobotType::male;
                    else if (type == "female")
                        card[i].robot_type = RobotType::female;
                } else if (line.substr(0, 6) == "name: ") {
                    card[i].name = line.substr(6, 8);
                }
            }
        }
    }

    void save() {
        for (int i = 0; i < 3; i++) {
            std::string filename = "../record/" + std::to_string(i + 1) + ".rec";
            if (!card[i].valid) {
                // 无效存档
                if (fs::exists(filename)) fs::remove(filename);
                continue;
            }

            std::ofstream fout(filename);
            fout << "level: " << card[i].level << std::endl;
            if (card[i].robot_type == RobotType::male)
                fout << "robot_type: male" << std::endl;
            else
                fout << "robot_type: female" << std::endl;
            fout << "name: " << card[i].name << std::endl;
        }
    }

};

class Robot {
public:
    int pos_x;
    int pos_y;
    RobotType type;

    Robot(RobotType _type, int _x = 0, int _y = 0) {
        type = _type;
        pos_x = _x;
        pos_y = _y;
    }

    void move(int _x, int _y) {
        pos_x = _x;
        pos_y = _y;
    }

    void set_cursor(int _x, int _y) {
        // 设置光标位置
        HANDLE hOut;
        COORD pos = {static_cast<SHORT>(_x), static_cast<SHORT>(_y)};
        hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleCursorPosition(hOut, pos);
    }

    void printRobot() {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        int cursorX = 0, cursorY = 0;
        // 获取光标位置
        if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
            cursorX = csbi.dwCursorPosition.X;
            cursorY = csbi.dwCursorPosition.Y;
        }
        if (type == RobotType::female) {
            // 将光标移动到指定位置并打印机器人
            set_cursor(pos_x, pos_y);
            std::cout << "@___@";
            set_cursor(pos_x, pos_y + 1);
            std::cout << "(@^@)";
            set_cursor(pos_x, pos_y + 2);
            std::cout << "/ ~ \\";
            set_cursor(pos_x, pos_y + 3);
            std::cout << " | | ";
        } else {
            // 将光标移动到指定位置并打印机器人
            set_cursor(pos_x, pos_y);
            std::cout << "__!__";
            set_cursor(pos_x, pos_y + 1);
            std::cout << "|@ @|";
            set_cursor(pos_x, pos_y + 2);
            std::cout << "/ ! \\";
            set_cursor(pos_x, pos_y + 3);
            std::cout << " | | ";
        }
        // 将光标移动回原来的位置
        set_cursor(cursorX, cursorY);
    }
};

class robot {
public:
    std::vector<int> input_{};
    std::vector<int> output_{};
    std::vector<int> ground_{};
    std::vector<InstrSet::instruction *> instruction_{};
    int pc_{};
    int hand_{};
    int clock{};

    void add_instruction(std::string s) {
        if (s == "inbox") {
            instruction_.push_back(new InstrSet::inbox());
        } else if (s == "outbox") {
            instruction_.push_back(new InstrSet::outbox());
        } else {
            std::istringstream instr(s);
            std::string op;
            int x;
            instr >> op >> x;
            if (op == "add") {
                instruction_.push_back(new InstrSet::add(x));
            } else if (op == "sub") {
                instruction_.push_back(new InstrSet::sub(x));
            } else if (op == "copyto") {
                instruction_.push_back(new InstrSet::copyto(x));
            } else if (op == "copyfrom") {
                instruction_.push_back(new InstrSet::copyfrom(x));
            } else if (op == "jump") {
                instruction_.push_back(new InstrSet::jump(x));
            } else if (op == "jumpifzero") {
                instruction_.push_back(new InstrSet::jumpifzero(x));
            }
        }
    }

    void step() {
        instruction_[pc_]->accept(ground_, hand_, input_, output_, pc_);
        clock++;
    }

    bool finished() {
        return pc_ == instruction_.size() or (input_.empty() and instruction_[pc_]->get_type() == InstrSet::JUMPIFZERO);
    }

    bool success(std::vector<int> output) {
        // TODO: 检查是否成功
        if (finished()) {
            if (output == output_) {
                return true;
            }
        }
        return false;
    }


    ~robot() {
        for (auto i: instruction_) { delete i; }
    }
};

