#include <bits/stdc++.h>


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
        JUMPIFNEG,
        UNKNOWN
    };

    inline std::string to_string(InstrType type) {
        switch (type) {
            case INBOX:
                return "inbox";
                break;

            case OUTBOX:
                return "outbox";
                break;

            case COPYFROM:
                return "copyfrom";
                break;

            case COPYTO:
                return "copyto";
                break;

            case ADD:
                return "add";
                break;

            case SUB:
                return "sub";
                break;

            case JUMP:
                return "jump";
                break;

            case JUMPIFZERO:
                return "jumpifzero";
                break;

            case JUMPIFNEG:
                return "jumpifneg";
                break;

            case UNKNOWN:
                return "unknown";
                break;

        }
        return "unknown";
    }


    class instruction {
    public:
        int x_;

        explicit instruction(int x = 0) : x_(x) {}

        virtual void print() const = 0;

        virtual std::string to_string() const = 0;


        virtual void accept(std::vector<int> &ground,
                            std::vector<bool> &ground_y,
                            int &hand,
                            std::vector<int> &input,
                            std::vector<int> &output,
                            int &pc, bool &handy) = 0;

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
                    std::vector<bool> &ground_y,
                    int &hand,
                    std::vector<int> &input,
                    std::vector<int> &output,
                    int &pc, bool &handy) override {
            hand = input.front();
            input.erase(input.begin());
            handy = true;
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
                    std::vector<bool> &ground_y,
                    int &hand,
                    std::vector<int> &input,
                    std::vector<int> &output,
                    int &pc, bool &handy) override {
            output.push_back(hand);
            handy = false;
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
                    std::vector<bool> &ground_y,
                    int &hand,
                    std::vector<int> &input,
                    std::vector<int> &output,
                    int &pc, bool &handy) override {
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
                    std::vector<bool> &ground_y,
                    int &hand,
                    std::vector<int> &input,
                    std::vector<int> &output,
                    int &pc, bool &handy) override {
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
                    std::vector<bool> &ground_y,
                    int &hand,
                    std::vector<int> &input,
                    std::vector<int> &output,
                    int &pc, bool &handy) override {
            ground[x_] = hand;
            ground_y[x_] = true;
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
                    std::vector<bool> &ground_y,
                    int &hand,
                    std::vector<int> &input,
                    std::vector<int> &output,
                    int &pc, bool &handy) override {
            handy = true;
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
                    std::vector<bool> &ground_y,
                    int &hand,
                    std::vector<int> &input,
                    std::vector<int> &output,
                    int &pc, bool &handy) override {
            pc = x_ - 1;
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
                    std::vector<bool> &ground_y,
                    int &hand,
                    std::vector<int> &input,
                    std::vector<int> &output,
                    int &pc, bool &handy) override {
            if (hand == 0) {
                pc = x_ - 1;

            } else {
                pc += 1;
            }
        }

        InstrSet::InstrType get_type() const override {
            return InstrSet::JUMPIFZERO;
        }
    };

    class jumpifneg : public instruction {
    public:

        explicit jumpifneg(int x) : instruction(x) {

        }

        void print() const override {
            std::cout << "jumpifneg" << ' ' << x_ << std::endl;
        }


        [[nodiscard]] std::string to_string() const override {
            return std::string("jumpifneg ") + std::to_string(x_);
        }


        void accept(std::vector<int> &ground,
                    std::vector<bool> &ground_y,
                    int &hand,
                    std::vector<int> &input,
                    std::vector<int> &output,
                    int &pc, bool &handy) override {
            if (hand < 0) {
                pc = x_ - 1;
            } else {
                pc += 1;
            }
        }

        InstrSet::InstrType get_type() const override {
            return InstrSet::JUMPIFNEG;
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
                    std::vector<bool> &ground_y,
                    int &hand,
                    std::vector<int> &input,
                    std::vector<int> &output,
                    int &pc, bool &handy) override {

        }

        InstrSet::InstrType get_type() const override {
            return InstrSet::UNKNOWN;
        }
    };

}


