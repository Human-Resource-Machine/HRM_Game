#pragma once

#include "json.h"
#include <bits/stdc++.h>
#include"hrm_gui.h"

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
        JUMPIFZERO,
        UNKNOWN
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

    class unknown : public instruction {
    public:


        void print() const override {
            std::cout << "unknown" << std::endl;
        }

        [[nodiscard]] std::string to_string() const override {

            return std::string("unknown");
        }

        void accept(std::vector<int> &ground,
                    int &hand,
                    std::vector<int> &input,
                    std::vector<int> &output,
                    int &pc) override {

        }

        InstrSet::InstrType get_type() const override {
            return InstrSet::UNKNOWN;
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

class Object {
public:
    int pos_x;
    int pos_y;

    Object(int _x = 0, int _y = 0) {
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

    virtual void print() = 0;
};

class Robot : public Object {
public:
    RobotType type;
    int hand;
    bool ingame = false;
    bool enable = false;

    explicit Robot(RobotType _type, int _x = 0, int _y = 0) : Object(_x, _y), type(_type) {}

    void print() override {
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


            if (ingame) {
                set_cursor(pos_x, pos_y - 3);
                std::cout << "+---+";
                set_cursor(pos_x, pos_y - 2);
                enable ? std::cout << '|' << std::setw(3) << std::setfill(' ') << hand << '|' : std::cout << '|'
                                                                                                          << "   "
                                                                                                          << '|';
                set_cursor(pos_x, pos_y - 1);
                std::cout << "+---+";
            }

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

            if (ingame) {
                set_cursor(pos_x, pos_y - 3);
                std::cout << "+---+";
                set_cursor(pos_x, pos_y - 2);
                enable ? std::cout << '|' << std::setw(3) << std::setfill(' ') << hand << '|' : std::cout << '|'
                                                                                                          << "   "
                                                                                                          << '|';
                set_cursor(pos_x, pos_y - 1);
                std::cout << "+---+";
            }

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

class Box : public Object {
public:

    int num_;
    bool enable;

    explicit Box(int _x = 0, int _y = 0, int num = 0) : Object(_x, _y), num_(num) {}


    void print() override {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        int cursorX = 0, cursorY = 0;
        // 获取光标位置
        if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
            cursorX = csbi.dwCursorPosition.X;
            cursorY = csbi.dwCursorPosition.Y;
        }
        {
            // 将光标移动到指定位置并打印机器人
            set_cursor(pos_x, pos_y);
            std::cout << "+---+";
            set_cursor(pos_x, pos_y + 1);
            enable ? std::cout << '|' << std::setw(3) << std::setfill(' ') << num_ << '|' : std::cout << '|' << "   "
                                                                                                      << '|';
            set_cursor(pos_x, pos_y + 2);
            std::cout << "+---+";
        }
        // 将光标移动回原来的位置
        set_cursor(cursorX, cursorY);
    }
};

class InstructionBox : public Object {
public:

    InstrSet::InstrType type_;
    bool pc_ = false;
    int x_ = 0;

    explicit InstructionBox(int _x, int _y, InstrSet::InstrType type, bool pc, int x) : Object(_x, _y),
                                                                                        type_(type), pc_(pc),
                                                                                        x_(x) {}


    void print() override {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        int cursorX = 0, cursorY = 0;
        // 获取光标位置
        if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
            cursorX = csbi.dwCursorPosition.X;
            cursorY = csbi.dwCursorPosition.Y;
        }
        {
            set_cursor(pos_x, pos_y);
            std::cout << (pc_ ? '*' : ' ');
            if (type_ == InstrSet::INBOX)
                std::cout << "inbox";
            else if (type_ == InstrSet::OUTBOX)
                std::cout << "outbox";
            else if (type_ == InstrSet::JUMP)
                std::cout << "jump" << ' ' << x_;
            else if (type_ == InstrSet::JUMPIFZERO)
                std::cout << "jumpifzero" << ' ' << x_;
            else if (type_ == InstrSet::ADD)
                std::cout << "add" << ' ' << x_;
            else if (type_ == InstrSet::SUB)
                std::cout << "sub" << ' ' << x_;
            else if (type_ == InstrSet::COPYFROM)
                std::cout << "copyfrom" << ' ' << x_;
            else if (type_ == InstrSet::COPYTO)
                std::cout << "copyto" << ' ' << x_;
            else if (type_ == InstrSet::UNKNOWN)
                std::cout << "unknown";
        }

        // 将光标移动回原来的位置
        set_cursor(cursorX, cursorY);
    }
};

class Curtain : public Object {
public:
    std::vector<Box> input_box{Box{0, 3 + 0},
                               Box{0, 3 + 3},
                               Box{0, 3 + 6},
                               Box{0, 3 + 9},
                               Box{0, 3 + 12},
                               Box{0, 3 + 15},
                               Box{0, 3 + 18}};
    std::vector<Box> output_box{Box{80, 3 + 0},
                                Box{80, 3 + 3},
                                Box{80, 3 + 6},
                                Box{80, 3 + 9},
                                Box{80, 3 + 12},
                                Box{80, 3 + 15},
                                Box{80, 3 + 18}};
    std::vector<Box> ground_box{};
    std::vector<InstructionBox> instruction_box{};

    void set_state(std::vector<int> &input, std::vector<int> &output, std::vector<int> &ground,
                   std::vector<InstrSet::instruction *> instruction, int pc) {
        for (auto &box: input_box) {
            box.enable = false;
        }
        for (auto &box: output_box) {
            box.enable = false;
        }
        for (int i = 0; i < std::min(input.size(),input_box.size()); i++) {
            input_box[i].enable = true;
            input_box[i].num_ = input[i];
        }
        for (int i = 0; i < std::min(output.size(), output_box.size()); i++) {
            output_box[i].enable = true;
            output_box[i].num_ = output[output.size() - 1 - i];
        }
        for (int i = 0; i < ground.size(); i++) {
            ground_box.push_back(Box{18 + 6 * i, 12, ground[i]});
        }
        for (int i = 0; i < instruction.size(); i++) {
            instruction_box.push_back(InstructionBox(100, i, instruction[i]->get_type(), i == pc, instruction[i]->x_));
        }
    }

    void print() override {
        for (auto &box: input_box) {
            box.print();
        }
        for (auto &box: output_box) {
            box.print();
        }
        for (auto &box: ground_box) {
            box.print();
        }
        for (auto &box: instruction_box) {
            box.print();
        }

    }
};

class code_manager {
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
            std::string x_;
            int x = 0;

            instr >> op;
            std::getline(instr, x_);

            x_.erase(0, x_.find_first_not_of(" "));
            x_.erase(x_.find_last_not_of(" ") + 1);

            if (x_ == std::to_string(atoi(x_.c_str()))) {

                x = atoi(x_.c_str());

            } else {
                instruction_.push_back(new InstrSet::unknown());
                return;
            }
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
        return pc_ == instruction_.size() or (input_.empty() and instruction_[pc_]->get_type() == InstrSet::INBOX);
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


    ~code_manager() {
        for (auto i: instruction_) { delete i; }
    }
};

