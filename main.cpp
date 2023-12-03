#include <iostream>
#include <format>
#include <vector>

class code;

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
                        code &code) = 0;
};

class code {
public:
    std::vector<instruction *> instructions_;
    int pc;

    code() : pc(0) {};

    code(const code &) = delete;

    code(code &&) = delete;


    code &operator=(const code &) = delete;

    code &operator=(code &&) = delete;

    ~code() {
        for (auto *instr: instructions_) {
            delete instr;
        }
    }

    void push_back(instruction *instr) {
        instructions_.push_back(instr);
    }

    void print() const {
        for (int i = 0; i < instructions_.size(); ++i) {
            std::cout << (i == pc ? "*" : " ");
            std::cout << i << ' ';
            instructions_[i]->print();
            if (i != instructions_.size() - 1) {
                std::cout << std::endl;
            }
        }
    }

    [[nodiscard]] std::string to_string() const {
        std::string result;
        for (int i = 0; i < instructions_.size(); ++i) {
            result += i == pc ? "*" : " ";

            result += std::format("{} ", i);
            result += instructions_[i]->to_string();
            if (i != instructions_.size() - 1) {
                result += '\n';
            }
        }
    }

    [[nodiscard]] size_t size() const {
        return instructions_.size();
    }
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
                code &code) override {
        hand = input.front();
        input.erase(input.begin());
        code.pc += 1;
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
                code &code) override {
        output.push_back(hand);
        code.pc += 1;
    }
};

class add : public instruction {
public:
    explicit add(int x) : instruction(x) {}

    void print() const override {
        std::cout << "add" << ' ' << x_ << std::endl;
    }

    [[nodiscard]] std::string to_string() const override {
        return std::format("add {}", x_);
    }

    void accept(std::vector<int> &ground,
                int &hand,
                std::vector<int> &input,
                std::vector<int> &output,
                code &code) override {
        hand += ground[x_];
        code.pc += 1;
    }

};

class sub : public instruction {
public:
    sub(int x) : instruction(x) {}

    void print() const override {
        std::cout << "sub" << ' ' << x_ << std::endl;
    }

    [[nodiscard]] std::string to_string() const override {
        return std::format("sub {}", x_);
    }

    void accept(std::vector<int> &ground,
                int &hand,
                std::vector<int> &input,
                std::vector<int> &output,
                code &code) override {
        hand -= ground[x_];
        code.pc += 1;
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
        return std::format("copyto {}", x_);
    }

    void accept(std::vector<int> &ground,
                int &hand,
                std::vector<int> &input,
                std::vector<int> &output,
                code &code) override {
        ground[x_] = hand;
        code.pc += 1;
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
        return std::format("copyfrom {}", x_);
    }

    void accept(std::vector<int> &ground,
                int &hand,
                std::vector<int> &input,
                std::vector<int> &output,
                code &code) override {
        hand = ground[x_];
        code.pc += 1;
    }
};

class jump : public instruction {
public:
    jump(int x) : instruction(x) {}

    void print() const override {
        std::cout << "jump" << ' ' << x_ << std::endl;
    }

    std::string to_string() const override {
        return std::format("jump {}", x_);
    }

    void accept(std::vector<int> &ground,
                int &hand,
                std::vector<int> &input,
                std::vector<int> &output,
                code &code) override {
        code.pc = x_;
    }
};

class jumpzero : public instruction {
public:
    explicit jumpzero(int x) : instruction(x) {
    }

    void print() const override {
        std::cout << "jumpzero" << ' ' << x_ << std::endl;
    }

    [[nodiscard]] std::string to_string() const override {
        return std::format("jumpzero {}", x_);
    }

    void accept(std::vector<int> &ground,
                int &hand,
                std::vector<int> &input,
                std::vector<int> &output,
                code &code) override {
        if (hand == 0) {
            code.pc = x_;

        } else {
            code.pc += 1;
        }
    }
};


class robot {
public:
    std::vector<int> ground_;
    int hand_;
    std::vector<int> input_;
    std::vector<int> output_;
    code code_;
    int &pc = code_.pc;

    void init() {
        pc = 0;
    }

    void run(std::vector<int> &ans) {
        while (pc < code_.size()) {
            code_.instructions_[pc]->accept(ground_, hand_, input_, output_, code_);
            if (ans == output_ and input_.empty()) {
                std::cout << "success" << std::endl;
                break;
            }
        }
    }

};

int main() {
    robot robot;
    robot.ground_ = {};
    robot.hand_ = 0;
    robot.input_ = {1, 2};
    robot.output_ = {};
    robot.code_.push_back(new inbox());
    robot.code_.push_back(new outbox());
    robot.code_.push_back(new inbox());
    robot.code_.push_back(new outbox());
    robot.code_.pc = 0;
    robot.init();
    auto ans = std::vector<int>{1, 2};
    robot.run(ans);
    return 0;
}