class code_manager {
public:
    std::vector<int> input_{};
    std::vector<int> output_{};
    std::vector<int> ground_{};
    std::vector<bool> ground_y{};
    std::vector<InstrSet::instruction *> instruction_{};
    std::vector<std::string> available_instructions{};
    int pc_{};
    int hand_{};
    bool handy = false;
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
            } else if (op == "jumpifneg") {
                instruction_.push_back(new InstrSet::jumpifneg(x));
            } else {
                instruction_.push_back(new InstrSet::unknown());
            }
        }
    }

    void step() {
        instruction_[pc_]->accept(ground_, ground_y, hand_, input_, output_, pc_, handy);
        clock++;
    }

    bool finished() {
        return pc_ == instruction_.size() or (0 <= pc_ and pc_ < instruction_.size() and input_.empty() and
                                              instruction_[pc_]->get_type() == InstrSet::INBOX) or
               error_on_instruction();
    }

    bool success(std::vector<int> output) {
        // TODO: 检查是否成功
        if (finished() and not error_on_instruction()) {
            return output == output_;
        }
        return false;
    }

    bool error_on_instruction() {
        // shouldn't reach this
        if (pc_ > instruction_.size() or pc_ < 0) {
            assert(false);
        }
        if (pc_ == instruction_.size()) {
            return false;
        }
        return instruction_[pc_]->get_type() == InstrSet::UNKNOWN

               or std::find(available_instructions.begin(), available_instructions.end(),
                            to_string(instruction_[pc_]->get_type())) == available_instructions.end()

               or (instruction_[pc_]->get_type() == InstrSet::OUTBOX and !handy)

               or
               ((instruction_[pc_]->get_type() == InstrSet::ADD or instruction_[pc_]->get_type() == InstrSet::SUB) and
                (!handy or instruction_[pc_]->x_ < 0 or instruction_[pc_]->x_ >= ground_.size() or
                 !ground_y[instruction_[pc_]->x_]))

               or (instruction_[pc_]->get_type() == InstrSet::COPYTO and
                   (!handy or instruction_[pc_]->x_ < 0 or instruction_[pc_]->x_ >= ground_.size()))

               or (instruction_[pc_]->get_type() == InstrSet::COPYFROM and
                   (instruction_[pc_]->x_ < 0 or instruction_[pc_]->x_ >= ground_.size() or
                    !ground_y[instruction_[pc_]->x_])

                   or (instruction_[pc_]->get_type() ==
                       InstrSet::JUMP and (instruction_[pc_]->x_ <= 0 or instruction_[pc_]->x_ > instruction_.size()))

                   or (instruction_[pc_]->get_type() == InstrSet::JUMPIFZERO and
                       (!handy or instruction_[pc_]->x_ <= 0 or instruction_[pc_]->x_ > instruction_.size()))
//
//                   or ((instruction_[pc_]->get_type() ==
//                        InstrSet::JUMP or
//                        instruction_[pc_]->get_type() ==
//                        InstrSet::JUMPIFZERO or
//                        instruction_[pc_]->get_type() ==
//                        InstrSet::JUMPIFNEG) and
//                       (instruction_[pc_]->x_ < 0 or
//                        instruction_[pc_]->x_ >=
//                        instruction_.size()))

               );

    }


    ~code_manager() {
        for (auto i: instruction_) { delete i; }
    }
};


using namespace std;

int main() {
    int x;
    int m;
    cin >> x;
    cin >> m;
    cin.get();
    code_manager r;
    vector<int> output;
    int ground;
    if (x == 1) {
        r.available_instructions = {"inbox", "outbox"};
        r.input_ = {1, 2};
        output = {1, 2};
        ground = 0;
    } else if (x == 2) {
        r.available_instructions = {"inbox", "outbox", "copyfrom", "copyto", "add", "sub", "jump", "jumpifzero"};
        r.input_ = {3, 9, 5, 1, -2, -2, 9, -9};
        output = {-6, 6, 4, -4, 0, 0, 18, -18};
        ground = 3;
    } else if (x == 3) {
        r.available_instructions = {"inbox", "outbox", "copyfrom", "copyto", "add", "sub", "jump", "jumpifzero"};
        r.input_ = {6, 2, 7, 7, -9, 3, -3, -3};
        output = {7, -3};
        ground = 3;
    } else if (x == 4) {
        r.available_instructions = {"inbox", "outbox", "copyfrom", "copyto", "add", "sub", "jump", "jumpifzero",
                                    "jumpifneg"};
        r.input_ = {3, -7, -3, 0, -8, -7};
        output = {3, 7, 3, 0, 8, 7};
        ground = 3;
    } else if (x == 5) {
        r.available_instructions = {"inbox",
                                    "outbox",
                                    "copyto",
                                    "add",
                                    "jump"};
        r.input_ = {7, -3, 6, 0};
        output = {56, -24, 48, 0};
        ground = 5;
    } else if (x == 6) {
        r.available_instructions = {"inbox",
                                    "outbox",
                                    "copyfrom",
                                    "copyto",
                                    "add",
                                    "jump"};
        r.input_ = {6, -2, 2, 0};
        output = {240, -80, 80, 0};
        ground = 3;
    } else if (x == 7) {
        r.available_instructions = {"inbox",
                                    "outbox",
                                    "copyfrom",
                                    "copyto",
                                    "add",
                                    "jump",
                                    "jumpifzero",
                                    "sub"};
        r.input_ = {7, 8, 9, 9, 10, 11};
        output = {56, 81, 110};
        ground = 5;
    }
    r.ground_ = std::vector<int>(ground, 0);
    r.ground_y = std::vector<bool>(ground, false);

    for (int i = 0; i < m; ++i) {
        string s;
        getline(std::cin, s);
        r.add_instruction(s);
    }

//    if (r.instruction_[1]->get_type()==InstrSet::INBOX)
//    cout<< "Success";
//    else
//    {
//
//    }
//    return 0;

    while (!r.finished()) {
        // 执行一步
        r.step();
    }
    if (r.success(output)) {
        std::cout << "Success" << std::endl;
    } else if (r.error_on_instruction()) {
        std::cout << "Error on instruction " << r.pc_ + 1 << std::endl;
    } else {
        std::cout << "Fail" << std::endl;
    }


    return 0;
}