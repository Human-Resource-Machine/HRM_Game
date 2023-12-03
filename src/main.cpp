#include <iostream>
#include "hrm_gui.h"

Json::Value readJsonFile(const std::string &filename) {
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
}

void read(int level, std::vector<int> &input, std::vector<int> &output, int &ground,
          std::vector<std::string> &instructions) {
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
}

int main() {

    HRM_GUI my_game;
    while (1) {
        my_game.run();

        std::cin.get();
        std::cin.get();

        // 准备工作：期望的输入输出、地面的大小、可用的指令
        std::vector<int> input{};
        std::vector<int> output{};
        int ground;
        std::vector<std::string> available_instructions{};
        // 读入输入输出、地面的大小、可用的指令
        read(1, input, output, ground, available_instructions);

        // 读入编写的指令
        std::cout << "input:" << std::endl;
        std::string s;
        robot r;
        r.input_ = input;
        r.ground_ = std::vector<int>(ground, 0);
        while (getline(std::cin, s)) {
            if (s == "") break;
            r.add_instruction(s);
        }
        // TODO: 检查指令是否合法
        // 运行
        while (!r.finished()) {
            r.step();
        }


    }

    return 0;
}
